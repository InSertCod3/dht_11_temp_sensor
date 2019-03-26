#include "DHT.h"

#define DHTPIN 2    // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
// #define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
// #define DHTTYPE DHT21   // DHT 21 (AM2301)

// $ Variables
long Temperture_delay = 9000; // 1000 = 1 sec
// 10 * 60 * 1000600  1min = 60000
long previousMillis = 0;
float heat_threshold_HOT = 88.55; // fah. above to activate fan
float heat_threshold_COLD = 80.10; // fah. above to activate fan
bool fan_on = true;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("Fan Toogler v1 \n");
  pinMode(5, OUTPUT);

  dht.begin();
}

void toggle_fan_relay_ON () {
  Serial.print("Turning Fan ON \n");
  fan_on = true;
  digitalWrite(5, LOW);
}

void toggle_fan_relay_OFF () {
  Serial.print("Turning Fan OFF \n");
  fan_on = false;
  digitalWrite(5, HIGH);
}

void loop() {
  // Wait a few seconds between measurements.
  unsigned long currentMillis = millis();
  
  if(currentMillis - previousMillis > Temperture_delay) {
    previousMillis = currentMillis;
  // delay(10 * 60 * 1000);
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);

  //Log Data In Serial
  Serial.print("Current Heat Is : ");
  Serial.print(hif);
  Serial.print("\n");
  Serial.print("Current Temperature Is : ");
  Serial.print(f);
  Serial.print("\n");
  
  if (hif >= heat_threshold_HOT){
    if (fan_on == false){
      toggle_fan_relay_ON();
      Serial.print("DID : toggle_fan_relay_ON() <heat_threshold_HOT\n");
      return;
    } else {
      Serial.print("fan already on <heat_threshold_HOT\n");
      return;
    }
  }
  if (hif <= heat_threshold_COLD){
    if (fan_on == true){
    toggle_fan_relay_OFF();
    Serial.print("DID : toggle_fan_relay_OFF <heat_threshold_COLD\n");
    return;
    } else {
      Serial.print("fan already off <heat_threshold_COLD\n");
      return;
    }
  }

  if (hif > heat_threshold_COLD && hif < heat_threshold_HOT) {
      Serial.print("ROOM Temp is Balanced \n");
      if (fan_on == true){
           toggle_fan_relay_OFF();
           Serial.print("DID : toggle_fan_relay_OFF() Balanced \n");
           return;
      } else {
        Serial.print("fan already off and were Balanced \n");
        return;
    }
    }
  }
}

