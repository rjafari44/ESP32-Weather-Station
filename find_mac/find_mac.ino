#include <WiFi.h>

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);     // Set ESP32 to Station mode
  Serial.println(WiFi.macAddress());  // Print MAC address
}

void loop()
{
}
