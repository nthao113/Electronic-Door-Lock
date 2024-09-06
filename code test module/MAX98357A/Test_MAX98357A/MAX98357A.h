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
  // i2s_set_pin(I2S_PORT, NULL);  
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

void playBeep(float frequency, int duration_ms, float volume) {

  i2sInit_play();

  // Step 2: Generate beep data
  int sampleCount = (I2S_SAMPLE_RATE * duration_ms) / 1000;
  uint16_t *samples = (uint16_t *)malloc(sampleCount * sizeof(uint16_t));
  int16_t amplitude = 32767 * volume; // Adjust amplitude based on volume (0.0 to 1.0)
  for (int i = 0; i < sampleCount; ++i) {
    samples[i] = (uint16_t)(sin(2.0f * M_PI * frequency * i / I2S_SAMPLE_RATE) * amplitude);
  }

  // Step 3: Write beep data to I2S
  size_t bytes_written = 0;
  i2s_write_bytes(I2S_PORT, (const char *)samples, sampleCount * sizeof(uint16_t), portMAX_DELAY);

  // Step 4: Cleanup
  free(samples);
  clear_i2s_driver();

}

void playDoorOpenSuccessSound(float volume) {

  playBeep(261.63, 200, volume); // C4
  playBeep(293.66, 200, volume); // D4
  playBeep(329.63, 200, volume); // E4
  playBeep(349.23, 200, volume); // F4
  playBeep(392.00, 200, volume); // G4

}


void playDoorOpenFailSound(float volume) {

  playBeep(392.00, 200, volume); // G4 note, 200 ms
  playBeep(329.63, 200, volume); // E4 note, 200 ms
  playBeep(261.63, 200, volume); // C4 note, 200 ms

}



#endif