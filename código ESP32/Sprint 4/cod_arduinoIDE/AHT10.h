//Importação da biblioteca do sensor AHT10
#include <Adafruit_AHT10.h>

Adafruit_AHT10 aht; //Criação de um objeto AHT10

//Função para iniciar o sensor
int sensorSetup(){
  int sensorWork;
  if (aht.begin()){
    Serial.println("O sensor está conectado devidamente");
    sensorWork = 1;
  }
  else{
    Serial.println("O sensor não está conectado devidamente");
    sensorWork = 0;
  }
  return sensorWork;
}

// Criação de variável da leitura de temperatura
float getTemp(){
  sensors_event_t humidity, temp; //Criação de variáveis para guardar a temperatura lida pelo sensor
  aht.getEvent(&humidity, &temp); //Inserção do valor de temperatura na variável criada
  return temp.temperature; //Retorno da temperatura
}

// Criação de variável da leitura de umidade
float getHum(){
  sensors_event_t humidity, temp; //Criação de variáveis para guardar a umidade lida pelo sensor
  aht.getEvent(&humidity, &temp); //Inserção do valor de umidade na variável criada
  return humidity.relative_humidity; //Retorno da umidade
}
