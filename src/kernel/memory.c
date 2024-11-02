#include <memory.h>

void far* memset(void far* ptr, int value, uint16_t num) {
  uint8_t* u8Ptr = (uint8_t*) ptr;

  for (int i = 0; i < num; i++) {
    u8Ptr[i] = (uint8_t) value;
  }

  return ptr;
}

void far* memcpy(void far* dst, const void far* src, uint16_t num) {
  uint8_t* u8Dst = (uint8_t*) dst;
  const uint8_t* u8Src = (const uint8_t*) src;

  for (int i = 0; i < num; i++) {
    u8Dst[i] = u8Src[i];
  }

  return dst;
}

int memcmp(const void far* ptr1, const void far* ptr2, uint16_t num) {
  const uint8_t* u8Ptr1 = (const uint8_t*) ptr1;
  const uint8_t* u8Ptr2 = (const uint8_t*) ptr2;

  for (int i = 0; i < num; i++) {
    if (u8Ptr1[i] != u8Ptr2[i]) {
      return (u8Ptr1[i] > u8Ptr2[i]) ? 1 : -1;
    }
  }

  return 0;
}
