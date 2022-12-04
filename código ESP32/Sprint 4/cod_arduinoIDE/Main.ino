//Importação das bibliotecas criadas e da biblioteca "Wire.h" para utilização do SDA e SCL
#include "AHT10.h"
#include "LCD.h"
#include "LED.h"
#include "wifi.h"
#include "IFTT.h"
#include <Wire.h>

//Variáveis para guardar a informação das portas utilizadas para SDA e SCL no dispositivo
#define SDA 5
#define SCL 4

int delayTime = 200; //Variável utilizada para definir o tempo de espera da execução de cada loop
int timer = 0; //Variável utilizada para controlar a velocidade das mensagens do display
int sendDataTimer = 0; //Variável utilizada para controlar a velocidade do envio de dados
float tempMax;
float tempMin;
float umidMax;
float umidMin;

void setup(){
  Serial.begin(115200);
  Wire.begin(SDA, SCL); //Inicialização do sensor e do display
  int wifiWorking = wifiSetup(); //Função para conectar ao wifi
  tempMax = getMaxTemp(); //Função para pegar o limite salvo na configuração temperatura máxima
  tempMin = getMinTemp(); //Função para pegar o limite salvo na configuração temperatura mínima
  umidMax = getMaxUmid(); //Função para pegar o limite salvo na configuração temperatura máxima
  umidMin = getMinUmid(); //Função para pegar o limite salvo na configuração temperatura mínima
  int sensorWorking = sensorSetup(); //Função para iniciar o sensor
  lcdSetup(); //Função para iniciar o display

  //Verificação de erro na conexão com o sensor
  if (sensorWorking == 0){
    sensorError();
  }

  //Verificação de erro da conexão wifi
  if (wifiWorking == 0){
    wifiError();
  }

  ledSetup(); //Função para iniciar os LED
}

void loop() {
  float t = getTemp(); //Criação de variável para guardar o valor da temperatura atual lida pelo sensor
  float u = getHum(); //Criação de variável para guardar o valor da umidade atual lida pelo sensor

  //Caso de erro de sensor descalibrado
  if(t < 0 || t > 100 || u < 0 || u > 150){
    sensorReadError();
  }

  timer = currentLcd(timer); //Carregamento do tempo local atual para mostrar no LCD
  lcdClear(); //Reset do LCD
  currentStatus(t, u, timer); //Função para mostrar a temperatura e umidade atual no display

  //Leitura da temperatura e umidade
  int tERR = temp(t, timer, tempMax, tempMin);
  int uERR = umi(u, timer, umidMax, umidMin);

  resetLED(); //Reset dos LEDs
  changeLED(tERR, uERR, timer); //Função para configurar os LEDs com base na situação atual de temperatura e umidade

  //Verificação se passou 1 minuto utilizando o timer e envio dos dados ao Google Sheets
  if(sendDataTimer > 59990){
    int sendingError = sendData(t, u);
    //Verificação de erro no envio dos dados
    if(sendingError == 1){
      ifttError();
    }
    sendDataTimer = 0;
  }

  //Adição do tempo de delay nas variáveis temporais e delay
  timer += delayTime;
  sendDataTimer += delayTime;
  delay(delayTime);
}