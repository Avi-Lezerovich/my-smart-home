#include "HomeSpan.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 15
#define DHTTYPE DHT11

#define UPDATE_INTERVAL 60 * 1000 // 60 sec
#define TEMP_DIFF_THRESHOLD 0.5   // 0.5 degree celsius
#define HUMID_DIFF_THRESHOLD 1.0  // 1.0 percent

DHT dht(DHTPIN, DHTTYPE);

struct TemperatureSensor : Service::TemperatureSensor
{

  SpanCharacteristic *currentTemperature;
  float temperature;

  TemperatureSensor() : Service::TemperatureSensor()
  {
    currentTemperature = new Characteristic::CurrentTemperature(10);
  }

  void loop()
  {
    if (currentTemperature->timeVal() < UPDATE_INTERVAL)
      return;

    temperature = dht.readTemperature();

    if (isnan(temperature) || abs(currentTemperature->getVal() - temperature) < TEMP_DIFF_THRESHOLD)
      return;

    currentTemperature->setVal(temperature);
  }
};

struct HumiditySensor : Service::HumiditySensor
{

  SpanCharacteristic *currentHumidity;
  float humidity;

  HumiditySensor() : Service::HumiditySensor()
  {
    currentHumidity = new Characteristic::CurrentRelativeHumidity(10);
  }

  void loop()
  {
    if (currentHumidity->timeVal() < UPDATE_INTERVAL)
      return;

    humidity = dht.readHumidity();
    if (isnan(humidity) || abs(currentHumidity->getVal() - humidity) < HUMID_DIFF_THRESHOLD)
      return;

    currentHumidity->setVal(humidity);
  }
};
