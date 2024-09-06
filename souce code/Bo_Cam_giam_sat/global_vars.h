#ifndef global_vars_
#define global_vars_

#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <esp_task_wdt.h>

TaskHandle_t the_camera_loop_task;
TaskHandle_t the_sd_loop_task;

const char* ssid = "Be Kind Home";
const char* password = "tothon1%";

const char* websockets_server_host = "192.168.4.1";
const uint16_t websockets_server_port_0 = 8080;
const uint16_t websockets_server_port_1 = 8081;

using namespace websockets;
WebsocketsClient client_send_video;
WebsocketsClient client_send_audio;

bool streaming_video = false;
bool streaming_audio = false;


#endif