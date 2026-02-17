#include <Wire.h>
#include <WiFiConfig.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

// replace with your ESP-32 MAC address
uint8_t receiverAddress[] = {0xA8, 0x46, 0x74, 0x5C, 0x13, 0x98};

// weather report object for sending report
WeatherReport report;

const int LIGHT_PIN = 3;         // ADC pin
const int LIGHT_THRESHOLD = 350; // threshold for adjusting when it triggers day and night

void setup() {
  Serial.begin(115200);

  // initialize I2C on custom pins (SDA, SCL)
  Wire.begin(5, 6);

  // check for AHT20 status
  if (!aht.begin()) {
    Serial.println("AHT20 not detected!");
    while (1);
  }

  // setup WiFi
  wifi_setup();

  // configure ESP32 as transmitter
  peer_setup(receiverAddress); 
}

void loop() {
  int lightValue{};                // int for store LDR reading
  sensors_event_t humidity, temp;  // AHT20 sensor reading object for storing data

  // read current temperature and humidity
  aht.getEvent(&humidity, &temp); 

  // read analog value from light sensor
  lightValue = analogRead(LIGHT_PIN);

  report.temperature = temp.temperature;        // save temperature to report
  report.humidity = humidity.relative_humidity; // save humidity to report


  // if lightValue is greater than set threshold, report day, else its night
  if (lightValue > LIGHT_THRESHOLD) {
    report.light = 1;  // Day
  } else {
    report.light = 0;  // Night
  }

  send_report(receiverAddress, report);

  // display report to Serial Monitor
  Serial.print("Temp: "); Serial.print(report.temperature); Serial.println(" C");
  Serial.print("Humidity: "); Serial.print(report.humidity); Serial.println(" %");
  Serial.print("Light ADC: "); Serial.println(lightValue);


  // if report returns 1, display day, else display night
  if (report.light == 1) {
    Serial.println("Time: Day");
  } else {
    Serial.println("Time: Night");
  }
  Serial.println("--------------------");

  delay(10000);
}
