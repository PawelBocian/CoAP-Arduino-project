#ifndef RADIO_HANDLER_H
#define RADIO_HANDLER_H

#include <Arduino.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#include "radio/request/Request.h"
#include "radio/response/Response.h"
#include "radio/stats/Stats.h"

class RadioHandler{
public:
    RF24 radio = RF24(7,8);
    RF24Network network = RF24Network(this->radio);

    uint16_t UNO_node = 01;
    uint16_t Mini_node = 00;

    RF24NetworkHeader uno_header = RF24NetworkHeader(this->UNO_node);
    RF24NetworkHeader mini_header = RF24NetworkHeader(this->Mini_node);

    struct Request req_message;
    struct Response res_message;
    struct Stats stats_message;

    void setup();

    uint8_t sendRequest(Request req);
    uint8_t setSpeakerVolume(int value);

    uint8_t getSpeakerVolume();
    uint8_t getPotentiometerValue();
    uint8_t getStatistics();

    uint8_t observeResource(int state);
};

#endif //RADIO_HANDLER_H
