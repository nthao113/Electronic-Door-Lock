#include "INM441.h"
#include "Camera.h"

void setup() {
  Serial.begin(115200);
  delay(500);

  ESP32_CAM_init();
  clear_i2s_driver();
  delay(500);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  client0.onEvent(onEventsCallback0);
  client1.onEvent(onEventsCallback1);  
  delay(500);

  xTaskCreatePinnedToCore(websocket_task_core0, "ws_core0", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(websocket_task_core1, "ws_core1", 2048, NULL, 1, NULL, 1);

}

void loop() {

}

// Task for core 0
void websocket_task_core0(void *pvParameters) {
  while(!client0.connect(websockets_server_host, websockets_server_port_0, "/")){
    delay(200);
    Serial.print(".0");
  }
  Serial.println("Socket Connected on port 8080!");  
  vTaskDelete(NULL);
}

// Task for core 1
void websocket_task_core1(void *pvParameters) {
  while(!client1.connect(websockets_server_host, websockets_server_port_1, "/")){
    delay(200);
    Serial.print(".1");
  }
  Serial.println("Socket Connected on port 8081!");  
  vTaskDelete(NULL);
}

void onEventsCallback0(WebsocketsEvent event, String data) {
  if(event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("Connnection Opened");
    xTaskCreatePinnedToCore(client_stream_video, "client_stream_video" , 1024*10 , NULL, 2, NULL, 0);
  } else if(event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("Connnection Closed");
    ESP.restart();
  }
}
void onEventsCallback1(WebsocketsEvent event, String data) {
  if(event == WebsocketsEvent::ConnectionOpened) {
    Serial.println("Connnection Opened");
    xTaskCreatePinnedToCore(client_stream_audio, "client_stream_audio", 1024*5 , NULL, 2, NULL, 1);
  } else if(event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("Connnection Closed");
    ESP.restart();
  }
}
