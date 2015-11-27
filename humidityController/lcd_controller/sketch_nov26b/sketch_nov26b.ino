#include <Wire.h> 

#include <LiquidCrystal_I2C.h>

#include "DHT.h"

#define DHTPIN 2

#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

int ledPin = 13;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address


void setup()  
{
Serial.begin(9600); 
Serial.println("DHT22");
pinMode(ledPin, OUTPUT);
 
  dht.begin();
  
  lcd.begin(16,2);    
  
// ------- Quick 3 blinks of backlight  -------------
  for(int i = 0; i< 3; i++)
  {
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight(); // finish with backlight on  

//-------- Write characters on the display ------------------
  // NOTE: Cursor Position: Lines and Characters start at 0  
  lcd.setCursor(3,0); //Start at character 3 on line 0
  lcd.print("Mini Dyno 2000");
  delay(1000);
  lcd.setCursor(5,1);
  lcd.print("Powered by");
  delay(1000);  
  lcd.setCursor(5,2);
  lcd.print("SimpleDyno");
  delay(2000);   

}


void loop()  
{
  
  digitalWrite(ledPin, HIGH);
  delay(10);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  
if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);

  lcd.setCursor(3,0);
  lcd.print("Mini Dyno 2000");
  lcd.setCursor(3,1);
  lcd.print("Humidity:"); 
  lcd.print(h);
  lcd.print("%    ");
  lcd.setCursor(3,2);
  lcd.print("Temp: "); 
  lcd.print(f);
  lcd.print(" *F      ");
  lcd.setCursor(0,3);
  lcd.print("Heat Index: ");
  lcd.print(hi);
  lcd.print(" *F");
   
  
  {
    // when characters arrive over the serial port...
    if (Serial.available()) {
      // wait a bit for the entire message to arrive
      delay(100);
      // clear the screen
      lcd.clear();
      // read all the available characters
      while (Serial.available() > 0) {
        // display each character to the LCD
        lcd.write(Serial.read());
        
        
      }
    }
  }

}
