#include <HTTPClient.h>

//Estabelecimento da conexão ao servidor
String server = "http://maker.ifttt.com";
String eventName = "ESP32_DATA";
String IFTTT_Key = "cx0CXpfFUenm9hCyyRD5WW";
String IFTTTUrl="https://maker.ifttt.com/trigger/ESP32_DATA/with/key/cx0CXpfFUenm9hCyyRD5WW";

float value1;
float value2;

//Função para enviar os dados coletados ao Google Sheets
void sendDataToSheet()
{
  String url = server + "/trigger/" + eventName + "/with/key/" + IFTTT_Key + "?value1=" + String((float)value1) + "&value2="+String((float)value2);  
  Serial.println(url);
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  http.begin(url);

  Serial.print("[HTTP] GET...\n");
  int httpCode = http.GET();

  //Verificação para evitar erros
  if(httpCode > 0) {
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}

void sendData(float t, float u) {
  value1 = t;
  value2 = u;
  sendDataToSheet();
}