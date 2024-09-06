#ifndef LCD_TFT_
#define LCD_TFT_

#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>

#include "display.h"
#include "global_vars.h"
#include "MAX98357A.h"

#define CALIBRATION_FILE "/TouchCalData3"
#define BLACK_SPOT
#define REPEAT_CAL false

bool LockStatus = true;                                                    
static uint16_t x = 0, y = 0;
bool pressed;

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


void lcdtft_init() {
  
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(2);

  tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)

  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(2);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);
  
}

void Lock()
{
  Serial.println("");
  Serial.print("LockStatus : ");
  Serial.print(LockStatus);
  if(LockStatus == 1){
    tft.pushImage(0, 0, 320, 240, display);
  }else {
    tft.pushImage(0, 0, 320, 240, displayLOCK);
  }
  delay(500);

  while(ScreenDisplay == 0){   
    pressed = tft.getTouch(&x, &y);
    if (pressed && x > 10 && x < 150 && y > 10 && y < 115) {
      LockStatus = !LockStatus ;
      Serial.println(LockStatus);
      break;
    }
    if (pressed && x > 10 && x < 150 && y > 125 && y < 200) {
      ScreenDisplay = 1;
      break;
    }
    vTaskDelay(100);
  }

}

void Display_Client_Video() {

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(40, 110);
  tft.printf("waiting for connection ...");  

  while (streaming_video != true){
    Request_send_to_clients(true);

    pressed = tft.getTouch(&x, &y);
    if (pressed && x > 0 && x < 60 && y > 180 && y < 240) {
      Request_send_to_clients(false);
      ScreenDisplay = 0;
      break;
    }
    vTaskDelay(500);
  }
  

  while (streaming_video == true) {

    if(client_get_video.available()){
      client_get_video.poll();

      WebsocketsMessage msg0 = client_get_video.readBlocking();

      // Get the width and height in pixels of the jpeg if you wish
      uint16_t w = 0, h = 0;
      TJpgDec.getJpgSize(&w, &h, (const uint8_t*)msg0.c_str(), msg0.length());
      
      // Draw the image, top left at 0,0
      TJpgDec.drawJpg(0, 0, (const uint8_t*)msg0.c_str(), msg0.length());
    }   

    pressed = tft.getTouch(&x, &y);
    if (pressed && x > 0 && x < 60 && y > 180 && y < 240) {
      Request_send_to_clients(false);
      ScreenDisplay = 0;
      break;
    } 
    vTaskDelay(50);
  } 

}





#endif
