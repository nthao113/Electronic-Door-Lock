#include "MPR121.h"
#include "R557.h"
#include "RFID_RC522.h"

void setup() 
{
  Serial.begin(115200); 
  while (!Serial) { 
    delay(10);
  }

  pinMode(lock, OUTPUT);
  digitalWrite(lock, LOW);

  Serial.println("Starting the wifi ...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  delay(500);  

// cài đặt kết nối MFID_RC522
  SPI.begin();     
  mfrc522.PCD_Init();  
  delay(500);  

// kiểm tra kết nối MPR121
  // Serial.println("MPR121 test!");
  // if (!cap.begin(0x5A)) {
  //   Serial.println("MPR121 not found, check wiring?");
  //   while (1);
  // }
  // Serial.println("MPR121 found!");
  // delay(500);

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
  finger.getParameters();
  finger.getTemplateCount();
 
  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
  Serial.println();
  delay(500);

  // Setup Callbacks
  Serial.println("Comecting to server... ");
  client_get_audio.onEvent(onEventsCallback);
  while(!client_get_audio.connect(websockets_server_host, websockets_server_port, "/")){
    delay(500);
    Serial.print(".");
  }
  delay(500);

  //Tasks
  xTaskCreatePinnedToCore(Unlock, "Unlock", 1024*5, NULL, 1, NULL, 0);
  delay(500);

  Serial.println("End of setup()\n\n");
}

void loop(){
  client_get_audio.poll();
  vTaskDelay(500);
}

void Unlock(void *pvParameters){
  while(1){
    getcardID();
    vTaskDelay(100);
    getFingerprintID();
    vTaskDelay(100);
    // getpasswordID();
    // vTaskDelay(100);
  }
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
        xTaskCreatePinnedToCore(Play_Server_Audio, "Play_Server_Audio", 1024*10, NULL, 1, NULL, 1);
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
        ESP.restart();
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
        streaming_audio = true;
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
        streaming_audio = false;
    }
}

