#include <stdint.h> // biblioteka do uint-ów



struct Request {
  uint8_t type;
  uint8_t object;
  uint16_t value;

  void setRequestValues(uint8_t type, uint8_t object,uint16_t value);

};
