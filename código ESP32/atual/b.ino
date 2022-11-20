//Importação das bibliotecas
#include "AHT10.h"
#include "LCD.h"
#include "LED.h"
#include "wifi.h"
#include "IFTT.h"
#include <Wire.h>

#define SDA 5
#define SCL 4

void setup(){
  Serial.begin(115200);
  Wire.begin(SDA, SCL); //Inicialização do sensor e do display
  delay(5000);
  wifiSetup(); //Função para conectar ao wifi
  sensorSetup(); //Função para iniciar o sensor
  lcdSetup(); //Função para iniciar o display
  ledSetup(); //Função para iniciar os LED
}

void loop() {
  float t = getTemp(); //Criação de variável para guardar o valor da temperatura atual lida pelo sensor
  float u = getHum(); //Criação de variável para guardar o valor da umidade atual lida pelo sensor
  currentStatus(t, u);
  lcdClear();
  int tERR = temp(t);
  lcdClear();
  int uERR = umi(u);
  lcdClear();
  resetLED();
  changeLED(tERR, uERR);
  sendData(t, u);
  delay(10);
}