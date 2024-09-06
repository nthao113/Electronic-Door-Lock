#ifndef INM441_
#define INM441_

#include "global_vars.h"

#define I2S_WS  27
#define I2S_SCK 32
#define I2S_SD  33

#define I2S_PORT_RECORD I2S_NUM_1
#define I2S_SAMPLE_RATE   (16000)
#define I2S_SAMPLE_BITS   (16)
#define I2S_READ_LEN      (1024)


void i2sInit_record(){

  const i2s_config_t i2s_config_rx = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = I2S_SAMPLE_RATE,
    .bits_per_sample = i2s_bits_per_sample_t(I2S_SAMPLE_BITS),
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // default interrupt priority
    .dma_buf_count = 16,
    .dma_buf_len = 64
  };

  const i2s_pin_config_t pin_config_rx = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_PORT_RECORD, &i2s_config_rx, 0, NULL);
  i2s_set_pin(I2S_PORT_RECORD, &pin_config_rx);

}

void i2s_adc_data_scale(uint8_t * d_buff, uint8_t* s_buff, uint32_t len)
{
  uint32_t j = 0;
  uint32_t dac_value = 0;
  for (int i = 0; i < len; i += 2) {
    dac_value = ((((uint16_t) (s_buff[i + 1] & 0xf) << 8) | ((s_buff[i + 0]))));
    d_buff[j++] = 0;
    d_buff[j++] = dac_value * 256 / 2048 ;
  }
}

void Send_Audio_To_Client (void *pvParameters) {
  while(1){

    if(streaming_audio == true){      
      i2sInit_record();

      int i2s_read_len = I2S_READ_LEN;
      size_t bytes_read;

      char* i2s_read_buff = (char*) calloc(i2s_read_len, sizeof(char));
      uint8_t* flash_write_buff = (uint8_t*) calloc(i2s_read_len, sizeof(char));
      
      while (streaming_audio == true) {
        i2s_read(I2S_PORT_RECORD, (void*) i2s_read_buff, i2s_read_len, &bytes_read, portMAX_DELAY);
        i2s_adc_data_scale(flash_write_buff, (uint8_t*)i2s_read_buff, i2s_read_len);

        client_send_audio.sendBinary((const char*)flash_write_buff, i2s_read_len);
        ets_printf("Never Used Stack Size: %u\n", uxTaskGetStackHighWaterMark(NULL));

        vTaskDelay(50);
      }
      
      free(i2s_read_buff);
      i2s_read_buff = NULL;
      free(flash_write_buff);
      flash_write_buff = NULL;

      i2s_driver_uninstall(I2S_PORT_RECORD);
      i2s_set_pin(I2S_PORT_RECORD, NULL);  

    }else{
      vTaskDelay(500);
    }
  }
}



#endif
