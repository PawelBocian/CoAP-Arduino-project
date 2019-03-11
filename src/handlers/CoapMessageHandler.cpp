//
// Created by Wojtek on 09.01.2019.
//

#include "CoapMessageHandler.h"

void CoapMessageHandler::handleMessage(uint8_t *packetBuffer, uint16_t packetSize, EthernetUDP udp){

    //sets this static value so we do not use content formats from previous calls
    this->contentFormat = PLAIN;

    //holds enough space for every payload
    char stringBuffer[STRING_BUFFER] = "";

    int8_t parsingResult = 0;

    parsingResult = this->parsedMessage.parse(packetBuffer, packetSize);

    boolean potentiometerAckFlag = false;

    //used to handle malformed requests and diagnose an issue
    switch(parsingResult){
        case -11:
        this->assembledMessage.assemble(packetBuffer, 1, this->assembledMessage.formatResponseCode(BAD_REQUEST), bad_version);
        goto end;

        case -12:
        this->assembledMessage.assemble(packetBuffer, 1, this->assembledMessage.formatResponseCode(BAD_REQUEST), bad_header);
        goto end;

        case -21:
        this->assembledMessage.assemble(packetBuffer, 1, this->assembledMessage.formatResponseCode(BAD_REQUEST), bad_token);
        goto end;

        case -31:
        this->assembledMessage.assemble(packetBuffer, 1, this->assembledMessage.formatResponseCode(INTERNAL_SERVER_ERROR), unknown_error);
        goto end;

        case -42:
        this->assembledMessage.assemble(packetBuffer, 1, this->assembledMessage.formatResponseCode(BAD_REQUEST), bad_option);
        goto end;

        default:
        break;
    }


    //sets corresponding message ID and token
    this->assembledMessage.setHeader(this->parsedMessage.getHeader());
    this->assembledMessage.setToken(this->parsedMessage.getToken());
    
    //handling coap ping
    if(this->assembledMessage.getHeader().getType() == CON && this->assembledMessage.getHeader().getCode() == EMPTY && packetSize == 4){

      this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, ACK, EMPTY, "");
      goto end;
    }

    Option option;

    for(uint8_t i = 0; i < this->parsedMessage.getOptionCounter(); i++){

        option = this->parsedMessage.getOptions()[i];

        //checking for unsupported or not implemented critical functionalities
        switch(option.getNumber()){
            case IF_MATCH:

                sprintf_P(stringBuffer, if_match_not_implemented);

                this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(501), stringBuffer);
                goto end;

            case URI_HOST:

                sprintf_P(stringBuffer, uri_host_not_implemented);

                this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(501), stringBuffer);
                goto end;

            case IF_NONE_MATCH:

                sprintf_P(stringBuffer, if_none_match_not_implemented);

                this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(501), stringBuffer);
                goto end;

            case OBSERVE:

                sprintf_P(stringBuffer, observe_not_implemented);

                this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(501), stringBuffer);
                goto end;

            case URI_PORT:

                sprintf_P(stringBuffer, uri_port_not_implemented);

                this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(501), stringBuffer);
                goto end;

            case URI_QUERY:

                sprintf_P(stringBuffer, uri_query_not_implemented);

                this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(501), stringBuffer);
                goto end;

            case PROXY_URI:

                sprintf_P(stringBuffer, proxy_uri_not_supported);

                this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(505), stringBuffer);
                goto end;

            case PROXY_SCHEME:

                sprintf_P(stringBuffer, proxy_scheme_not_supported);

                this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(PROXYING_NOT_SUPPORTED), stringBuffer);
                goto end;

            case BLOCK2:

                sprintf_P(stringBuffer, block2_not_implemented);

                this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(501), stringBuffer);
                goto end;

            case BLOCK1:

                sprintf_P(stringBuffer, block1_not_implemented);

                this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(NOT_IMPLEMENTED), stringBuffer);
                goto end;

            case ACCEPT:
                //we can create responses in JSON and plain format
                if(option.compareValue(json)){
                    this->contentFormat = JSON;
                    break;

                //we cannot create them in other formats
                }else{

                    sprintf_P(stringBuffer, unsupported_content_format);

                    this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(UNSUPPORTED_CONTENT_FORMAT), stringBuffer);
                    goto end;
                }

            case CONTENT_FORMAT:
                if(option.compareValue(json)){
                    break;

                }else{

                    sprintf_P(stringBuffer, unsupported_content_format);

                    this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(UNSUPPORTED_CONTENT_FORMAT), stringBuffer);
                    goto end;
                }

            default:
                break;
        }
    }

    //this giant 'for' loop will handle all the scenarios we predicted that would happen
    for(uint8_t i = 0; i < this->parsedMessage.getOptionCounter(); i++){

        option = this->parsedMessage.getOptions()[i];

        switch(option.getNumber()){
            case URI_PATH:

                //#####SPEAKER#####
                if(option.compareValue(speaker)){

                    //#####GET######
                    if(this->parsedMessage.getHeader().getCode() == GET){

                        //####NON####
                        if(this->parsedMessage.getHeader().getType() == NON){

                              this->radioHandler.getSpeakerVolume();

                              uint16_t volume = waitForResponse();

                              //####FAIL####
                              if(volume == 20001){
                                  sprintf_P(stringBuffer, get_speaker_fail);

                                  this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(INTERNAL_SERVER_ERROR), stringBuffer);

                                  this->clearStringBuffer(stringBuffer);

                                  goto end;

                              //####SUCCESS####
                              }else{
                                  if(this->contentFormat == JSON){
                                      sprintf_P(stringBuffer, get_speaker_ok_json, volume);
                                      this->assembledMessage.addOption(CONTENT_FORMAT, json);
                                  }else{
                                      sprintf_P(stringBuffer, get_speaker_ok_plain, volume);
                                  }

                                  this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(CONTENT), stringBuffer);

                                  this->clearStringBuffer(stringBuffer);

                                  goto end;
                              }

                        //####CON####
                        }else if(this->parsedMessage.getHeader().getType() == CON){

                            this->radioHandler.getSpeakerVolume();

                            uint16_t volume = waitForResponse();

                            //####FAIL####
                            if(volume == 20001){
                                sprintf_P(stringBuffer, get_speaker_fail);

                                this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, ACK, this->assembledMessage.formatResponseCode(INTERNAL_SERVER_ERROR), stringBuffer);

                                this->clearStringBuffer(stringBuffer);

                                goto end;

                            //####SUCCESS####
                            }else{
                                if(this->contentFormat == JSON){

                                    sprintf_P(stringBuffer, get_speaker_ok_json, volume);
                                    this->assembledMessage.addOption(CONTENT_FORMAT, json);

                                }else{
                                    sprintf_P(stringBuffer, get_speaker_ok_plain, volume);
                                }

                                this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, ACK, this->assembledMessage.formatResponseCode(CONTENT), stringBuffer);

                                this->clearStringBuffer(stringBuffer);

                                goto end;
                            }

                        }else{
                            goto end;
                        }

                    //####PUT####
                    }else if(this->parsedMessage.getHeader().getCode() == PUT){

                        //####NON####
                        if(this->parsedMessage.getHeader().getType() == NON){

                            uint16_t setValue;

                            setValue = atoi(this->parsedMessage.getPayload());

                            uint8_t speakerResponse = this->radioHandler.setSpeakerVolume(setValue);

                            //####SUCCESS####
                            if(speakerResponse == 0){

                                    sprintf_P(stringBuffer, set_speaker_ok, setValue);

                                    this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(CONTENT), stringBuffer);

                                    this->clearStringBuffer(stringBuffer);

                                    goto end;

                            //####FAIL####
                            }else if(speakerResponse == 1){
                                    sprintf_P(stringBuffer, set_speaker_fail);

                                    this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(INTERNAL_SERVER_ERROR), stringBuffer);

                                    this->clearStringBuffer(stringBuffer);

                                    goto end;

                            //####VALUE NOT IN RANGE####
                            }else if(speakerResponse == 2){
                                    sprintf_P(stringBuffer, set_speaker_invalid_input);

                                    this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(BAD_REQUEST), stringBuffer);

                                    this->clearStringBuffer(stringBuffer);

                                    goto end;
                            }

                        //####CON####
                        }else if(this->parsedMessage.getHeader().getType() == CON){

                            uint16_t setValue;

                            setValue = atoi(this->parsedMessage.getPayload());

                            uint8_t speakerResponse = this->radioHandler.setSpeakerVolume(setValue);

                            //####SUCCESS####
                            if(speakerResponse == 0){

                                    sprintf_P(stringBuffer, set_speaker_ok, setValue);

                                    this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, ACK, this->assembledMessage.formatResponseCode(CONTENT), stringBuffer);

                                    this->clearStringBuffer(stringBuffer);

                                    goto end;

                            //####FAIL####
                            }else if(speakerResponse == 1){
                                    sprintf_P(stringBuffer, set_speaker_fail);

                                    this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, ACK, this->assembledMessage.formatResponseCode(INTERNAL_SERVER_ERROR), stringBuffer);

                                    this->clearStringBuffer(stringBuffer);

                                    goto end;

                            //####VALUE NOT IN RANGE####
                            }else if(speakerResponse == 2){

                                    sprintf_P(stringBuffer, set_speaker_invalid_input);

                                    this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, ACK, this->assembledMessage.formatResponseCode(BAD_REQUEST), stringBuffer);

                                    this->clearStringBuffer(stringBuffer);

                                    goto end;
                            }
                        }

                    //
                    }else{
                        sprintf_P(stringBuffer, method_not_allowed);

                        this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(METHOD_NOT_ALLOWED), stringBuffer);

                        this->clearStringBuffer(stringBuffer);

                        goto end;
                    }

                //######POTENTIOMETER######
                }else if(option.compareValue(potentiometer)){

                    //####GET####
                    if(this->parsedMessage.getHeader().getCode() == GET){
                        //####NON####
                        if(this->parsedMessage.getHeader().getType() == NON){

                            this->radioHandler.getPotentiometerValue();

                            uint16_t potValue = waitForResponse();

                            if(potValue == 20001){
                                sprintf_P(stringBuffer, get_potentiometer_fail);

                                this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(INTERNAL_SERVER_ERROR), stringBuffer);

                                this->clearStringBuffer(stringBuffer);

                                goto end;

                            }else{

                                if(this->contentFormat == JSON){
                                    sprintf_P(stringBuffer, get_potentiometer_ok_json, potValue);
                                    this->assembledMessage.addOption(CONTENT_FORMAT, json);
                                }else{
                                    sprintf_P(stringBuffer, get_potentiometer_ok_plain, potValue);
                                }

                                this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(CONTENT), stringBuffer);

                                this->clearStringBuffer(stringBuffer);

                                goto end;
                            }

                        //####CON####
                        }else if(this->parsedMessage.getHeader().getType() == CON){

                            this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, ACK, 0, EMPTY, NULL, "");

                            this->clearStringBuffer(stringBuffer);

                            //sets a flag in order to get back to this point after sending ACK,
                            // in order to assemble a proper response
                            potentiometerAckFlag = true;
                            goto end;

                            potentiometerSendNon:

                            this->radioHandler.getPotentiometerValue();

                            uint16_t potValue = waitForResponse();

                            //####FAIL####
                            if(potValue == 20001){
                                sprintf_P(stringBuffer, get_potentiometer_fail);

                                this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(INTERNAL_SERVER_ERROR), stringBuffer);

                                this->clearStringBuffer(stringBuffer);

                                goto end;

                            //####SUCCESS
                            }else{
                              if(this->contentFormat == JSON){
                                  sprintf_P(stringBuffer, get_potentiometer_ok_json, potValue);
                                  this->assembledMessage.addOption(CONTENT_FORMAT, json);
                              }else{
                                  sprintf_P(stringBuffer, get_potentiometer_ok_plain, potValue);
                              }

                              this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(CONTENT), stringBuffer);

                              this->clearStringBuffer(stringBuffer);

                              goto end;
                            }
                        }

                    //####OTHER THAN GET####
                    }else{
                        sprintf_P(stringBuffer, method_not_allowed);

                        this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(METHOD_NOT_ALLOWED), stringBuffer);

                        this->clearStringBuffer(stringBuffer);

                        goto end;
                    }

                //#####METRICS#####
                }else if(option.compareValue(metrics)){

                    //####GET####
                    if(this->parsedMessage.getHeader().getCode() == GET){

                        //####NON####
                        if(this->parsedMessage.getHeader().getType() == NON){

                            this->radioHandler.getStatistics();

                            Stats stats = waitForStatistics();

                            if(stats.packagesReceived == 0){
                                sprintf_P(stringBuffer, get_metrics_fail);

                                this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(INTERNAL_SERVER_ERROR), stringBuffer);

                                this->clearStringBuffer(stringBuffer);

                                goto end;

                            }else{
                              if(this->contentFormat == JSON){
                                  sprintf_P(stringBuffer, get_metrics_ok_json, stats.packagesReceived, stats.messagesSend, stats.sendingComplete);
                                  this->assembledMessage.addOption(CONTENT_FORMAT, json);
                              }else{
                                  sprintf_P(stringBuffer, get_metrics_ok_plain, stats.packagesReceived, stats.messagesSend, stats.sendingComplete);
                              }

                              this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(CONTENT), stringBuffer);

                              this->clearStringBuffer(stringBuffer);

                              goto end;
                          }

                      //####CON####
                      }else if(this->parsedMessage.getHeader().getType() == CON){

                          this->radioHandler.getStatistics();

                          Stats stats = waitForStatistics();

                          if(stats.packagesReceived == 0){
                              sprintf_P(stringBuffer, get_metrics_fail);

                              this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, ACK, this->assembledMessage.formatResponseCode(INTERNAL_SERVER_ERROR), stringBuffer);

                              this->clearStringBuffer(stringBuffer);

                              goto end;

                          }else{

                            if(this->contentFormat == JSON){
                                sprintf_P(stringBuffer, get_metrics_ok_json, stats.packagesReceived, stats.messagesSend, stats.sendingComplete);
                                this->assembledMessage.addOption(CONTENT_FORMAT, json);
                            }else{
                                sprintf_P(stringBuffer, get_metrics_ok_plain, stats.packagesReceived, stats.messagesSend, stats.sendingComplete);
                            }

                            this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, ACK, this->assembledMessage.formatResponseCode(CONTENT), stringBuffer);

                            this->clearStringBuffer(stringBuffer);

                            goto end;
                            }
                        }

                    //####ELSE THAN GET####
                    }else{

                        sprintf_P(stringBuffer, method_not_allowed);

                        this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(METHOD_NOT_ALLOWED), stringBuffer);

                        this->clearStringBuffer(stringBuffer);

                        goto end;
                    }

                }else if(option.compareValue(well_known) && this->parsedMessage.getOptions()[i+1].compareValue(core)){

                    sprintf_P(stringBuffer, discover);

                    this->assembledMessage.addOption(CONTENT_FORMAT, link_format);
                    this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(CONTENT), stringBuffer);

                    this->clearStringBuffer(stringBuffer);

                    goto end;

                }else{

                    sprintf_P(stringBuffer, not_found);

                    this->assembledPacketSize = this->assembledMessage.assemble(packetBuffer, NON, this->assembledMessage.formatResponseCode(NOT_FOUND), not_found);

                    this->clearStringBuffer(stringBuffer);

                    goto end;
                }
        }
    }

    //clean up and send
    end:
    uint16_t packSize = this->assembledPacketSize;
    this->assembledPacketSize = 0;

    if(packSize){

         Serial.print(F("remote IP: ")); Serial.println(udp.remoteIP());

         udp.beginPacket(udp.remoteIP(), udp.remotePort());

         Serial.println(udp.write(packetBuffer, packSize));
         udp.endPacket();

         if(potentiometerAckFlag){
            potentiometerAckFlag = false;
            goto potentiometerSendNon;
         }
    }

    this->parsedMessage.freeMemory(parsingResult);
}

uint16_t CoapMessageHandler::waitForResponse(){

    uint32_t currentTime = millis();

    while(true){
         this->radioHandler.network.update();

         if(this->radioHandler.network.available()){
              this->radioHandler.network.read(this->radioHandler.uno_header, &this->radioHandler.res_message, sizeof(this->radioHandler.res_message));

                  uint16_t value = this->radioHandler.res_message.value;
                  return value;
          }

        if(millis() - currentTime > 500){
            //returns 'timed off'
            return 20001;
        }
    }
}

Stats CoapMessageHandler::waitForStatistics(){
    uint32_t currentTime = millis();

    while(true){
        this->radioHandler.network.update();

        if(this->radioHandler.network.available()){
            this->radioHandler.network.read(this->radioHandler.uno_header, &this->radioHandler.res_message, sizeof(this->radioHandler.res_message));
                Stats stats;
                stats.messagesSend = radioHandler.res_message.messages;
                stats.packagesReceived = radioHandler.res_message.result;
                stats.sendingComplete = radioHandler.res_message.value;

                return stats;
        }

        if(millis() - currentTime > 500){

            Stats stats;
            stats.packagesReceived = 0;
            return stats;
        }
    }
}

void CoapMessageHandler::clearStringBuffer(char *stringBuffer){

    //reset the string buffer
    for(uint8_t i = 0; i < STRING_BUFFER; i++){
        stringBuffer[i] = 0;
    }
}
