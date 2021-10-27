//Định nghĩa Serial cho Blynk
#define BLYNK_PRINT Serial

//Include các thư viện
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <Arduino_FreeRTOS.h>

//Mã Auth Token 
//char auth[] = "YzJUjIJYR9_JsI-mlhccFFuP900d0772";
char auth[] = "lmRBRkoxenzg6XyUfE_rqsr8AMssyciz";
//Tên Wifi kết nối
char ssid[] = "Tran Thap";
//Mật khẩu
char pass[] = "";

//Định nghĩa BAUD RATE cho ESP8266
#define ESP8266_BAUD 115200
ESP8266 wifi(&Serial);

// Khai báo các chân kết nối đèn, nút và cảm biến
int ledWifi = 11;
int ledNgoaiNha = 12;
int ledCauThang = 13;      
int btn1 = A1;
int btn2 = A2;
int btn3 = A3;
int pir = 2;      
int ldr = A0;

// Các biến chuyển chế độ
int flag;
int flag1;
int delaypir = 5000;
int muctoi = 500;

// Khai báo đối tượng để sử dụng LCD và timer
WidgetLCD lcd(V0);

// Khai bao 3 task
TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;

void setup()
{
  // Debug console
  Serial.begin(9600);
  delay(10);
  // Set ESP8266 baud rate
  Serial.begin(ESP8266_BAUD);
  delay(10);
//  Blynk.begin(auth, wifi, ssid, pass);
  Blynk.begin(auth, wifi ,ssid, pass, "blynk-server.com", 8080);
 
  pinMode(ledCauThang, OUTPUT);
  pinMode(ledNgoaiNha, OUTPUT);
  pinMode(ledWifi, OUTPUT);
  pinMode(pir, INPUT);
  pinMode(ldr, INPUT_PULLUP);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(btn3, INPUT);
  lcd.clear();
  Blynk.virtualWrite(V6, delaypir/1000);
  Blynk.virtualWrite(V8, delaypir/1000);
  Blynk.virtualWrite(V9, muctoi);

  xTaskCreate(
    TaskLedNgoaiNha 
    ,  "LedNgoaiNha"   
    ,  128 
    ,  NULL
    ,  1  
    ,  &TaskHandle_1 );

  xTaskCreate(
    TaskLedBtn 
    ,  "TaskLedBtn"   
    ,  128  
    ,  NULL
    ,  1  
    ,  &TaskHandle_2 );
  
  xTaskCreate(
    TaskLedCauThang 
    ,  "LedCauThang"
    ,  128  
    ,  NULL
    ,  1  
    ,  &TaskHandle_3 );
}

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

BLYNK_WRITE(V6)
{
  int pinData = param.asInt(); 
  delaypir = 1000 * pinData;
  Blynk.virtualWrite(V8, pinData);
}

BLYNK_WRITE(V9)
{
  int pinData = param.asInt();
  muctoi = pinData;
}

BLYNK_WRITE(V7)
{
  int pinData = param.asInt(); 
  if(flag1==1){
    digitalWrite(ledWifi, pinData);
    digitalWrite(ledNgoaiNha, pinData);
    digitalWrite(ledCauThang, pinData);
    if(pinData == 1){
      Blynk.virtualWrite(V1, 1);
      Blynk.virtualWrite(V2, 1);
      Blynk.virtualWrite(V3, 1);
    }else{
      Blynk.virtualWrite(V1, 0);
      Blynk.virtualWrite(V2, 0);
      Blynk.virtualWrite(V3, 0);
    }
  }
}

void TaskLedNgoaiNha(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  while(1){
    Blynk.run();
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
      }
    }
  }
}

void TaskLedCauThang(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  while(1){
    Blynk.run();
    int pirStatus = digitalRead(pir);
  
    if(flag1 == 0){
      if(flag == 1){
        if (pirStatus == HIGH)                
        {
          Blynk.virtualWrite(V3, 1);
          digitalWrite(ledCauThang, HIGH); // LED On
          delay(delaypir);
        }else{
          Blynk.virtualWrite(V3, 0);
          digitalWrite(ledCauThang, LOW); // LED Off
        }
      }
    }
  }
}

void TaskLedBtn(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  while(1){
    Blynk.run();
    if(flag1 == 0){
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
}
 
void loop()
{
}
