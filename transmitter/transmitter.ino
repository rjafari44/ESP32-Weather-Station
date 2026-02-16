#include <Wire.h>
#include <WiFiConfig.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

// replace with your ESP-32 MAC address
uint8_t receiverAddress[] = {0xA8, 0x46, 0x74, 0x5C, 0x13, 0x98};

// weather report object for sending report
WeatherReport report;

const int lightPin = 0;         // ADC pin
const int lightThreshold = 2000; // Adjust after testing

void setup() {
  Serial.begin(115200);

  // Initialize I2C on custom pins
  Wire.begin(6, 5);  // SDA=6, SCL=5

  // Initialize AHT20
  if (!aht.begin()) {
    Serial.println("AHT20 not detected!");
    while (1);
  }

  // Setup WiFi + ESP-NOW
  wifi_setup();
  peer_setup(receiverAddress);
}

void loop() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  int lightValue = analogRead(lightPin);

  report.temperature = (int)temp.temperature;
  report.humidity = (int)humidity.relative_humidity;

  report.light = (lightValue > lightThreshold) ? 1 : 0;

  send_report(receiverAddress, report);

  // Debug Serial
  Serial.print("Temp: "); Serial.print(report.temperature); Serial.println(" C");
  Serial.print("Humidity: "); Serial.print(report.humidity); Serial.println(" %");
  Serial.print("Time: "); Serial.println(report.light ? "Day" : "Night");
  Serial.println("--------------------");

  delay(10000);
}
