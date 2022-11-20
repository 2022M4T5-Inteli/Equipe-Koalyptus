//Importação da biblioteca de wifi
#include <WiFi.h>

const char* ssid = "Inteli-welcome";
const char* password = "";

void wifiSetup(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("conecting wifi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting");
  }
  Serial.println("Connected to the WiFi network: ");
  Serial.println(ssid);
  Serial.println("Network IP: ");
  Serial.println(WiFi.localIP());
}