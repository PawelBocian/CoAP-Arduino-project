#include "RadioHandler.h"

void RadioHandler::setup(){
    SPI.begin();
    this->radio.begin();
    this->network.begin(45,UNO_node);
    Serial.println(F("Set up radio and network"));
}

uint8_t RadioHandler::sendRequest(Request req) {                               //Sending request to Arduino Mini_pro
  int didSend = this->network.write(mini_header,&req,sizeof(req));             //Sending "req" message to Arduino Mini_pro
  if(didSend == 0){
      Serial.println(F("Sending Failed"));
      return 1;                                                                //Returning 1 when Sending process failed.
  }else{
      Serial.println(F("Sending OK"));
      return 0;                                                                //Returning 0 when Sending process successful
  }
}

uint8_t RadioHandler::setSpeakerVolume(int value){                             // Set Mini_pro Buzzer tone
    if(value > 20000 || value < 0){
        return 2;                                                              //Returning 2 when frequency below 0

    }else{
        this->req_message.setRequestValues(0, 0, (uint16_t)value);             // Described in Mini Pro part
        Serial.print(F("*****: ")); Serial.println((uint16_t)value);

        sendRequest(this->req_message);

        return 0;
    }
}

uint8_t RadioHandler::getSpeakerVolume(){                                      // Get Mini_pro Buzzer Tone value
    req_message.setRequestValues(1,0,0);
    return sendRequest(req_message);
}

uint8_t RadioHandler::getPotentiometerValue(){                                 // Get Mini_pro Potentiometer value
    req_message.setRequestValues(1,1,0);                                       // Described in Mini Pro part

    return sendRequest(req_message);
}

uint8_t RadioHandler::getStatistics(){                                         // Get Mini_pro Connection statistics
    req_message.setRequestValues(4,0,0);
    return sendRequest(req_message);
}

uint8_t RadioHandler::observeResource(int state){                              // Setting Mini_pro Observe() function enable/disable

    return sendRequest(req_message);
}
