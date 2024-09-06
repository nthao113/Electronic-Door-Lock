#ifndef global_vars
#define global_vars

#define Buzzer 14 // tượng trưng cho ổ khóa_ổ khóa sẽ phát triển sau

// Khi mở khóa thành công
void Authorized_access(){
    Serial.println("Authorized access");
    digitalWrite(Buzzer, LOW);
    delay(200);
    digitalWrite(Buzzer, HIGH);
    Serial.println();
    delay(2000);
}

// Khi mở khóa thất bại
void Access_denied(){
    Serial.println("Access denied");
    digitalWrite(Buzzer, LOW);
    delay(100);
    digitalWrite(Buzzer, HIGH);
    delay(100);
    digitalWrite(Buzzer, LOW);
    delay(100);
    digitalWrite(Buzzer, HIGH);
    delay(2000);
}

#endif