#include <kernel/page.h>
#include <lib/bitmap.h>
#include <string.h>
#include <stdint.h>

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

	for(int i = 0; i < 1024; i++) {
		new_pt->pages[i] = new_page;
		new_page.addr++;
	}
	
	// hardcoded reservation for this. maybe i should work on the
	// page bitmap before doing things like this but it's fine
	page_directory_t* new_pd = (page_directory_t*) 0xC0400000;
	
	new_pd->tables[768] = boot_page_dir->tables[768];
	new_pd_entry.addr = 0x400;
	new_pd->tables[769] = new_pd_entry;
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
