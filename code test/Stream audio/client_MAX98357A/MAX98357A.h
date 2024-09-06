#ifndef MAX98357A_
#define MAX98357A_

#include <Arduino.h>
#include "driver/i2s.h"

#define I2S_DOUT  25
#define I2S_BCLK  26
#define I2S_LRC   27

#define I2S_PORT I2S_NUM_1
#define I2S_SAMPLE_RATE   (16000)
#define I2S_SAMPLE_BITS   (16)
#define UPDATE_INTERVAL   (500)

void clear_i2s_driver(){
  //we need to clear the driver 
  i2s_driver_uninstall(I2S_PORT);
  //i2s_set_pin(I2S_PORT, NULL);  
}

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

  i2s_driver_install(I2S_PORT, &i2s_config_tx, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config_tx);

}






#endif