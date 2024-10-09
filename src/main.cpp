#include <HomeSpan.h>
#include "temperature_humidity_sensor.h"


void setup()
{
  dht.begin();
  Serial.begin(115200);
  homeSpan.begin(Category::Bridges,"ESP32 Bridge");
  homeSpan.setPairingCode("00000000");
homeSpan.setWifiCredentials("SSD", "PASSWORD");

 
   

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      
  new SpanAccessory();
    new Service::AccessoryInformation();

      new Characteristic::Identify();
      new Characteristic::Name("Temperature Sensor");
    new TemperatureSensor();

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name("Humidity Sensor");
    new HumiditySensor();

}

void loop()
{
  homeSpan.poll();
}
