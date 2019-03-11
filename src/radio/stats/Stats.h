#include <stdint.h> // biblioteka do uint-ów


struct Stats {
  uint8_t packagesReceived;
  uint8_t sendingComplete;
  uint8_t messagesSend;

  void SetStatsValue(uint8_t packagesReceived, uint8_t osendingComplete,uint8_t messagesSend);
};
