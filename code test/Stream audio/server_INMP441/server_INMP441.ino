#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include "INM441.h"

const char* ssid = "Be Kind Home";
const char* password = "tothon1%";

TaskHandle_t i2sADCHandler = NULL;

using namespace websockets;
WebsocketsServer server;
WebsocketsClient client;

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

  // Khởi tạo WebSocket servers
  server.listen(8080); // Listen on port 8080
  client = server.accept();

  Serial.println("WebSocket servers started on ports 8080.");
  delay(500);

  xTaskCreatePinnedToCore(i2s_adc_task, "i2s_adc_task" , 1024*5 , NULL, 1, NULL, 1);

  Serial.print("end setup");
}

void loop (){
  client = server.accept();
  vTaskDelay(500);
}

// void sendtexttoclient(){

//   if(client.available()) {
//     return echo
//     client.send("start");
//     client.ping();
//     client.pong();

//   }

// }

void i2s_adc_task(void *pvParameters)
{
    i2sInit_record();

    int i2s_read_len = I2S_READ_LEN;
    size_t bytes_read;

    char* i2s_read_buff = (char*) calloc(i2s_read_len, sizeof(char));
    uint8_t* flash_write_buff = (uint8_t*) calloc(i2s_read_len, sizeof(char));

    while (1) {
      if(client.available()) {
        i2s_read(I2S_PORT, (void*) i2s_read_buff, i2s_read_len, &bytes_read, portMAX_DELAY);
        i2s_adc_data_scale(flash_write_buff, (uint8_t*)i2s_read_buff, i2s_read_len);
        
        client.sendBinary((const char*)flash_write_buff, i2s_read_len);
        ets_printf("Never Used Stack Size: %u\n", uxTaskGetStackHighWaterMark(NULL));
        
        vTaskDelay(50);
      }
    }
    
    free(i2s_read_buff);
    i2s_read_buff = NULL;
    free(flash_write_buff);
    flash_write_buff = NULL;

    
}


