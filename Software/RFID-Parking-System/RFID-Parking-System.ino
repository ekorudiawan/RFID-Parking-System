#include <LiquidCrystal.h>
#include <Servo.h> 

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
Servo myservo; 

byte usr1[6]={0x58,0x00,0x6E,0xD6,0xD8};
byte usr2[6]={0x58,0x00,0x6E,0x76,0x53};

void setup() {
  Serial.begin(9600);   
  myservo.attach(9);
  myservo.write(0);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("-=Parkir  RFID=-");  
}

void loop () {
  byte i = 0;
  byte val = 0;
  byte code[6];
  byte checksum = 0;
  byte bytesread = 0;
  byte tempbyte = 0;
  
  lcd.setCursor(0, 0);
  lcd.print("-=Parkir  RFID=-"); 
  lcd.setCursor(0, 1);
  lcd.print("                ");  
  
  if(Serial.available() > 0) {
    if((val = Serial.read()) == 2) {                  
      bytesread = 0; 
      while (bytesread < 12) {                        
        if( Serial.available() > 0) { 
          val = Serial.read();
          if((val == 0x0D)||(val == 0x0A)||(val == 0x03)||(val == 0x02)) { 
            break;                                    
          }

          if ((val >= '0') && (val <= '9')) {
            val = val - '0';
          } else if ((val >= 'A') && (val <= 'F')) {
            val = 10 + val - 'A';
          }

          if (bytesread & 1 == 1) {
            code[bytesread >> 1] = (val | (tempbyte << 4));
            if (bytesread >> 1 != 5) {                
              checksum ^= code[bytesread >> 1];       
            };
          } else {
            tempbyte = val;                        
          };

          bytesread++;                                
        } 
      } 

      if (bytesread == 12) {      
        lcd.setCursor(0,0);    
        lcd.print("Code  ");
        for (i=0; i<5; i++) {
          if (code[i] < 16) { 
            lcd.print("0");
          }
          lcd.print(code[i], HEX);
        }

        boolean fail1 = false;
        boolean fail2 = false;
        // Cek User 1
        for(int i=0; i<5 ;i++) {
            if(code[i] != usr1[i]) {
              fail1 = true;
            } 
        }
        // Cek User 2
        for(int i=0; i<5 ;i++) {
            if(code[i] != usr2[i]) {
              fail2 = true;
            } 
        }
        
        if(fail1 == false || fail2 == false) {
            lcd.setCursor(0,1);
            lcd.print("Silahkan Keluar ");
            myservo.write(90);
            delay(2000);
            myservo.write(0);
        }
        else {
            lcd.setCursor(0,1);
            lcd.print("Akses Gagal     "); 
            delay(2000);
        }
        
      }

      bytesread = 0;
    }
  }
}
