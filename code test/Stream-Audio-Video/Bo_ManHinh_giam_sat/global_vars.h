#ifndef global_vars_
#define global_vars_

#include <ArduinoWebsockets.h>
#include <WiFi.h>

const char* ssid = "Be Kind Home";
const char* password = "tothon1%";

using namespace websockets;
WebsocketsServer server0;
WebsocketsServer server1;
WebsocketsClient client_core0;
WebsocketsClient client_core1;


#endif