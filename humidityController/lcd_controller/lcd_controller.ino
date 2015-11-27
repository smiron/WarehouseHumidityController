#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal.h>

/* YourDuino.com Example Software Sketch
   DHT11 Humidity and Temperature Sensor test
   Displayed on I2C LCD Display
   Credits: Rob Tillaart
   http://arduino-info.wikispaces.com/PROJECT-Temp-Humidity-Display
   terry@yourduino.com */
   
/*-----( Import needed libraries )-----*/
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
#define DHTTYPE DHT22
#define DHTPIN 4
DHT dht(DHTPIN, DHTTYPE);
int ledPin = 13;


/*-----( Declare Constants, Pin Numbers )-----*/


void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  
  Serial.begin(9600); //(Remove all 'Serial' commands if not needed)
  Serial.println("START SETUP");
  //pinMode(ledPin, OUTPUT);

  
  dht.begin();
  
  lcd.begin(16,2);
  lcd.backlight();
  lcd.print("DHT22 Temp/Humid");
  
  Serial.println("END SETUP");
}/*--(end setup )---*/

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  delay(2000);
  Serial.println("LOOP");
  
  lcd.setCursor(0, 1);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  
  lcd.print("C=");
  lcd.print(temperature, 0);
  
  Serial.print("Temperature (oC): ");
  Serial.println(temperature, 2);

  Serial.print("Temperature (oF): ");
  Serial.println(Fahrenheit(temperature), 2);

  
  lcd.print("  H=");
  lcd.print(humidity, 0);
  lcd.print("%");
  
  Serial.print("Humidity (%): ");
  Serial.println(humidity, 2);  

  Serial.print("Temperature (K): ");
  Serial.println(Kelvin(temperature), 2);

  Serial.print("Dew Point (oC): ");
  Serial.println(dewPoint(temperature, humidity));

  Serial.print("Dew PointFast (oC): ");
  Serial.println(dewPointFast(temperature, humidity));

  
}/* --(end main loop )-- */

/*-----( Declare User-written Functions )-----*/
//
//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
        return 1.8 * celsius + 32;
}

//Celsius to Kelvin conversion
double Kelvin(double celsius)
{
        return celsius + 273.15;
}

// dewPoint function NOAA
// reference: http://wahiduddin.net/calc/density_algorithms.htm 
double dewPoint(double celsius, double humidity)
{
        double A0= 373.15/(273.15 + celsius);
        double SUM = -7.90298 * (A0-1);
        SUM += 5.02808 * log10(A0);
        SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
        SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
        SUM += log10(1013.246);
        double VP = pow(10, SUM-3) * humidity;
        double T = log(VP/0.61078);   // temp var
        return (241.88 * T) / (17.558-T);
}

// delta max = 0.6544 wrt dewPoint()
// 5x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
        double a = 17.271;
        double b = 237.7;
        double temp = (a * celsius) / (b + celsius) + log(humidity/100);
        double Td = (b * temp) / (a - temp);
        return Td;
}
