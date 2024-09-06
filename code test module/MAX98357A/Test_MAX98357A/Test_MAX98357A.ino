/*
  Simple Internet Radio Demo
  esp32-i2s-simple-radio.ino
  Simple ESP32 I2S radio
  Uses MAX98357 I2S Amplifier Module
  Uses ESP32-audioI2S Library - https://github.com/schreibfaul1/ESP32-audioI2S
 
  DroneBot Workshop 2022
  https://dronebotworkshop.com
*/

// Include required libraries
#include "Arduino.h"
#include "WiFi.h"
#include "MAX98357A.h"


// Wifi Credentials
const char* ssid = "Be Kind Home";
const char* password = "tothon1%";

void setup() {

  // Start Serial Monitor
  Serial.begin(115200);

  // Setup WiFi in Station mode
  Serial.println("Starting the wifi ...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  delay(500);  

}
void loop() {
  // playBeep(1000.0f, 200, 0.5);
  playDoorOpenSuccessSound(0.7);
  Serial.println("thành công ...");
  delay(3000);
  playDoorOpenFailSound(0.7);
  Serial.println("thất bại ...");
  delay(3000);
}

