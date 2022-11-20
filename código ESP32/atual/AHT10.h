//Importação da biblioteca do sensor AHT10
#include <Adafruit_AHT10.h>

Adafruit_AHT10 aht; //Criação de um objeto AHT10

//Função para iniciar o sensor
void sensorSetup(){
  if (aht.begin()){
    Serial.println("O sensor está conectado devidamente");
  }
  else{
    Serial.println("O sensor não está conectado devidamente");
  }
}

float getTemp(){
  sensors_event_t humidity, temp; //Criação de variáveis para guardar a temperatura lida pelo sensor
  aht.getEvent(&humidity, &temp); //Inserção do valor de temperatura na variável criada
  return temp.temperature; //Retorno da temperatura
}

float getHum(){
  sensors_event_t humidity, temp; //Criação de variáveis para guardar a umidade lida pelo sensor
  aht.getEvent(&humidity, &temp); //Inserção do valor de umidade na variável criada
  return humidity.relative_humidity; //Retorno da umidade
}