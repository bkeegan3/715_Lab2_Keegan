#include "DHT.h"

#define DHTPIN 2 
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000); // read every 2 second

  float temp = dht.readTemperature(true); // read as °F with true

  if (isnan(temp)){
    Serial.println("Missed DHT measurement");
    return;
  }

  Serial.print(F("%  Temperature: "));
  Serial.print(temp);
  Serial.println(F("°F"));

}
