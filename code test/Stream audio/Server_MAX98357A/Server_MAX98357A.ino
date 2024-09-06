#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include "driver/i2s.h"

#define I2S_DOUT  22
#define I2S_BCLK  26
#define I2S_LRC   25

#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE   (16000)
#define I2S_SAMPLE_BITS   (16)
#define UPDATE_INTERVAL   (500)

const char* ssid = "Be Kind Home";
const char* password = "tothon1%";

using namespace websockets;
WebsocketsServer server;
WebsocketsClient client;

void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println("Setting AP...");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP Address : ");
  Serial.println(IP);

  server.listen(8888);
  
  i2sInit();
}

void loop() {
  if(server.poll()){
    client = server.accept();
    Serial.println("Client connected");
  }

  if(client.available()){
    client.poll();

    WebsocketsMessage msg = client.readBlocking();
    char *buf_ptr = (char*)msg.c_str();
    int bytes_written = msg.length();
    i2s_write_bytes(I2S_PORT, buf_ptr, bytes_written, portMAX_DELAY);
    
    Serial.println("Data written to I2S");
  }

}

void i2sInit(){
  const i2s_config_t i2s_config_tx = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = I2S_SAMPLE_RATE,
    .bits_per_sample = i2s_bits_per_sample_t(I2S_SAMPLE_BITS),
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 32,
    .dma_buf_len = 64
  };

  const i2s_pin_config_t pin_config_tx = {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_LRC,
    .data_out_num = I2S_DOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(I2S_PORT, &i2s_config_tx, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config_tx);

  Serial.println("I2S Initialized");
}
