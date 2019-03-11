#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Dhcp.h>

#include "coap/Message.h"
#include "handlers/CoapMessageHandler.h"

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(169,254,13,25);
short localPort = 8080;
EthernetUDP Udp;

uint16_t packetSize;
uint8_t packetBuffer[MAX_BUFFER];

CoapMessageHandler coapMessageHandler;

using namespace Enums;

void setup()
{
  Serial.begin(115200);
  // Ethernet.begin(mac, ip);
  Ethernet.begin(mac);
  Udp.begin(localPort);

  Serial.print(F("Listening at: "));
  Serial.print(Ethernet.localIP());
  Serial.println(F("..."));
  coapMessageHandler.radioHandler.setup();
}

void loop()
{
    packetSize = Udp.parsePacket();
    if(packetSize){
        //writes incoming udp payload into packetBuffer
        Udp.read(packetBuffer, MAX_BUFFER);

        //handles the message
        coapMessageHandler.handleMessage(packetBuffer, packetSize, Udp);
    }
}
