#include <BlynkSimpleStream.h>

char auth[] = "YzJUjIJYR9_JsI-mlhccFFuP900d0772";

// Khai báo các chân kết nối đèn, nút và cảm biến
int ledWifi = 11;
int ledNgoaiNha = 12;
int ledCauThang = 13;      
int btn1 = A1;
int btn2 = A2;
int btn3 = A3;
int pir = 2;      
int ldr = 3;

// Các biến chuyển chế độ
int flag;
int flag1;

// Khai báo đối tượng để sử dụng LCD và timer
WidgetLCD lcd(V0);
BlynkTimer timer;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, Serial);
  
  
  pinMode(ledCauThang, OUTPUT);
  pinMode(ledNgoaiNha, OUTPUT);
  pinMode(ledWifi, OUTPUT);
  pinMode(pir, INPUT);
  pinMode(ldr, INPUT_PULLUP);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(btn3, INPUT);
  lcd.clear();

  timer.setInterval(500L, sendUptime); 
}

//BLYNK_APP_CONNECTED() {
//  flag = 1;
//  Blynk.virtualWrite(V4, 1);
//  lcd.print(0, 0, "vl1");
//}
//
//BLYNK_APP_DISCONNECTED() {
//  flag = 0;
//  Blynk.virtualWrite(V4, 0);
//  lcd.print(0, 0, "vl2");
//}

BLYNK_WRITE(V1) 
{
  int pinData = param.asInt(); 
  digitalWrite(ledWifi, pinData);
}

BLYNK_WRITE(V2)
{
  int pinData = param.asInt(); 
  digitalWrite(ledNgoaiNha, pinData);
}

BLYNK_WRITE(V3) 
{
  int pinData = param.asInt(); 
  digitalWrite(ledCauThang, pinData);
}

BLYNK_WRITE(V4)
{
  int pinData = param.asInt(); 
  if(pinData == 1){
    flag = 1;
    lcd.clear();
    lcd.print(0, 0, "AUTO MODE");
  }else{
    flag = 0;
    lcd.clear();
    lcd.print(0, 0, "MANUAL MODE");
  }
}
BLYNK_WRITE(V5)
{
  int pinData = param.asInt(); 
  if(pinData == 1){
    flag1 = 1;
    lcd.clear();
    lcd.print(0, 0, "ONLINE MODE");
  }else{
    flag1 = 0;
    if(flag == 1){
      lcd.clear();
      lcd.print(0, 0, "AUTO MODE");
    }else{
      lcd.clear();
      lcd.print(0, 0, "MANUAL MODE");
    }
  }
}

void sendUptime()
{
  int pirStatus = digitalRead(pir);
  int ldrStatus = digitalRead(ldr);

  if(flag1 == 0){
    if(flag == 1){
      if(ldrStatus == HIGH){
        Blynk.virtualWrite(V2, 1);
        digitalWrite(ledNgoaiNha, HIGH); // LED On    
      }else{
        Blynk.virtualWrite(V2, 0);
        digitalWrite(ledNgoaiNha, LOW); // LED Off
      }
      
      if (pirStatus == HIGH)                
      {
        Blynk.virtualWrite(V3, 1);
        digitalWrite(ledCauThang, HIGH); // LED On
        delay(5000);
      }else{
        Blynk.virtualWrite(V3, 0);
        digitalWrite(ledCauThang, LOW); // LED Off
      }
    }
  
    if(flag == 0){
      if(digitalRead(btn1) == LOW){
        Blynk.virtualWrite(V1, 1);
        digitalWrite(ledWifi, HIGH); // LED On
      }else{
        Blynk.virtualWrite(V1, 0);
        digitalWrite(ledWifi, LOW); // LED Off
      }

      if(digitalRead(btn2) == LOW ){
        Blynk.virtualWrite(V2, 1);
        digitalWrite(ledNgoaiNha, HIGH); // LED On
      }else{
        Blynk.virtualWrite(V2, 0);
        digitalWrite(ledNgoaiNha, LOW); // LED Off
      }
      
      if (digitalRead(btn3) == LOW){
        Blynk.virtualWrite(V3, 1);
        digitalWrite(ledCauThang, HIGH); // LED On
      }else{
        Blynk.virtualWrite(V3, 0);
        digitalWrite(ledCauThang, LOW); // LED Off
      }
    }
  }
}
 
void loop()
{
  Blynk.run();
  timer.run();
}
