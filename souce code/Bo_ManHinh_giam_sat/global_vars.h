#ifndef global_vars_
#define global_vars_

#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <esp_task_wdt.h>
#include "driver/i2s.h"

const char* ssid = "Be Kind Home";
const char* password = "tothon1%";



using namespace websockets;
WebsocketsServer server_get_video;
WebsocketsClient client_get_video;

WebsocketsServer server_get_audio;
WebsocketsClient client_get_audio;

WebsocketsServer server_send_audio;
WebsocketsClient client_send_audio;

int ScreenDisplay = 0;  

bool streaming_video = false;
bool streaming_audio = false;



// Hàm gửi tin nhắn Ping cho client
void Request_send_to_clients(bool requestSent) {
  if(requestSent){
    if ( client_get_video.available() && client_send_audio.available()) {
      client_get_video.ping();
      client_send_audio.ping();
      delay(500);

      streaming_video = true;
      streaming_audio = true;
      Serial.println("Request sent Ping to clients for audio stream");
    return;

    } else {
      Serial.println("Not all clients are connected...");
    }
  }else {
    if ( client_get_video.available() && client_send_audio.available()) {
      client_get_video.pong();
      client_send_audio.pong();
      delay(500);

      streaming_video = false;
      streaming_audio = false;
      Serial.println("Request sent Pong to clients for audio stream");
      return;

    } else {
      Serial.println("Not all clients are connected");
    }
  }
}

//client_get_video.available() && client_send_audio.available()


#endif