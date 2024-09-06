#ifndef global_vars
#define global_vars

#include <ArduinoWebsockets.h>
#include <WiFi.h>

const char* ssid = "Be Kind Home";
const char* password = "tothon1%";

const char* websockets_server_host = "192.168.4.1";
const uint16_t websockets_server_port = 8082;

using namespace websockets;
WebsocketsClient  client_get_audio;


bool streaming_audio = false;

#define lock 14 // tượng trưng cho ổ khóa_ổ khóa sẽ phát triển sau


#include "MAX98357A.h"
// Khi mở khóa thành công
void Authorized_access(){
    Serial.println("Authorized access");

    digitalWrite(lock, HIGH);
    playDoorOpenSuccessSound(0.7);
    delay(2000);

    digitalWrite(lock, LOW);
    delay(200);
}

// Khi mở khóa thất bại
void Access_denied(){
    Serial.println("Access denied");

    playDoorOpenFailSound(0.7);
    
}

#endif