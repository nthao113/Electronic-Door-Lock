#include "INM441.h"
#include "ESP32CAM.h"

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n");
  Serial.println("Begin of setup()\n");

  Serial.println("Starting the wifi ...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  delay(500);  

  timeClient.begin(); // used to start the NTP client
  devstr.toCharArray(devname, devstr.length());         // name of your camera for mDNS, Router, and filenames
  framebuffer = (uint8_t*)ps_malloc(1024 * 1024);       // buffer to store a jpg in motion // needs to be larger for big frames from ov5640 

  Serial.println("Mounting the SD card ...");
  esp_err_t card_err = init_sdcard();
  if (card_err != ESP_OK) {
    Serial.printf("SD Card init failed with error 0x%x", card_err);
    major_fail();
    return;
  }
  delay(500);  

  Serial.println("Setting up the camera ...");
  config_camera();  
  delay(500);  

  Serial.println("Checking SD for available space ...");
  delete_old_stuff(); 
  delay(500);   

  Serial.println("Comecting to server... ");
  client_send_video.onEvent(onEventsCallback0);
  while(!client_send_video.connect(websockets_server_host, websockets_server_port_0, "/")){
    delay(500);
    Serial.print(".");
  }
  delay(500); 

  // client_send_audio.onEvent(onEventsCallback1);  
  // while(!client_send_audio.connect(websockets_server_host, websockets_server_port_1, "/")){
  //   delay(500);
  //   Serial.print(".");
  // }  
  // delay(500); 

  Serial.println("Creating task");

  xTaskCreatePinnedToCore( the_camera_loop, "the_camera_loop", 3000, NULL, 1, &the_camera_loop_task, 0);   
  delay(1000);

  Serial.println("End of setup()\n\n");
}

void loop() {
  client_send_video.poll();
  vTaskDelay(500);
  // client_send_audio.poll();
  // vTaskDelay(500);
}


void onEventsCallback0(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened0");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed0");
        ESP.restart();
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
        streaming_video = true;
        streaming_audio = true;
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
        streaming_video = false;
        streaming_audio = false;
    }
}

// void onEventsCallback1(WebsocketsEvent event, String data) {
//     if(event == WebsocketsEvent::ConnectionOpened) {
//         Serial.println("Connnection Opened");
//         xTaskCreatePinnedToCore(Send_Audio_To_Server, "Send_Audio_To_Server", 1024*4 , NULL, 3, NULL, 1);
//     } else if(event == WebsocketsEvent::ConnectionClosed) {
//         Serial.println("Connnection Closed");
//         ESP.restart();
//     } else if(event == WebsocketsEvent::GotPing) {
//         // Serial.println("Got a Ping!");
//     } else if(event == WebsocketsEvent::GotPong) {
//         // Serial.println("Got a Pong!");
//     }
// }





