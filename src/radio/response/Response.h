#include <stdint.h> // biblioteka do uint-ów



struct Response {
  uint8_t type;
  uint8_t result;
  uint16_t value;
  uint32_t messages;
};
