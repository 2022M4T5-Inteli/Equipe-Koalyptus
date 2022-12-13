//Importação da biblioteca do DisplayLCD
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); //Criação de um objeto LCD

//Variáveis usadas para habilitar os símbolos "%" e "°" no Display
byte percent[8] = {
  B00000,
  B00000,
  B11001,
  B11010,
  B00100,
  B01011,
  B10011,
  B00000
};

byte celcius[8] = {
  B00000,
  B00000,
  B10110,
  B01001,
  B01000,
  B01000,
  B01001,
  B00110
};

//Função para iniciar o display
void lcdSetup(){
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, percent); //Criação do caractere "%" para utilizar no Display
  lcd.createChar(1, celcius); //Criação do caractere "°" para utilizar no Display
}

// Função que para mostrar os dados de horário, umidade relativa e temperatura no display
void currentStatus(float t, float u){
  lcdSetup(); //Função para iniciar o display
  lcd.setCursor(1, 0);
  lcd.print("Hora: 16:20"); //horário fictício, terá que puxar de algum lugar
  lcd.setCursor(0, 1);
  lcd.print(("T:"));
  lcd.print(t);
  lcd.print(F("C"));
  lcd.print(("\nUR:"));
  lcd.print(u);
  return;
}

// função com as condições para cada faixa de temperatura
int temp(float t){
  if (t > 0 && t <= 26.6){
    lcd.setCursor(0, 0);
    lcd.print(("Alerta:"));
    lcd.setCursor(0, 1);
    lcd.print(("Temp. BAIXA"));
    return 1;
  }
  else if (t > 26.6 && t < 37.08){
    lcd.setCursor(0, 0);
    lcd.print(("Temperatura:"));
    lcd.setCursor(0, 1);
    lcd.print(("dentro do esperado"));
    return 0;
  }
  else if (t >= 37.08 && t < 37.8) {
    lcd.setCursor(0, 0);
    lcd.print(("Alerta:"));
    lcd.setCursor(0, 1);
    lcd.print(("Temperatura ALTA"));
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(("Abrir 50% das"));
    lcd.setCursor(0, 1);
    lcd.print(("janelas laterais"));
    return 1;
  }
  else if (t >= 37.8 && t < 39.6) {
    lcd.setCursor(0, 0);
    lcd.print(("Alerta:"));
    lcd.setCursor(0, 1);
    lcd.print(("Temperatura ALTA"));
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(("Abrir 100% das"));
    lcd.setCursor(0, 1);
    lcd.print(("janelas laterais"));
    return 1;
  }
  else if (t >= 39.6) {
    lcd.setCursor(0, 0);
    lcd.print(("Alerta: Temperatura alta"));
    lcd.setCursor(0, 1);
    lcd.print(("Abrir janelas zenitais e laterais"));
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(("Abrir janelas"));
    lcd.setCursor(0, 1);
    lcd.print(("zenitais e laterais"));
    return 1;
  }
   else if (t <= 0){
    lcd.setCursor(0, 0);
    lcd.print(("Alerta: Erro na leitura"));
    lcd.setCursor(0, 1);
    lcd.print(("da temperatura"));
    return 2;
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print(("Alerta: Erro na leitura"));
    lcd.setCursor(0, 1);
    lcd.print(("da temperatura"));
    return 2;
  }
}

// função com as condições para cada faixa de umidade
int umi(float u){
  if (u > 0 && u <= 73.5){
    lcd.setCursor(0, 0);
    lcd.print(("Alerta: Umidade"));
    lcd.setCursor(0, 1);
    lcd.print(("relativa BAIXA"));
    return 1;
  }
  else if (u > 73.5 && u < 95){
    lcd.setCursor(0, 0);
    lcd.print(("Umidade relativa:"));
    lcd.setCursor(0, 1);
    lcd.print(("dentro do ideal"));
    return 0;
  }
  else if (u >= 95){
    lcd.setCursor(0, 0);
    lcd.print(("Alerta: Umidade"));
    lcd.setCursor(0, 1);
    lcd.print(("relativa ALTA"));
    return 1;
  }
  else if (u <= 0){
    lcd.setCursor(0, 0);
    lcd.print(("Alerta: Erro na leitura"));
    return 2;
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print(("Alerta: Erro na leitura"));
    lcd.setCursor(0, 1);
    lcd.print(("da umidade relativa"));
    return 2;
  }
}

void lcdClear(){
  delay(3000);
  lcd.clear();
}