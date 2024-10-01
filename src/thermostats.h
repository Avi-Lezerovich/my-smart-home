#include "HomeSpan.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 15
#define DHTTYPE DHT11

#define UPDATE_INTERVAL 10000 // 10-second update interval

DHT dht(DHTPIN, DHTTYPE);

struct MyTemperatureSensor : Service::TemperatureSensor
{
  unsigned long lastUpdateTime = 0, currentMillis;
  SpanCharacteristic *currentTemp;
  float temperature = 0.0;

  MyTemperatureSensor() : Service::TemperatureSensor()
  {
    currentTemp = new Characteristic::CurrentTemperature(0);
  }

  void loop()
  {
    temperature = dht.readTemperature();
    currentMillis = millis();
    if (currentMillis - lastUpdateTime >= UPDATE_INTERVAL && !isnan(temperature))
    {
      lastUpdateTime = currentMillis;
      currentTemp->setVal(temperature);
    }
  }
};

struct MyHumiditySensor : Service::HumiditySensor
{
  unsigned long lastUpdateTime = 0, currentMillis;
  SpanCharacteristic *currentHumidity;
  float humidity = 0.0;

  MyHumiditySensor() : Service::HumiditySensor()
  {
    currentHumidity = new Characteristic::CurrentRelativeHumidity(0);
  }

  void loop()
  {
    humidity = dht.readHumidity();
    currentMillis = millis();
    if (currentMillis - lastUpdateTime >= UPDATE_INTERVAL && !isnan(humidity))
    {
      lastUpdateTime = currentMillis;
      currentHumidity->setVal(humidity);
    }
  }
};