#include <Wire.h>
#include <Adafruit_MPR121.h>

Adafruit_MPR121 cap = Adafruit_MPR121();

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("MPR121 test!");
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

}

char pass[7];               // Mảng char để lưu trữ pass nhập vào
int passIndex = 0;          // Chỉ số hiện tại trong mảng pass
bool enteringPass = false;  // Biến xác định xem đang nhập pass hay không

void loop() {

  // Kiểm tra xem đã có touch nào được nhấn hay không
  if (cap.touched()) {
    uint16_t touched = cap.touched(); // Đọc trạng thái của các cảm biến cảm ứng
    while(cap.touched()){};  //đợi cho tới khi nút đc thả ra

    if(touched == 0b0000100000000000){
      enteringPass = true;
      Serial.println("Start entering your password!!!");

      while(enteringPass == true){
        if (cap.touched()){
          uint16_t touched = cap.touched();
          while(cap.touched()){};

          // Xác định cảm biến cảm ứng nào đã được nhấn
          for (int i = 0; i < 12; i++) {
            if (touched & (1 << i)) {
              // Thêm giá trị của cảm biến vào pass
              switch (i) {
                case 8  : pass[passIndex] = '7'; break;
                case 4  : pass[passIndex] = '8'; break;
                case 0  : pass[passIndex] = '9'; break;
                case 9  : pass[passIndex] = '4'; break;
                case 5  : pass[passIndex] = '5'; break;
                case 1  : pass[passIndex] = '6'; break;
                case 10 : pass[passIndex] = '1'; break;
                case 6  : pass[passIndex] = '2'; break;
                case 2  : pass[passIndex] = '3'; break;
                case 7  : pass[passIndex] = '0'; break;
                case 11 : // Ký tự 'x' để bắt đầu lại
                          if (enteringPass) { // Nếu đang nhập pass
                            resetEnteredPass(); // Đặt lại pass
                            Serial.println("Start over...");
                          }
                          break;
                case 3  :  // Ký tự 'v' để kết thúc nhập pass
                          if (enteringPass) { // Nếu đang nhập pass
                            enteringPass = false; // Dừng việc nhập pass lại 
                            Serial.println("Enter pass...");
                          }
                          break;
                }
                  
                // Di chuyển sang phần tử tiếp theo trong pass
                passIndex++;
            }
          }
        }
        if (!enteringPass || passIndex == 6) { // Nếu không phải đang nhập pass hoặc nhập quá 6 ký tự
          passIndex = 0; // Đặt lại chỉ số để nhận ký tự tiếp theo
          enteringPass = false;
        }              
      }

      delay(500); // Đợi một chút trước khi tiếp tục
      Serial.println(pass);
      if (checkPass(pass)) {
        Serial.println("Pass entered correctly!");
      } else {
        Serial.println("Incorrect pass!");
      }
      resetEnteredPass();
    }
  }
}

bool checkPass(char pass[]) {
    // Kiểm tra pass
    if (pass[0] == '1' && pass[1] == '1' && pass[2] == '3' && pass[3] == '\0') {
        return true; // Pass đúng
    } else {
        return false; // Pass sai
    }
}

void resetEnteredPass() {
    for (int i = 0; i < 6; i++) {
        pass[i] = '\0'; // Đặt tất cả các phần tử của mảng về giá trị null
    }
    passIndex = 0; // Đặt lại chỉ số của mảng
}

