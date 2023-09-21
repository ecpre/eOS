#include <kernel/page.h>
#include <lib/bitmap.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <kernel/memory.h>

void swap_page_directory(page_directory_t* page_dir) {
	uint32_t raw_page_dir = (uint32_t) page_dir;
	asm volatile("mov %0, %%cr3" :: "r"(raw_page_dir));
}

// adds a second page table to store paging structs and swaps to new
// page directory located at page directory entry no. 1023
void paging_reinit(page_directory_t* boot_page_dir, uint32_t creation_loc) {
	int creation_loc_align = creation_loc >> 12;
	// create new page to store new page directory
	page_directory_entry_t new_pd_entry;
	memset(&new_pd_entry, 0, 4);
	new_pd_entry.present = 1;
	new_pd_entry.rw = 1;
	new_pd_entry.addr = creation_loc_align;
	boot_page_dir->tables[769] = new_pd_entry;
	
	creation_loc += 0xC0000000;
	
	page_table_entry_t new_page;
	memset(&new_page, 0, 4);
	new_page.present = 1;
	new_page.rw = 1;
	// hardcoded phsyical address of page 2
	new_page.addr = 0x400;

	page_table_t* new_pt = (page_table_t*) creation_loc;
	
	uint32_t kernel_pt_loc_virt = boot_page_dir->tables[768].addr << 12;
	kernel_pt_loc_virt += 0xC0000000;

	for(int i = 0; i < 1024; i++) {
		new_pt->pages[i] = new_page;
		new_page.addr++;
	}

	// hardcoded reservation for this. maybe i should work on the
	// page bitmap before doing things like this but it's fine
	page_directory_t* new_pd = (page_directory_t*) 0xC0400000;
	memcpy((void*)0xC0701000, (void*) kernel_pt_loc_virt, 4096);
	
	new_pd->tables[768] = boot_page_dir->tables[768];
	new_pd->tables[768].addr = 0x701;
	new_pd_entry.addr = 0x400;
	new_pd->tables[1023] = new_pd_entry;

	// this arithmetic could be rendered unnecessary by just splitting the
	// creation loc into two different variables i guess
	void* new_pd_physical = (void*) new_pd;
	new_pd_physical -= 0xC0000000;

	swap_page_directory(new_pd_physical);
}

// can use the fact that the page directory is always at 0xFFFFF000
void* get_physical_addr(void* virtual_addr) {

	uint32_t pd_index = (uint32_t) virtual_addr >> 22;
	uint32_t pt_index = (uint32_t) virtual_addr >> 12 & 0x03FF;

	page_directory_t* pd = (page_directory_t*) 0xFFFFF000;
	// return a null pointer if nothing there I guess. for now
	if (pd->tables[pd_index].present == 0) return (void*) 0;

	page_table_t* pt = (page_table_t*) 0xFFC00000 + pd_index;
	if (pt->pages[pt_index].present == 0) return (void*) 0;

	uint32_t page_addr = pt->pages[pt_index].addr << 12;
	
	// page table addr + the last bits of the original virt addr
	return (void*)(page_addr + ((uint32_t) virtual_addr & 0xFFF));

}

// creates two bitmaps. both of 128KB size for simplicty. one for page frames and
// one for pages. returns a pointer to the frame bitmap. add 128k+4 to get pages
// NOTE: void* physical_memory will be a pointer to something like
// memory_map_t* physical_memory once I get this working! don't forget that!
bitmap* page_frame_map_init(frame_map_t* physical_memory) {
	page_directory_t* pd = (page_directory_t*) 0xFFFFF000;
	page_table_t* bitmap_pt = (page_table_t*) 0xFFC00000 + 769;
	// something has gone wrong if this page table exists before this is called
	if (pd->tables[769].present == 1) abort();
	pd->tables[769].present = 1;
	pd->tables[769].rw = 1;
	pd->tables[769].addr = 0x702;

	for (int i = 0; i < 1024; i++) {
		bitmap_pt->pages[i].present = 1;
		bitmap_pt->pages[i].rw = 1;
		bitmap_pt->pages[i].addr = 0x800+i;
	}
	// marking frame bitmap as used will be a little more complicated
	// TODO: do this tomorrow or whenever I get back to this!	
	bitmap* frame_bitmap = (bitmap*) 0xC0400000;
	frame_bitmap->bytes = (uint8_t*) 0xC0400004;
	memset(frame_bitmap->bytes, 0xFF, 0x20000);
	while (physical_memory != NULL) {
		//if not free
		if (physical_memory->free == 0) {
			for (uint32_t i = 0; i<physical_memory->length; i++) {
				set_bitmap(frame_bitmap, i+physical_memory->start_frame, 0);
			}
		}
		physical_memory = physical_memory->next;
	}
	
	// mark page bitmap	
	bitmap* page_bitmap = (bitmap*) 0xC0420004;
	page_bitmap->bytes = (uint8_t*) 0xC0420008;
	memset(page_bitmap->bytes, 0, 0x20000);
	// all of first page and first 64 pages in second page are used
	// (for now, all of the first page isn't really used so this is
	// not exactly ideal)
	for(int i = 0; i < 1088; i++) {
		set_bitmap(page_bitmap, 0xC0000+i, 1);
	}
	// mark page directory as used
	for(int i = 0; i < 1024; i++) {
		set_bitmap(page_bitmap, 0xFFC00+i, 1);
	}

	return frame_bitmap;
}
