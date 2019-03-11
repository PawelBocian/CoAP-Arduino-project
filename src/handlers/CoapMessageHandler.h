//
// Created by Wojtek on 19.01.2019.
//

#ifndef COAP_MESSAGE_HANDLER_H
#define COAP_MESSAGE_HANDLER_H

#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Dhcp.h>

#include "RadioHandler.h"
#include "coap/Message.h"

#include "utils/Attributes.h"
#include "utils/Enums.h"
#include "utils/Strings.h"

#include "memory/MemoryFree.h"

class CoapMessageHandler {

private:
    Message  parsedMessage;
    Message  assembledMessage;
    uint16_t assembledPacketSize;

    uint8_t contentFormat;

public:
    RadioHandler radioHandler;

    void     handleMessage(uint8_t *packetBuffer, uint16_t packetSize, EthernetUDP udp);
    void     clearStringBuffer (char *stringBuffer);
    void     printStringBuffer (char *stringBuffer);
    uint16_t waitForResponse();
    Stats waitForStatistics();

};

#endif //COAP_MESSAGE_HANDLER_H
