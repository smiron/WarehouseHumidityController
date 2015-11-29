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
#define SPEAKER_PIN  12

#define TIME_SLICE    500
#define HUMIDITY_TIME_SLICES_MEASURE  4
#define BLINKLED_TIME_SLICES_MEASURE  1

#define HUMIDITY_MIN      80
#define HUMIDITY_MIN_ABS  75

#define HUMIDITY_MAX      85
#define HUMIDITY_MAX_ABS  95

#define TEMP_MIN_ABS  3
#define TEMP_MAX_ABS  15

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

bool relayState = false;
int humidityTimeSliceCurrent = 0;
int blickLedTimeSliceCurrent = 0;

float humidity;
float temperature;

bool hasHumidityError = false;
bool hasTemperatureError = false;

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  
  Serial.begin(9600);
  dht.begin();
  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("H20:  __._%");
  lcd.setCursor(0, 1);
  lcd.print("Temp: __._");
  lcd.print((char)223);

  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  
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
  
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
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

  lcd.setCursor(14, 0);
  if (relayState) {  
    lcd.print("+");
  }
  else {
    lcd.print(" ");
  }

  alarmHumidity();
  alarmTemperature();
  soundAlarm();
    
  lcd.setCursor(6, 0);
  lcd.print("     ");
  lcd.setCursor(6, 0);
  lcd.print(humidity, 1);
  lcd.print("%");

  lcd.setCursor(6, 1);
  lcd.print("     ");
  lcd.setCursor(6, 1);
  lcd.print(temperature, 1);
  lcd.print((char)223);

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("\tTemperature: ");
  Serial.print(temperature);
  Serial.print("\tState: ");
  Serial.println(relayState ? "ON" : "OFF");
}

void soundAlarm() {
  if (hasTemperatureError || hasHumidityError) {
    playTone(150, 5);
  }
}

void alarmTemperature() {
  if (temperature < TEMP_MIN_ABS || temperature > TEMP_MAX_ABS) {
    lcd.setCursor(15, 1);
    lcd.print("E");
    hasTemperatureError = true;
  }
  else {
    lcd.setCursor(15, 1);
    lcd.print(" ");
    hasTemperatureError = false;
  }
}

void alarmHumidity() {
  if (humidity < HUMIDITY_MIN_ABS || humidity > HUMIDITY_MAX_ABS) {
    lcd.setCursor(15, 0);
    lcd.print("E");
    hasHumidityError = true;
  }
  else {
    lcd.setCursor(15, 0);
    lcd.print(" ");
    hasHumidityError = false;
  }
}

void playTone(long duration, int freq) {
    duration *= 1000;
    int period = (1.0 / freq) * 1000000;
    long elapsed_time = 0;
    while (elapsed_time < duration) {
        digitalWrite(SPEAKER_PIN,HIGH);
        delayMicroseconds(period / 2);
        digitalWrite(SPEAKER_PIN, LOW);
        delayMicroseconds(period / 2);
        elapsed_time += (period);
    }
}

