//Importação da biblioteca de wifi
#include <WiFi.h>
#include "server.h"
#include "time.h"

//Constantes para guardar informações de nome e senha do wifi embarcado
const char* serverSsid = "Koalyptus";
const char* serverPassword = "koalaslyptus";

int wifiSetup(){
  int wifiOnOff;


  WiFi.mode(WIFI_STA);
  WiFi.softAP(serverSsid, serverPassword); //Conexão wifi
  serverSetup(); //Função para iniciar o servidor embarcado

  //Constantes para guardar informações de nome e senha do wifi externo
  String wifiSsid = readFile(SPIFFS, "/nameWifiText.txt");
  String wifiPassword = readFile(SPIFFS, "/passWifiText.txt");
  const char* wifiSsidChar = wifiSsid.c_str();
  const char* wifiPasswordChar = wifiPassword.c_str();

  WiFi.begin(wifiSsidChar, wifiPasswordChar); //Conexão wifi
  Serial.println("conecting wifi");
  delay(5000);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("ERR_03");
    wifiOnOff = 0;
  }
  else{
    Serial.println("Connected to the WiFi network");
    wifiOnOff = 1;
  }

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  return wifiOnOff;
}

float getMaxTemp(){
  float tMax = readFile(SPIFFS, "/tMaxInt.txt").toFloat();
  return tMax;
}

float getMinTemp(){
  float tMin = readFile(SPIFFS, "/tMinInt.txt").toFloat();
  return tMin;
}

float getMaxUmid(){
  float uMax = readFile(SPIFFS, "/uMaxInt.txt").toFloat();
  return uMax;
}

float getMinUmid(){
  float uMin = readFile(SPIFFS, "/uMinInt.txt").toFloat();
  return uMin;
}
