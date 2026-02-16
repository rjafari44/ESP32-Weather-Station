#include <Wire.h>
#include <WiFiConfig.h>
#include <LiquidCrystal_I2C.h>

// LCD at address 0x27, 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

WeatherReport report;

void setup() {
  Serial.begin(115200);

  // Initialize I2C on custom pins SDA=5, SCL=4
  Wire.begin(5, 4);   // Must be called BEFORE lcd.init()!

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Setup WiFi + ESP-NOW
  wifi_setup();

  // Configure this ESP32 as receiver
  set_report_receiver(&report);

  lcd.setCursor(0,0);
  lcd.print("Waiting...");
}

void loop() {
  lcd.clear();

  // Row 1: Temperature & Humidity
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(report.temperature);
  lcd.print("C ");

  lcd.print("H:");
  lcd.print(report.humidity);
  lcd.print("%");

  // Row 2: Day/Night
  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(report.light == 1 ? "Day" : "Night");

  // Serial monitor debug
  Serial.print("Temp: ");
  Serial.println(report.temperature);
  Serial.print("Humidity: ");
  Serial.println(report.humidity);
  Serial.print("Light: ");
  Serial.println(report.light);

  delay(5000);   // Refresh every 5 seconds
}
