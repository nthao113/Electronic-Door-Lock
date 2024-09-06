#ifndef RFID_RC522
#define RFID_RC522

#include <SPI.h>
#include <MFRC522.h>
#include "global_vars.h"

#define SS_PIN 5     
#define RST_PIN 4  

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void getcardID(){
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    Serial.println("No card");
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }

  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "93 05 1D 36") //change here the UID of the card/cards that you want to give access
  {
    Authorized_access(); 
  }
 
 else   {
    Access_denied();
  }
}

#endif