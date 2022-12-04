// Importação da biblioteca do DisplayLCD
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // CriaÃ§Ã£o de um objeto LCD

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;

int lcdTimer = 3000;
int lcdState = 0;
int lcdChange = 1;
int lcdClearer = 0;

// Função para controlar o fluxo das mensagens no LCD
int currentLcd(int timer)
{
  if (timer >= lcdTimer)
  {
    lcdClearer = 1;
    lcdState++;
    if (lcdState > 4)
    {
      lcdState = 0;
    }
    lcdChange = 1;
    return 0;
  }
  return timer;
}

// Função para iniciar o display e configurar o horário
void lcdSetup()
{
  lcd.init();
  lcd.backlight();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); // Configurações para pegar as informações temporais
}

// Função que para mostrar os dados de horário, umidade relativa e temperatura no display
void currentStatus(float t, float u, int timer)
{
  if (lcdState == 0 && lcdChange == 1)
  {
    struct tm timeinfo;
    // Serial.println(&timeinfo, "%H");
    // Serial.println(&timeinfo, "%M");
    lcdSetup(); // Função para iniciar o display
    lcd.setCursor(1, 0);
    lcd.print("Hora: ");
    lcd.print(&timeinfo, "%I");
    lcd.print(":");
    lcd.print(&timeinfo, "%M");
    lcd.setCursor(0, 1);
    lcd.print(("T:"));
    lcd.print(t);
    lcd.print(F("C"));
    lcd.print((" UR:"));
    lcd.print(u);
    lcdChange = 0;
  }
  return;
}

// Função com as condições para cada faixa de temperatura
// Dependendo da temperatura lida, vai mostrar uma mensagem específica
int temp(float t, int timer, int maxTemperature, int minTemperature)
{
  if (t > 0 && t <= (minTemperature * 0.95))
  {
    if (lcdState == 1 && lcdChange == 1)
    {
      lcd.setCursor(0, 0);
      lcd.print(("Alerta:"));
      lcd.setCursor(0, 1);
      lcd.print(("Temp. BAIXA"));
      lcdChange = 0;
      lcdState++;
    }
    return 1;
  }
  else if (t > (minTemperature * 0.95) && t < (maxTemperature * 1.03))
  {
    if (lcdState == 1 && lcdChange == 1)
    {
      lcd.setCursor(0, 0);
      lcd.print(("Temperatura: "));
      lcd.setCursor(0, 1);
      lcd.print(("dentro do esperado"));
      lcdChange = 0;
      lcdState++;
    }
    return 0;
  }
  else if (t >= (maxTemperature * 1.03) && t < (maxTemperature * 1.05))
  {
    if (lcdState == 1 && lcdChange == 1)
    {
      lcd.setCursor(0, 0);
      lcd.print(("Alerta:"));
      lcd.setCursor(0, 1);
      lcd.print(("Temperatura ALTA"));
      lcdChange = 0;
    }
    if (lcdState == 2 && lcdChange == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(("Abrir 50% das"));
      lcd.setCursor(0, 1);
      lcd.print(("janelas laterais"));
      lcdChange = 0;
    }
    return 1;
  }
  else if (t >= (maxTemperature * 1.05) && t < (maxTemperature * 1.1))
  {
    if (lcdState == 1 && lcdChange == 1)
    {
      lcd.setCursor(0, 0);
      lcd.print(("Alerta:"));
      lcd.setCursor(0, 1);
      lcd.print(("Temperatura ALTA"));
      lcdChange = 0;
    }
    if (lcdState == 2 && lcdChange == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(("Abrir 100% das"));
      lcd.setCursor(0, 1);
      lcd.print(("janelas laterais"));
      lcdChange = 0;
    }
    return 1;
  }
  else if (t >= (maxTemperature * 1.1))
  {
    if (lcdState == 1 && lcdChange == 1)
    {
      lcd.setCursor(0, 0);
      lcd.print(("Alerta:"));
      lcd.setCursor(0, 1);
      lcd.print(("Temperatura ALTA"));
      lcdChange = 0;
    }
    if (lcdState == 2 && lcdChange == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(("Abrir janelas"));
      lcd.setCursor(0, 1);
      lcd.print(("zenitais e laterais"));
      lcdChange = 0;
    }
    return 1;
  }
  else if (t <= 0)
  {
    if (lcdState == 1 && lcdChange == 1)
    {
      lcd.setCursor(0, 0);
      lcd.print(("Alerta: Erro"));
      lcd.setCursor(0, 1);
      lcd.print(("na leitura"));
      lcdChange = 0;
      lcdState++;
    }
    return 2;
  }
  else
  {
    if (lcdState == 1 && lcdChange == 1)
    {
      lcd.setCursor(0, 0);
      lcd.print(("Alerta: Erro"));
      lcd.setCursor(0, 1);
      lcd.print(("na leitura"));
      lcdChange = 0;
      lcdState++;
    }
    return 2;
  }
}

// Função com as condições para cada faixa de umidade
int umi(float u, int timer, int maxUmidity, int minUmidity)
{
  if (u > 0 && u <= (minUmidity * 0.95))
  {
    if (lcdState == 3 && lcdChange == 1)
    {
      lcd.setCursor(0, 0);
      lcd.print(("Alerta: Umidade"));
      lcd.setCursor(0, 1);
      lcd.print(("relativa BAIXA"));
      lcdChange = 0;
      lcdState++;
    }
    return 1;
  }
  else if (u > (minUmidity * 0.95) && u < maxUmidity)
  {
    if (lcdState == 3 && lcdChange == 1)
    {
      lcd.setCursor(0, 0);
      lcd.print(("Umidade relativa:"));
      lcd.setCursor(0, 1);
      lcd.print(("dentro do ideal"));
      lcdChange = 0;
      lcdState++;
    }
    return 0;
  }
  else if (u >= maxUmidity)
  {
    if (lcdState == 3 && lcdChange == 1)
    {
      lcd.setCursor(0, 0);
      lcd.print(("Alerta: Umidade"));
      lcd.setCursor(0, 1);
      lcd.print(("relativa ALTA"));
      lcdChange = 0;
      lcdState++;
    }
    return 1;
  }
  else if (u <= 0)
  {
    if (lcdState == 3 && lcdChange == 1)
    {
      lcd.setCursor(0, 0);
      lcd.print(("Alerta: Erro na leitura"));
      lcdChange = 0;
      lcdState++;
    }
    return 2;
  }
  else
  {
    if (lcdState == 3 && lcdChange == 1)
    {
      lcd.setCursor(0, 0);
      lcd.print(("Alerta:"));
      lcd.setCursor(0, 1);
      lcd.print(("Erro na leitura"));
      lcdChange = 0;
      lcdState++;
    }
    return 2;
  }
}

// Reset do LCD
void lcdClear()
{
  if (lcdClearer == 1)
  {
    lcd.clear();
    lcdClearer = 0;
  }
}

// Função chamada quando há um erro na conexão com o wifi
wifiError()
{
  lcd.setCursor(0, 0);
  lcd.print(("ERR_03"));
  lcd.setCursor(0, 1);
  lcd.print(("Erro no sensor"));
  delay(5000);
}

// Função chamada quando há um erro na conexão com o sensor
sensorError()
{
  lcd.setCursor(0, 0);
  lcd.print(("ERR_01"));
  lcd.setCursor(0, 1);
  lcd.print(("Erro no sensor"));
  delay(5000);
}

// Função chamada quando há um erro na calibração do sensor
sensorReadError()
{
  lcd.setCursor(0, 0);
  lcd.print(("ERR_02"));
  lcd.setCursor(0, 1);
  lcd.print(("Erro no sensor"));
  delay(5000);
}

// Função chamada quando há um erro no envio de dados
ifttError()
{
  lcd.setCursor(0, 0);
  lcd.print(("ERR_04"));
  lcd.setCursor(0, 1);
  lcd.print(("Envio de dados"));
  delay(5000);
}