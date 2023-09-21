// linked list of memory

typedef struct frame_map {
	uint32_t start_frame;
	uint32_t length;
	struct frame_map* next;
	uint8_t free;
} frame_map_t;
