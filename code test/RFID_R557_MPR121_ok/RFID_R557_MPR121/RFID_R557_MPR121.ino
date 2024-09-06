#include "MPR121.h"
#include "R557.h"
#include "RFID_RC522.h"

void setup() 
{
  Serial.begin(115200); 
  while (!Serial) { 
    delay(10);
  }

  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, HIGH);

// cài đặt kết nối MFID_RC522
  SPI.begin();     
  mfrc522.PCD_Init();  

// kiểm tra kết nối MPR121
  Serial.println("MPR121 test!");
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

// cài đặt và kiểm tra cảm biến vân tay điện dung 
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.println("\nAdafruit Fingerprint sensor enrollment");
  // Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  // Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  // Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  // Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  // Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  // Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  // Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  // Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);
 
  finger.getTemplateCount();
 
  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
      Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
  Serial.println();

}

void loop(){
  getcardID();
  delay(500);
  getFingerprintID();
  delay(500);
  getpasswordID();
  delay(500);
}

