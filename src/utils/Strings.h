#ifndef STRINGS_H
#define STRINGS_H

const char bad_version[] PROGMEM   = "Version not supported";
const char bad_header[] PROGMEM    = "Wrong format of coap header";
const char bad_token[] PROGMEM     = "Incorrect token";
const char unknown_error[] PROGMEM = "An unknown error has occured";
const char bad_option[] PROGMEM    = "Wrong format of coap option";

const char unsupported_content_format[] PROGMEM    = "Try plain text or JSON";
const char not_found[] PROGMEM                     = "Following Uri-Path not found";
const char method_not_allowed[] PROGMEM            = "Request type not supported at this Uri-Path ";

const char if_match_not_implemented[] PROGMEM      = "Option If-Match is not supported ";
const char uri_host_not_implemented[] PROGMEM      = "Option Uri-Host is not supported ";
const char if_none_match_not_implemented[] PROGMEM = "Option If-None-Match is not supported ";
const char observe_not_implemented[] PROGMEM       = "Option Observe is not supported :";
const char uri_port_not_implemented[] PROGMEM      = "Option Uri-Port is not supported ";
const char uri_query_not_implemented[] PROGMEM     = "Option Uri-Query is not supported ";
const char block2_not_implemented[] PROGMEM        = "Option Block2 is not supported ";
const char proxy_uri_not_supported[] PROGMEM       = "Proxying is not supported ";
const char proxy_scheme_not_supported[] PROGMEM    = "Proxying is not supported ";
const char block1_not_implemented[] PROGMEM        = "Option Block1 is not supported ";

const char speaker[] PROGMEM       = "speaker";
const char potentiometer[] PROGMEM = "potentiometer";
const char metrics[] PROGMEM       = "metrics";
const char well_known[] PROGMEM    = ".well-known";
const char core[] PROGMEM          = "core";
const char discover[] PROGMEM      = "</speaker>,\n</potentiometer>,\n</metrics> ";

// const char link_format[] PROGMEM               = "application/link-format";
const char link_format[] PROGMEM = "(";
const char json[] PROGMEM = "2";

const char get_speaker_ok_plain[] PROGMEM      = "Speaker frequency is: %u Hz.";
const char get_speaker_ok_json[] PROGMEM       = "{\"speaker\": %u} ";
const char get_speaker_fail[] PROGMEM          = "Failed to get a frequency ";
const char set_speaker_ok[] PROGMEM            = "Set speaker frequency to: %u Hz.";
const char set_speaker_invalid_input[] PROGMEM = "Please use values from range: 0 Hz - 20000 Hz.";
const char set_speaker_fail[] PROGMEM          = "Failed to set a frequency.";

const char get_potentiometer_ok_plain[] PROGMEM = "Potentiometer value is: %u .";
const char get_potentiometer_ok_json[] PROGMEM = "{\"potentiometer\": %u} ";
const char get_potentiometer_fail[] PROGMEM    = "Failed to get potentiometer's value ";
const char notification_potentiometer[] PROGMEM = "Potentiometer have been updated. New value: %u .";

const char get_metrics_ok_plain[] PROGMEM = "Packets received: %u; Packets sent: %u; Packet loss: %u%%. ";
const char get_metrics_ok_json[] PROGMEM = "{\"packetsReceived\": %u,\n\"packetsSent\": %u,\n\"packetLoss\": %u} ";
const char get_metrics_fail[] PROGMEM = "Failed to get metrics";

#endif //SRINGS_H
