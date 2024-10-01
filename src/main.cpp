#include <Arduino.h>
#include "thermostats.h"

void setup()
{
  Serial.begin(115200);
  dht.begin(); 

  homeSpan.begin(Category::Thermostats, "Temperature Sensor");
  homeSpan.setPairingCode("11122333");

  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Identify();
  new MyHumiditySensor();
  new MyTemperatureSensor();
  
}

void loop()
{
  homeSpan.poll();
}
