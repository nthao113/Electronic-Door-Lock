#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include "MAX98357A.h"

const char* ssid = "Be Kind Home";
const char* password = "tothon1%";

const char* websockets_server_host = "192.168.4.1";
const uint16_t websockets_server_port = 8080;

using namespace websockets;
WebsocketsClient client;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("Starting the wifi ...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  delay(500); 

  i2sInit_play();

  // Setup Callbacks
  // client.onMessage(onMessageCallback);
  client.onEvent(onEventsCallback);
  client.connect(websockets_server_host, websockets_server_port, "/");
  delay(500);

  Serial.print("end setup");
}

void loop() {
  client.poll();
  vTaskDelay(500);
}

void server_stream_audio(void *pvParameters) {

  while (true) {

    if(client.available()){
      client.poll();
      WebsocketsMessage msg1 = client.readBlocking();
      char *buf_ptr = (char*)msg1.c_str();
      int bytes_written = msg1.length();
      i2s_write_bytes(I2S_PORT, buf_ptr, bytes_written, portMAX_DELAY);
      
      Serial.println("Data written to I2S");
      vTaskDelay(50);
    }

  }

}

// void onMessageCallback(WebsocketsMessage message) {
//     if(message.data() == "start"){
//       // xTaskCreatePinnedToCore(client_stream_video, "client_stream_video" , 1024*10 , NULL, 1, NULL, 0);
//       Serial.print("Got Message: ");
//       Serial.println(message.data());    
//     }
// }

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
        xTaskCreatePinnedToCore(server_stream_audio, "server_stream_audio" , 1024*5 , NULL, 1, NULL, 1);
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
        ESP.restart();
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

