#include <Wire.h>
#include <WiFiConfig.h>
#include <LiquidCrystal_I2C.h>

// LCD at address 0x27, 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// weather report object for receiving report
WeatherReport report;

void setup() {
  Serial.begin(115200);

  // initialize I2C on custom pins (SDA, SCL)
  Wire.begin(5, 4);   // Must be called BEFORE lcd.init()!

  // initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // setup WiFi
  wifi_setup();

  // configure this ESP32 as receiver
  set_report_receiver(&report);

  // display an initial waiting on row 1
  lcd.setCursor(0,0);
  lcd.print("Waiting...");
}

void loop() {
  lcd.clear();

  // row 1: temperature & humidity
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(report.temperature);
  lcd.print("C ");

  lcd.print("H:");
  lcd.print(report.humidity);
  lcd.print("%");

  // row 2: Day/Night
  lcd.setCursor(0, 1);
  lcd.print("Time: ");

  // if report returns 1, display day, else display night
  if (report.light == 1) {
    lcd.print("Day");
  } else {
    lcd.print("Night");
  }

  // display the report to the serial monitor
  Serial.print("Temp: ");
  Serial.println(report.temperature);
  Serial.print("Humidity: ");
  Serial.println(report.humidity);
  Serial.print("Light: ");
  Serial.println(report.light);

  delay(5000);   // refresh every 5 seconds to account for any report time mismatch
}
