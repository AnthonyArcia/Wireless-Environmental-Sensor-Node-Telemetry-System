//#include "DHT.h"
#include "DHTesp.h"

#define DHTPIN 14     // what digital pin the DHT22 is conected to 'D5'
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors

//DHT dht(DHTPIN, DHTTYPE);
DHTesp dht;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);

  // Wait for serial to initialize.
  while(!Serial) { }

  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  Serial.println("Running DHT!");
  Serial.println("-------------------------------------");
  dht.setup(14, DHTesp::DHT22); // Connect DHT sensor to GPIO 14//17
}

int timeSinceLastRead = 0;
void loop() {

  // Report every 2 seconds.
  if(timeSinceLastRead > 2000) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    //float h = dht.readHumidity();
    float h = dht.getHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.getTemperature();
    //float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float f = dht.readTemperature(true);
    float f = dht.toFahrenheit(t);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      timeSinceLastRead = 0;
      return;
    }

    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h, true);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" ");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print(" *C ");
    Serial.print(hif);
    Serial.println(" *F");

    timeSinceLastRead = 0;
  }
  delay(100);
  timeSinceLastRead += 100;
}
