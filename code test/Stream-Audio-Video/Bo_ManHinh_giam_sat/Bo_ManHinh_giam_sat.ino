#include "MAX98357A.h"
#include "LCD_TFT.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);

  lcdtft_init_stream();
  i2sInit_play();
  delay(500);

  Serial.println();
  Serial.println("Setting AP...");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP Address : ");
  Serial.println(IP);

  server0.listen(8080); // Listen on port 8080
  server1.listen(8081); // Listen on port 8081
  Serial.println("WebSocket servers started on ports 8080 and 8081.");
  delay(500);

  xTaskCreatePinnedToCore(taskCore0, "TaskCore0", 1024*10, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskCore1, "TaskCore1", 1024*20, NULL, 1, NULL, 1);
  
}

void loop() {

}

void taskCore0(void *pvParameters) {
  while (true) {
    if (server0.poll()) {
      client_core0 = server0.accept();
      Serial.println("Client connected to server on port 8080");
    }
    if(client_core0.available()){
      client_core0.poll();

      WebsocketsMessage msg0 = client_core0.readBlocking();

      // Get the width and height in pixels of the jpeg if you wish
      uint16_t w = 0, h = 0;
      TJpgDec.getJpgSize(&w, &h, (const uint8_t*)msg0.c_str(), msg0.length());
      
      // Draw the image, top left at 0,0
      TJpgDec.drawJpg(0, 0, (const uint8_t*)msg0.c_str(), msg0.length());

    }     
    delay(10);
  }
}

void taskCore1(void *pvParameters) {
  while (true) {
    if (server1.poll()) {
      client_core1 = server1.accept();
      Serial.println("Client connected to server on port 8081");
    } 
    if(client_core1.available()){
      client_core1.poll();
      WebsocketsMessage msg1 = client_core1.readBlocking();
      char *buf_ptr = (char*)msg1.c_str();
      int bytes_written = msg1.length();
      i2s_write_bytes(I2S_PORT, buf_ptr, bytes_written, portMAX_DELAY);
      
      // Serial.println("Data written to I2S");
    }
    delay(10);
  }
}
