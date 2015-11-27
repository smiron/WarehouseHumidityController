#include <Wire.h> 
#include <LiquidCrystal_I2C.h>



#define BACKLIGHT_PIN     13

//LiquidCrystal_I2C lcd(0x27);  // Set the LCD I2C address

LiquidCrystal_I2C lcd(0x3F, BACKLIGHT_PIN, POSITIVE);  // Set the LCD I2C address

void setup()
{
  // Switch on the backlight
  pinMode ( BACKLIGHT_PIN, OUTPUT );
  digitalWrite ( BACKLIGHT_PIN, HIGH );
  
  lcd.begin(16,2);               // initialize the lcd 

  lcd.home ();                   // go home
  lcd.print("Hello, ARDUINO ");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print (" FORUM - fm   ");
}

void loop()
{
   
}
