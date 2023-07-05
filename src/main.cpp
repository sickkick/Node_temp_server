#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 2     // Digital pin D1 connected to the DHT sensor
#define DHTTYPE DHT11

#define WIRE Wire

DHT_Unified dht(DHTPIN, DHTTYPE);

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &WIRE);

uint32_t delayMS;

void setupOLED() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  Serial.println("OLED begun");

  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display(); // actually display all of the above
}

void setupTempSensor() {
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);

  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));

  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  
    // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 8000;
}

void setup() {
  Serial.begin(9600);
  
  // Initialize device.
  dht.begin();
  setupTempSensor();
  setupOLED();
}

void loop() {
  // Delay between measurements.
  delay(delayMS);

  display.clearDisplay();
  display.setCursor(20,0);
  display.print("Garage Temp:");

  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);

  if (isnan(event.temperature)) {
    display.setCursor(0,18);
    display.print("Error reading temperature!");
  } 
  else {
    int tempInCel = (int)round(1.8*event.temperature+32);
    String temp = String(tempInCel) + "F";

    display.setCursor(10,18);
    display.print("Temp: "+temp);
  }

  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    display.setCursor(0,30);
    display.print("Error reading humidity!");
  }
  else {
    String hum = String(event.relative_humidity) + "%";

    display.setCursor(10,30);
    display.print("Humidity: "+hum);
  }

  yield();
  display.display();
}