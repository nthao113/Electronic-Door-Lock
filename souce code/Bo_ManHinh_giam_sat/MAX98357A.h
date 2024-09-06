#ifndef MAX98357A_
#define MAX98357A_

#include <Arduino.h>
#include "global_vars.h"

#define I2S_DOUT  22
#define I2S_BCLK  26
#define I2S_LRC   25

#define I2S_PORT_PLAY I2S_NUM_1
#define I2S_SAMPLE_RATE   (16000)
#define I2S_SAMPLE_BITS   (16)
#define UPDATE_INTERVAL   (500)


void i2sInit_play(){

  const i2s_config_t i2s_config_tx = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = I2S_SAMPLE_RATE,
    .bits_per_sample = i2s_bits_per_sample_t(I2S_SAMPLE_BITS),
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 16,
    .dma_buf_len = 64
  };

  const i2s_pin_config_t pin_config_tx = {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_LRC,
    .data_out_num = I2S_DOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(I2S_PORT_PLAY, &i2s_config_tx, 0, NULL);
  i2s_set_pin(I2S_PORT_PLAY, &pin_config_tx);

}

void Play_Client_Audio(void *pvParameters) {

  while (1) {
    if(streaming_audio == true){
      i2sInit_play();

      while (streaming_audio == true){
        
        if(client_get_audio.available()){
          client_get_audio.poll();
          WebsocketsMessage msg1 = client_get_audio.readBlocking();
          char *buf_ptr = (char*)msg1.c_str();
          int bytes_written = msg1.length();
          i2s_write_bytes(I2S_PORT_PLAY, buf_ptr, bytes_written, portMAX_DELAY);
          
          Serial.println("Data written to I2S");
        }
        vTaskDelay(50);
      }   

      i2s_driver_uninstall(I2S_PORT_PLAY);
      i2s_set_pin(I2S_PORT_PLAY, NULL);  
         
    }else{
      vTaskDelay(500);
    }
  }
}




#endif