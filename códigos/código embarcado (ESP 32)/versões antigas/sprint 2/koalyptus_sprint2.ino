#include <LiquidCrystal_I2C.h>
#include "DHT.h"
// define as saidas SDA e SCL para o display
#define SDA_PIN 9
#define SCL_PIN 10
// define as colunas e linhas do display LCD
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
DHT dht(4, DHT11);
void setup(){
  dht.begin(); /*Inicia o sensor DHT11*/
  Wire.begin(SDA_PIN, SCL_PIN);
  // inicializa LCD
  lcd.init();
  // liga a luz no LCD                      
  lcd.backlight();
  Serial.begin(9600);
  pinMode(35, OUTPUT); /*LED cor amarela*/
  pinMode(36, OUTPUT); /*LED cor vermelha*/
  pinMode(37, OUTPUT); /*LED cor azul*/
}
// Esta função é responsável por criar alertas de acordo com as faixas de temperatura
void temperatura(float T){
  digitalWrite(36, LOW);
  // Condições especificas para cada faixa de temperatura
  if (T <= 26.6){
    lcd.setCursor(0, 0);
    lcd.print(("Alerta:"));
    lcd.setCursor(0, 1);
    lcd.print(("Temp Baixa"));
    digitalWrite(37, HIGH);
    return;
  }
  else if (T > 26.6 && T < 37.08){
    lcd.setCursor(0, 0);
    lcd.print(("Temp dentro do"));
    lcd.setCursor(0, 1);
    lcd.print(("esperado"));
    digitalWrite(37, LOW);
    return;
  }
  else if (T >= 37.08 && T < 37.8) {
    lcd.setCursor(0, 0);
    lcd.print(("Abrir metade das"));
    lcd.setCursor(0, 1);
    lcd.print(("Janelas laterais"));
    digitalWrite(37, HIGH);
    return;
  }
  else if (T >= 37.8 && T < 39.6) {
    lcd.setCursor(0, 0);
    lcd.print(("Abrir todas as"));
    lcd.setCursor(0, 1);
    lcd.print(("Janelas laterais"));
    digitalWrite(37, HIGH);
    return;
  }
  else if (T >= 39.6) {
    lcd.setCursor(0, 0);
    lcd.print(("Abrir janelas la"));
    lcd.setCursor(0, 1);
    lcd.print(("terais e zenitais"));
    digitalWrite(37, HIGH);
    return;
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print(("Erro na leitura T"));
    digitalWrite(36, HIGH);
    return;
  }
}
// Esta função é responsável por criar alertas de acordo com as faixas de umidade
void umidade(float U){
  // Condições especificas para cada faixa de umidade
  if (U <= 73.5){
    lcd.setCursor(0, 0);
    lcd.print(("Alerta:"));
    lcd.setCursor(0, 1);
    lcd.print(("Umidade BAIXA"));
    digitalWrite(35, HIGH);
    return;
  }
  else if (U > 73.5 && U < 114.7){
    lcd.setCursor(0, 0);
    lcd.print(("Umi dentro do"));
    lcd.setCursor(0, 1);
    lcd.print(("esperado"));
    digitalWrite(35, LOW);
    return;
  }
  else if (U >= 114.7){
    lcd.setCursor(0, 0);
    lcd.print(("Alerta:"));
    lcd.setCursor(0, 1);
    lcd.print(("Umidade ALTA"));
    digitalWrite(35, HIGH);
    return;
  }
}
void loop() {
  //Criação de variável com humidade lida pelo sensor
  float U = dht.readHumidity();
  //Criação de variável com temperatura lida pelo sensor
  float T = dht.readTemperature();
  // Verficando erros na leitura do sensor
  digitalWrite(36, LOW);
  if (isnan(T) || isnan(U)) {
    Serial.println(F("Falha na leitura"));
    digitalWrite(36, HIGH);
    return;
  }
  //MOSTRA NO DISPLAY
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
   //horário fictício, será preciso puxar de algum lugar
  lcd.print("Hora: 16:20");
  lcd.setCursor(0, 1);
  lcd.print(("T:"));
  // Código para printar em graus celsius lcd.println(F("°C "));
  lcd.print(T);
  lcd.print(("\nUR:"));
  lcd.print(U);
  delay(3000);
  lcd.clear();
  // chamando a função que lê a temperatura e determina a mensagem
  temperatura(T);
  delay(3000);
  lcd.clear();
  // chamando a função que lê a umidade e determina a mensagem
  umidade(U);
  delay(6000);
}