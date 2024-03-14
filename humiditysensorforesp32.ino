#include "DHT.h"
#define DHTPIN 26  
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
  dht.begin();
}
void loop() {
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();  /*Read default temperature in Celsius*/
  float f = dht.readTemperature(true);  /*Read temperature in Fahrenheit*/
  if (isnan(h) || isnan(t) || isnan(f)) {   /*if condition to check all reading taken or not*/
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print(F("Humidity: ")); /*prints humidity value*/
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C ")); /*prints temperature in Celsius*/
  Serial.print(f);
  Serial.println(F("°F ")); /*prints temperature in Fahrenheit*/
}