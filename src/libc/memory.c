#include <memory.h>

void* memset(void* ptr, int value, size_t size) {
	uint8_t* u8Ptr = (uint8_t*) ptr;

	for (size_t i = 0; i < size; i++) {
		*u8Ptr = value;
		u8Ptr++;
	}

	return ptr;
}

int memcmp(const void* ptr1, const void* ptr2, size_t size) {
	const unsigned char* a = (const unsigned char*) ptr1;
	const unsigned char* b = (const unsigned char*) ptr2;

	for (size_t i = 0; i < size; i++) {
		if (a[i] < b[i]) {
			return -1;
		} else if (b[i] > a[i]) {
			return 1;
		}
	}

	return 0;
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;

	for (size_t i = 0; i < size; i++) {
		dst[i] = src[i];
	}

	return dstptr;
}

void* memmove(void* dstptr, const void* srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;

	if (dst < src) {
		for (size_t i = 0; i < size; i++) {
			dst[i] = src[i];
		}
	} else {
		for (size_t i = size; i != 0; i--) {
			dst[i-1] = src[i-1];
		}
	}

	return dstptr;
}