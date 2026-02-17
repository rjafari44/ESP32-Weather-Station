#include <WiFi.h>

void setup()
{
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);     // set ESP32 to station mode
  Serial.println(WiFi.macAddress());  // print MAC address
}

void loop()
{
  // nothing
}
