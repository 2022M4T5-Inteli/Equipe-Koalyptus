// Definição das portas dos LEDs
#define tempLED 37
#define umiLED 36
#define statusLEDR 19
#define statusLEDG 20
#define statusLEDB 21

int ledTimer = 300;

void ledSetup()
{
  pinMode(tempLED, OUTPUT);    // LED de temperatura
  pinMode(umiLED, OUTPUT);     // LED de umidade
  pinMode(statusLEDR, OUTPUT); // LED de vermelho do RGB de status
  pinMode(statusLEDG, OUTPUT); // LED de verde do RGB de status
  pinMode(statusLEDB, OUTPUT); // LED de azul do RGB de status
}

// Acende os LEDs conforme o status de temperatura, umidade e erro
void changeLED(int t, int u, int timer)
{
  // Condição que define o estado do LED para status de temperatura
  if (t == 1)
  {
    if (int((timer / ledTimer)) % 2 == 1)
    {
      digitalWrite(tempLED, HIGH);
    }
    else
    {
      digitalWrite(tempLED, LOW);
    }
  }

  // Condição que define o estado do LED para status de umidade
  if (u == 1)
  {
    if (int((timer / ledTimer)) % 2 == 1)
    {
      digitalWrite(umiLED, HIGH);
    }
    else
    {
      digitalWrite(umiLED, LOW);
    }
  }

  // Condição que define o estado do LED RGB
  if (t == 2 || u == 2)
  {
    if (int((timer / ledTimer)) % 2 == 1)
    {
      digitalWrite(statusLEDR, LOW);
      digitalWrite(statusLEDG, LOW);
      digitalWrite(statusLEDB, LOW);
    }
    else
    {
      digitalWrite(statusLEDR, HIGH);
      digitalWrite(statusLEDG, HIGH);
      digitalWrite(statusLEDB, LOW);
    }
  }
  if (t <= 1 && u <= 1)
  {
    digitalWrite(statusLEDR, LOW);
    digitalWrite(statusLEDG, HIGH);
    digitalWrite(statusLEDB, LOW);
  }
}

// Função que apaga todos os LEDs
void resetLED()
{
  digitalWrite(tempLED, LOW);
  digitalWrite(umiLED, LOW);
}