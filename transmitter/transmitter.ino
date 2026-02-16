#include <WiFiConfig.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

// >>> REPLACE with your receiver's MAC address <<<
uint8_t receiverAddress[] = {0x34, 0xB7, 0xDA, 0xF6, 0x3E, 0x78};

WeatherReport report;

// Light sensor pin
const int lightPin = 34;     // ADC pin
const int lightThreshold = 2000;   // Adjust after testing

void setup() {
  Serial.begin(115200);

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

  // Read light level
  int lightValue = analogRead(lightPin);

  // Fill WeatherReport container
  report.temperature = (int)temp.temperature;
  report.humidity = (int)humidity.relative_humidity;

  if (lightValue > lightThreshold) {
    report.light = 1;   // Day
  } else {
    report.light = 0;   // Night
  }

  // Send report
  send_report(receiverAddress, report);

  // Debug to Serial
  Serial.print("Temp: ");
  Serial.print(report.temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(report.humidity);
  Serial.println(" %");

  Serial.print("Time: ");
  if (report.light == 1) {
    Serial.println("Day");
  } else {
    Serial.println("Night");
  }

  Serial.println("--------------------");

  delay(10000);   // 10 seconds
}
