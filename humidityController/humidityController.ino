#include <DHT.h>

#define DHT_TYPE      DHT22
#define DHT_PIN       2
#define RELAY1_PIN    7
#define RELAY_OFF     1
#define RELAY_ON      0

#define TIME_SLICE    500
#define HUMIDITY_TIME_SLICES_MEASURE  4
#define BLINKLED_TIME_SLICES_MEASURE  1

#define HUMIDITY_MIN  90
#define HUMIDITY_MAX  93

bool relayState = false;
int humidityTimeSliceCurrent = 0;
int blickLedTimeSliceCurrent = 0;

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);
  pinMode(RELAY1_PIN, OUTPUT);

  dht.begin();
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

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C \t");

  Serial.print("Humidity: ");
  Serial.println(relayState ? "ON" : "OFF");
}

