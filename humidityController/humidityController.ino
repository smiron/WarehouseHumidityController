#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define DHT_TYPE      DHT22
#define DHT_PIN       4
#define RELAY1_PIN    7
#define RELAY_OFF     1
#define RELAY_ON      0

#define TIME_SLICE    500
#define HUMIDITY_TIME_SLICES_MEASURE  4
#define BLINKLED_TIME_SLICES_MEASURE  1

#define HUMIDITY_MIN  90
#define HUMIDITY_MAX  93

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

bool relayState = false;
int humidityTimeSliceCurrent = 0;
int blickLedTimeSliceCurrent = 0;

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  
  Serial.begin(9600);
  dht.begin();
  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.backlight();
  lcd.print("Se porneste ...");  

  pinMode(RELAY1_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, RELAY_OFF);
}

void loop() {

  delay(TIME_SLICE);

  measureHumidity();

  blickLed();
}

bool ledState = false;

void blickLed(){
  if (++blickLedTimeSliceCurrent < BLINKLED_TIME_SLICES_MEASURE) {
    return;
  }

  blickLedTimeSliceCurrent = 0;

  ledState = !ledState;
  if (ledState)
    digitalWrite(13, HIGH);
  else
    digitalWrite(13, LOW);
}

void measureHumidity(){

  if (++humidityTimeSliceCurrent < HUMIDITY_TIME_SLICES_MEASURE) {
    return;
  }

  humidityTimeSliceCurrent = 0;
  
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (relayState) {
    if (humidity > HUMIDITY_MAX) {
      digitalWrite(RELAY1_PIN, RELAY_OFF);
      relayState = false;
    }
  }
  else {
    if (humidity < HUMIDITY_MIN) {
      digitalWrite(RELAY1_PIN, RELAY_ON);
      relayState = true;
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("H20:  ");
  lcd.print(humidity, 2);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temperature, 2);
  lcd.print("C");

  Serial.print("Humidity: ");
  Serial.println(relayState ? "ON" : "OFF");
}

