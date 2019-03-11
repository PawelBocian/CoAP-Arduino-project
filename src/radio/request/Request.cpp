#include "Request.h"

void Request::setRequestValues(uint8_t type, uint8_t object,uint16_t value){
  this->type = type;
  this->object = object;
  this->value = value;
}
