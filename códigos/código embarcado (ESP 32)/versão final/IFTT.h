#include <HTTPClient.h>

//Estabelecimento da conexão ao servidor
String iftttserver = "http://maker.ifttt.com";
String eventName = "ESP32_DATA";
String IFTTT_Key = "cx0CXpfFUenm9hCyyRD5WW";
String IFTTTUrl="https://maker.ifttt.com/trigger/ESP32_DATA/with/key/cx0CXpfFUenm9hCyyRD5WW";

//Enviando os dados de temperatura para o IFTTT 
//Link da planilha https://docs.google.com/spreadsheets/d/1L7lRsg9MDNTPqTWmi661626ncFGmp9wtVwGpQj_ZS4Y/edit#gid=0
String iftttserver2 = "http://maker.ifttt.com";
String eventName2 = "receive-Input-Temp";
String IFTTT_Key2 = "dLo1XZ4t2aqmQRILe-aRajmx9FXNShDdlByGWIeVx9f";


//Enviando os dados de umidade para o IFTTT
//Link da planilha https://docs.google.com/spreadsheets/d/171ubYrqZ7Vq9fynssYJwAk017Jty2fHZbe9cAV_bUIY/edit#gid=0
String eventName3 = "receive-Input-Umi";


//valores usados para o envio de temperatura e umidade atual
float value1;
float value2;

//Função para enviar os dados coletados ao Google Sheets
int sendDataToSheet(){
  int ifttError = 0;
  //Envio dos dados
  String url = iftttserver + "/trigger/" + eventName + "/with/key/" + IFTTT_Key + "?value1=" + String((float)value1) + "&value2="+String((float)value2);  
  Serial.println(url);
  HTTPClient http;

  //Conexão com o link
  http.begin(url);
  int httpCode = http.GET();

  //Verificação de erros
  if(httpCode > 0) {
    Serial.println(value1);
    Serial.println(value2);
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  }

  //Caso de erro no envio
  else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    ifttError = 1;
  }
  http.end();
  return ifttError;
}

//Função que envia os dados da temperatura para o sheets
void makeIFTTTRequestTemp(float tempMax, float tempMin){

  float valueTempMax = tempMax;
  float valueTempMin = tempMin;

  //Definição do que deve ser enviado
  String urlTemp = iftttserver2 + "/trigger/" + eventName2 + "/with/key/" + IFTTT_Key2 + "?value1=" + String((float)valueTempMax) + "&value2="+ String((float)valueTempMin);  
  Serial.println(urlTemp);
  HTTPClient http;

  //Conexão com o link
  http.begin(urlTemp);
  int httpCode = http.GET();

  //Verificação de erros
  if(httpCode > 0) {
    Serial.println(valueTempMax);
    Serial.println(valueTempMin);
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  }
}


void makeIFTTTRequestUmi(float umidMax, float umidMin){
  float valueUmiMax = umidMax;
  float valueUmiMin = umidMin;

  //Definição do que deve ser enviado
  String urlUmi = iftttserver2 + "/trigger/" + eventName3 + "/with/key/" + IFTTT_Key2 + "?value1=" + String((float)valueUmiMax) + "&value2="+ String((float)valueUmiMin);  
  Serial.println(urlUmi);
  HTTPClient http;

  //Conexão com o link
  http.begin(urlUmi);
  int httpCode = http.GET();

  //Verificação de erros
  if(httpCode > 0) {
    Serial.println(valueUmiMax);
    Serial.println(valueUmiMin);
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  }
}

//Função para pegar os dados de temperatura e umidade e passá-los para as variáveis a serem enviadas
int sendData(float t, float u) {
  value1 = t;
  value2 = u;
  int sendError = sendDataToSheet();
  return sendError;
}