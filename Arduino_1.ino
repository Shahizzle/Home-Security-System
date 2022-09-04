#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define Password_Length 8
#define POTENTIOMETER_PIN A0

int signalPin = 12;

char Data[Password_Length];
char Master[Password_Length] = "123A456";
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;

const byte ROWS = 4;
const byte COLS = 4;

int red_light_pin = 10;
int green_light_pin = 11;
int blue_light_pin = 12;

int police = 0;
int armed;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(signalPin, OUTPUT);
}

void loop() {
  if (Serial.available())
    {
    //Serial.println("serial is available");
    String sentInfo = Serial.readStringUntil('\r\n');
    int cmd = sentInfo.toInt();
    armed = map((analogRead(POTENTIOMETER_PIN)), 0, 1023, 1, 0);

    if (armed == 0 && cmd == 0) {
      Serial.println("Offline");
      lcd.setCursor(0, 0);
      lcd.print("System Offline");
      delay(1000);
    }
    else if (armed == 1 && cmd == 0) 
    {
        Serial.println("Armed");
        lcd.setCursor(0, 0);
        lcd.print("System Armed");
        delay(1000);
        
     }
    else if (armed == 1 && cmd == 1)
      {
        Serial.println("Password");
        lcd.clear();
        while (true)
        {
        lcd.setCursor(0, 0);
        lcd.print("Enter Password:"); 
 
          Data[data_count] = customKeypad.waitForKey();
          lcd.setCursor(data_count, 1);
          lcd.print(Data[data_count]);
          data_count++;
        
 
        if (data_count == Password_Length - 1) {
          lcd.clear();
        
          if (!strcmp(Data, Master)) {
            lcd.print("Correct");
            RGB_color(0, 255, 0); // Green
            delay(5000);
            RGB_color(0, 0, 0);

            police = 0;
            break; //exit while loop
          }
          else {
            lcd.print("Incorrect");
            delay(1000);
            lcd.clear();
            RGB_color(255, 0, 0); // Red
            delay(1000);
            RGB_color(0, 0, 0);
            police++;
            if (police % 3 == 1)
            {
              lcd.print("2 Attempts Left");
              delay(1000);
              lcd.clear();
              customKey = "";
              data_count = 0;
            }
            else if (police % 3 == 2)
            {
              lcd.print("1 Attempts Left");
              delay(1000);
              lcd.clear();
              customKey = "";
              data_count = 0;
            }
          }
          //police code
          Serial.println(police);
          if (police == 3)
          {
            lcd.print("Calling Police");

            RGB_color(255, 0, 0); // Red
            delay(500);
            RGB_color(0, 0, 255); // Blue
            delay(500);
            RGB_color(255, 0, 0); // Red
            delay(500);
            RGB_color(0, 0, 255); // Blue
            delay(500);
            RGB_color(255, 0, 0); // Red
            delay(500);
            RGB_color(0, 0, 255); // Blue
            delay(500);
            RGB_color(255, 0, 0); // Red
            delay(500);
            RGB_color(0, 0, 255); // Blue
            delay(500);
            RGB_color(0, 0, 0);

            police = 0;
            break;
          }
        }
        
      }
      }
      
      lcd.clear();
      clearData();
    }

    
  }



void clearData() {
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
  return;
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
{
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}
