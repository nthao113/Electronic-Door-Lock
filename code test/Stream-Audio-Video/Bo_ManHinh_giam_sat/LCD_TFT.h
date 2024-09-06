#ifndef LCD_TFT_
#define LCD_TFT_

#include <SPI.h>
#include <TJpg_Decoder.h>
#include <TFT_eSPI.h>
#include "global_vars.h"

TFT_eSPI tft = TFT_eSPI();         // Invoke custom library

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // This might work instead if you adapt the sketch to use the Adafruit_GFX library
  // tft.drawRGBBitmap(x, y, bitmap, w, h);

  // Return 1 to decode next block
  return 1;
}


void lcdtft_init_stream() {
  
  tft.begin();
  tft.setRotation(3);
  tft.setTextColor(0xFFFF, 0x0000);
  tft.fillScreen(TFT_RED);
  tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)

  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);
  
}

// void server_stream_video() {
//   if(server.poll()){
//     client = server.accept();
//   }

//   if(client.available()){
//     client.poll();

//     WebsocketsMessage msg = client.readBlocking();

//     uint32_t t = millis();

//     // Get the width and height in pixels of the jpeg if you wish
//     uint16_t w = 0, h = 0;
//     TJpgDec.getJpgSize(&w, &h, (const uint8_t*)msg.c_str(), msg.length());
//     Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);
    
//     // Draw the image, top left at 0,0
//     TJpgDec.drawJpg(0, 0, (const uint8_t*)msg.c_str(), msg.length());
  
//     // How much time did rendering take (ESP8266 80MHz 271ms, 160MHz 157ms, ESP32 SPI 120ms, 8bit parallel 105ms
//     t = millis() - t;
//     Serial.print(t); Serial.println(" ms");
//   }  
// }

// void server_stream_video(const uint8_t* video_data, size_t length) {
//   uint16_t w = 0, h = 0;
//   TJpgDec.getJpgSize(&w, &h, video_data, length);
//   Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);
//   TJpgDec.drawJpg(0, 0, video_data, length);
//   Serial.println("Video data processed and displayed");
// }




#endif
