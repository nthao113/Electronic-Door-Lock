#ifndef global_vars_
#define global_vars_


#include <WiFi.h>
#include <ArduinoWebsockets.h>

//
// WARNING!!! Make sure that you have either selected ESP32 Wrover Module,
//            or another board which has PSRAM enabled
//

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE
#define CAMERA_MODEL_AI_THINKER

const char* ssid = "Be Kind Home";
const char* password = "tothon1%";

const char* websockets_server_host = "192.168.4.1";
const uint16_t websockets_server_port_0 = 8080;
const uint16_t websockets_server_port_1 = 8081;

using namespace websockets;
WebsocketsClient client0;
WebsocketsClient client1;

#endif