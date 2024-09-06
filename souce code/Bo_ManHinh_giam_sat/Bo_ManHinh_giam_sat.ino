#include "LCD_TFT.h"
#include "INM441.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("Setting AP...");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP Address : ");
  Serial.println(IP);

  // Cấu hình lcd
  lcdtft_init();
  delay(500);

  // Khởi tạo WebSocket servers
  server_get_video.listen(8080); // Listen on port 8080
  // server_get_audio.listen(8081); // Listen on port 8081
  server_send_audio.listen(8082);
  delay(500);


  tft.fillScreen(TFT_BLACK);
  tft.setCursor(40, 110);
  tft.printf("WebSocket servers started on ports 8080 ...");  

  client_get_video = server_get_video.accept();
  // client_get_audio = server_get_audio.accept();

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(40, 110);
  tft.printf("WebSocket servers started on ports 8082 ...");  

  client_send_audio = server_send_audio.accept();
  delay(500);


  // Tasks
  xTaskCreatePinnedToCore(Screen_Display, "Screen_Display", 1024*10, NULL, 1, NULL, 0);
  // xTaskCreatePinnedToCore(Play_Client_Audio, "Play_Client_Audio", 1024*5, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(Send_Audio_To_Client, "Send_Audio_To_Client", 1024*5 , NULL, 1, NULL, 1);
  delay(500);

  Serial.print("end setup");
}

void loop() {
  client_get_video = server_get_video.accept();
  vTaskDelay(500);
  // client_get_audio = server_get_audio.accept();
  // vTaskDelay(500);
  client_send_audio = server_send_audio.accept();
  vTaskDelay(500);
}

void Screen_Display(void *pvParameters) {
  esp_task_wdt_init(60, false);
  (void) pvParameters;

  while(true){
    
    Serial.println("");
    Serial.print("ScreenDisplay : ");
    Serial.print(ScreenDisplay);

    switch(ScreenDisplay){
      case 0: Lock();     
              break;

      case 1: Display_Client_Video();
              break;

      case 2: break;
    }
  }
}




