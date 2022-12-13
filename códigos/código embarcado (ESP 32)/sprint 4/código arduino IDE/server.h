#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

AsyncWebServer server(80);

const char* nameWifi = "nameWifiText";
const char* passWifi = "passWifiText";
const char* tMax = "tMaxInt";
const char* tMin = "tMinInt";
const char* uMax = "uMaxInt";
const char* uMin = "uMinInt";

// HTML para rodar no servidor WEB
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Config</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script>
    function submitMessage() {
      alert("Valor salvo no ESP");
      setTimeout(function(){ document.location.reload(false); }, 500);   
    }
  </script></head><body>

  <form action="/get" target="hidden-form">
    Faca a configuracao do dispostivo: insira dados da rede e das faixas de medicao. Clique em Enviar para enviar os dados.<br><br><br> 
  
    Insira o nome da rede WIFI (nome da rede atual: %nameWifiText%): <input type="text" name="nameWifiText">
    <input type="submit" value="Enviar" onclick="submitMessage()">
  </form><br>

  <form action="/get" target="hidden-form">
    Insira a senha da rede WIFI (senha da rede atual: %passWifiText%): <input type="text" name="passWifiText">
    <input type="submit" value="Enviar" onclick="submitMessage()">
  </form><br>

  <form action="/get" target="hidden-form">
    Defina o limite maximo da temperatura (limite atual: %tMaxInt%): <input type="number" name="tMaxInt">
    <input type="submit" value="Enviar" onclick="submitMessage()">
  </form><br>

  <form action="/get" target="hidden-form">
    Defina o limite minimo da temperatura (limite atual: %tMinInt%): <input type="number" name="tMinInt">
    <input type="submit" value="Enviar" onclick="submitMessage()">
  </form><br>

  <form action="/get" target="hidden-form">
    Defina o limite maximo da umidade (limite atual: %uMaxInt%): <input type="number" name="uMaxInt">
    <input type="submit" value="Enviar" onclick="submitMessage()">
  </form><br>

  <form action="/get" target="hidden-form">
    Defina o limite minimo da umidade (limite atual: %uMinInt%): <input type="number" name="uMinInt">
    <input type="submit" value="Enviar" onclick="submitMessage()">
  </form><br>

  <iframe style="display:none" name="hidden-form"></iframe>
</body></html>)rawliteral";

String readFile(fs::FS &fs, const char * path){
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    return String();
  }
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  file.close();
  return fileContent;
}


void writeFile(fs::FS &fs, const char * path, const char * message){
  File file = fs.open(path, "w");
  if(!file){
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}


String processor(const String& var){
  if(var == "nameWifiText"){
    return readFile(SPIFFS, "/nameWifiText.txt");
  }

  else if(var == "passWifiText"){
    return readFile(SPIFFS, "/passWifiText.txt");
  }

  else if(var == "tMaxInt"){
    return readFile(SPIFFS, "/tMaxInt.txt");
  }

  else if(var == "tMinInt"){
    return readFile(SPIFFS, "/tMinInt.txt");
  }

  else if(var == "uMaxInt"){
    return readFile(SPIFFS, "/uMaxInt.txt");
  }

  else if(var == "uMinInt"){
    return readFile(SPIFFS, "/uMinInt.txt");
  }

  return String();
}


void serverSetup() {
  #ifdef ESP32
    if(!SPIFFS.begin(true)){
      return;
    }
  #else
    if(!SPIFFS.begin()){
      return;
    }
  #endif

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/get?inputString=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;

    if (request->hasParam(nameWifi)) {
      inputMessage = request->getParam(nameWifi)->value();
      writeFile(SPIFFS, "/nameWifiText.txt", inputMessage.c_str());
    }

    else if (request->hasParam(passWifi)) {
      inputMessage = request->getParam(passWifi)->value();
      writeFile(SPIFFS, "/passWifiText.txt", inputMessage.c_str());
    }

    else if (request->hasParam(tMax)) {
      inputMessage = request->getParam(tMax)->value();
      writeFile(SPIFFS, "/tMaxInt.txt", inputMessage.c_str());
    }

    else if (request->hasParam(tMin)) {
      inputMessage = request->getParam(tMin)->value();
      writeFile(SPIFFS, "/tMinInt.txt", inputMessage.c_str());
    }

    else if (request->hasParam(uMax)) {
      inputMessage = request->getParam(uMax)->value();
      writeFile(SPIFFS, "/uMaxInt.txt", inputMessage.c_str());
    }

    else if (request->hasParam(uMin)) {
      inputMessage = request->getParam(uMin)->value();
      writeFile(SPIFFS, "/uMinInt.txt", inputMessage.c_str());
    }

    else {
      inputMessage = "Erro no envio";
    }

    request->send(200, "text/text", inputMessage);
  });
  server.begin();
}