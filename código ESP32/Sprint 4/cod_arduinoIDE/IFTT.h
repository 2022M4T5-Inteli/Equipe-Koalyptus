#include <HTTPClient.h>

// Estabelecimento da conexão ao servidor
String iftttserver = "http://maker.ifttt.com";
String eventName = "ESP32_DATA";
String IFTTT_Key = "cx0CXpfFUenm9hCyyRD5WW";
String IFTTTUrl = "https://maker.ifttt.com/trigger/ESP32_DATA/with/key/cx0CXpfFUenm9hCyyRD5WW";

// Valores usados para o envio de temperatura e umidade atual
float value1;
float value2;

// Função para enviar os dados coletados ao Google Sheets
int sendDataToSheet()
{
  int ifttError = 0;
  // Envio dos dados
  String url = iftttserver + "/trigger/" + eventName + "/with/key/" + IFTTT_Key + "?value1=" + String((float)value1) + "&value2=" + String((float)value2);
  Serial.println(url);
  HTTPClient http;

  // Conexão com o link
  http.begin(url);
  int httpCode = http.GET();

  // Verificação de erros
  if (httpCode > 0)
  {
    Serial.println(value1);
    Serial.println(value2);
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      Serial.println(payload);
    }
  }

  // Caso de erro no envio
  else
  {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    ifttError = 1;
  }
  http.end();
  return ifttError;
}

// Função para pegar os dados de temperatura e umidade e passá-los para as variáveis a serem enviadas
int sendData(float t, float u)
{
  value1 = t;
  value2 = u;
  int sendError = sendDataToSheet();
  return sendError;
}