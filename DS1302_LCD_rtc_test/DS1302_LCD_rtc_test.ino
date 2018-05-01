// DS1302_LCD 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A quick demo of how to use my DS1302-library to make a quick
// clock using a DS1302 and a 16x2 LCD.
//
// I assume you know how to connect the DS1302 and LCD.
// DS1302:  CE pin    -> Arduino Digital 2
//          I/O pin   -> Arduino Digital 3
//          SCLK pin  -> Arduino Digital 4
// LCD:     DB7       -> Arduino Digital 6
//          DB6       -> Arduino Digital 7
//          DB5       -> Arduino Digital 8
//          DB4       -> Arduino Digital 9
//          E         -> Arduino Digital 10
//          RS        -> Arduino Digital 11

#include <LiquidCrystal.h>
#include <DS1302.h>

int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// Init the DS1302
DS1302 rtc(0, 2, 1);
//pinMode(buzzerPin, OUTPUT);
// Init the LCD
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int hour_alarm= 0, minutes_alarm = 0, buzzerPin = 3;
boolean alarm_trigger = false;

void setup()
{
  // Set the clock to run-mode, and disable the write protection
  rtc.halt(false);
  rtc.writeProtect(false);
  pinMode(44, INPUT_PULLUP);
  pinMode(45, INPUT_PULLUP);
  // Setup LCD to 16x2 characters
  lcd.begin(16, 2);

  // The following lines can be commented out to use the values already stored in the DS1302
  //rtc.setDOW(TUESDAY);        // Set Day-of-Week to FRIDAY
  //rtc.setTime(14, 29, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(24, 4, 2018);   // Set the date to August 6th, 2010
}

void loop()
{
  // Display time centered on the upper line
  lcd.setCursor(0, 1);
  lcd.print(rtc.getTimeStr());
  
  
  lcd.setCursor(9, 1);
  lcd.print(rtc.getDOWStr(FORMAT_SHORT));
  
  
  lcd.setCursor(5, 0);
  lcd.print(rtc.getDateStr());
  
  if(digitalRead(44) == LOW)
  {
    lcd.setCursor(13,0);
    lcd.print("set");
    while(read_LCD_buttons() != 4){
      set_alarm();
      }
     alarm_trigger=true;
    lcd.print("on");
    }
  if(digitalRead(45) == LOW){
    hour_alarm= 0, minutes_alarm = 0;
    lcd.clear();
    lcd.setCursor(11,0);
     alarm_trigger=false;
    lcd.print("reset");
    delay(3000);
    }
  
  String time_now = rtc.getTimeStr(); 
  delay (1000);
  if(time_now.substring(0,2).toInt() == hour_alarm && time_now.substring(3,5).toInt()== minutes_alarm && alarm_trigger == true && time_now.substring(6).toInt()<15)
  {
    buzz_buzz();
    buzz_buzz();
    buzz_buzz();
    }
  
}

void buzz_buzz()
{
  analogWrite(buzzerPin, 20);
  delay(5000);
  analogWrite(buzzerPin, 0); 
}

int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   

 return btnNONE;  // when all others fail, return this...
}

void set_alarm()
{
  
  if(read_LCD_buttons()==1)
      {
       hour_alarm=hour_alarm+1; 
        
        }
      if(read_LCD_buttons()==2)
      {
        hour_alarm= hour_alarm-1;
        
        }
        if(read_LCD_buttons()==0)
      {
       minutes_alarm= minutes_alarm+1; 
        
        }
      if(read_LCD_buttons()==3)
      {
        minutes_alarm=  minutes_alarm-1;
        
        }
        if(hour_alarm <0 )hour_alarm=23;
        if(hour_alarm>23)hour_alarm=0;
        if(minutes_alarm <0 )minutes_alarm=59;
        if(minutes_alarm>59)minutes_alarm=0;
        
        
        lcd.print("Set alarm: " + String(hour_alarm) + ":" + String(minutes_alarm) );
       
        lcd.setCursor(0, 1);
        lcd.print(rtc.getTimeStr());
         delay(500);
        lcd.clear();
  }
