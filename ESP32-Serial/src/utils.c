#include "utils.h"

unsigned short calc_crc(unsigned char* buff, int count) {
  unsigned short crc = 0;
  for (int i = 0; i < count; i++) {
    crc = crc ^ (buff[i] << 8);
    for (int j = 0; j < 8; j++) {
      if (crc & 0x8000) {
        crc = (crc << 1) ^ 0x1021;
      } else {
        crc = crc << 1;
      }
    }
  }
  return crc;
}