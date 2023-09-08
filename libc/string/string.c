#include <string.h>

int memcmp(const void* ap, const void* bp, size_t size) {
	const unsigned char* a = (const unsigned char*) ap;
	const unsigned char* b = (const unsigned char*) bp;
	for (size_t i = 0; i < size; i++) {
		if (a[i] < b[i]) return -1;
		else if (a[i] > b[i]) return 1;
	}
	// if the two pieces of memory are identical
	return 0;
}

void* memcpy(void* dstp, const void* srcp, size_t size) {
	unsigned char* dst = (unsigned char*) dstp;
	const unsigned char* src = (const unsigned char*) srcp;
	for (size_t i = 0; i < size; i++) {
		dst[i] = src[i];
	}
	return dstp;
}

// try not to overwrite the source material, unlike memcpy
void* memmove(void* dstp, const void* srcp, size_t size) {
	unsigned char* dst = (unsigned char*) dstp;
	const unsigned char* src = (const unsigned char*) srcp;
	if (dst < src) {
		for (size_t i = 0; i < size; i++)
			dst[i] = src[i];
	}
	else {
		for (size_t i = 0; i != 0; i--) {
			dst[i-1] = src[i-1];
		}
	}
	return dstp;
}

void* memset(void* bufp, int value, size_t size) {
	unsigned char* buf  = (unsigned char*) bufp;
	for (size_t i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufp;
}

size_t strlen(const char* str) {
	size_t len = 0;
	while (*str++) len++;
	return len;
}
