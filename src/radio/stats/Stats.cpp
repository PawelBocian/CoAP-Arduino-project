#include "Stats.h"


void Stats::SetStatsValue(uint8_t packagesReceived, uint8_t osendingComplete,uint8_t messagesSend){
  this->packagesReceived = packagesReceived;
  this->sendingComplete = osendingComplete;
  this->messagesSend = messagesSend;
}
