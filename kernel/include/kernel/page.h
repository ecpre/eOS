#include <stdint.h>

typedef struct page_directory_entry {
	uint32_t present	: 1;
	uint32_t rw		: 1;
	uint32_t user		: 1;
	uint32_t writethrough	: 1;
	uint32_t cache_disable	: 1;
	uint32_t accessed	: 1;
	uint32_t available	: 1;
	uint32_t size		: 1;
	uint32_t global		: 1;
	uint32_t available2	: 3;
	uint32_t addr		: 20;
} page_directory_entry_t;

typedef struct page_table_entry {
	uint32_t present	: 1;
	uint32_t rw		: 1;
	uint32_t user		: 1;
	uint32_t writethrough	: 1;
	uint32_t cache_disable	: 1;
	uint32_t accessed	: 1;
	uint32_t dirty		: 1;
	uint32_t pat		: 1;
	uint32_t global		: 1;
	uint32_t available	: 3;
	uint32_t addr		: 20;
} page_table_entry_t;

typedef struct page_directory {
	page_directory_entry_t tables[1024];
} page_directory_t;

typedef struct page_table {
	page_directory_entry_t pages[1024];
} page_table_t;
