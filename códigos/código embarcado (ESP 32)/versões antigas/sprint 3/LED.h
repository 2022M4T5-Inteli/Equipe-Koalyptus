//Definição das portas dos LEDs
#define tempLED 37
#define umiLED 36
#define statusLEDR 19
#define statusLEDG 20
#define statusLEDB 21

void ledSetup(){
  pinMode(tempLED, OUTPUT); //LED de temperatura
  pinMode(umiLED, OUTPUT); //LED de umidade
  pinMode(statusLEDR, OUTPUT); //LED de vermelho do RGB de status
  pinMode(statusLEDG, OUTPUT); //LED de verde do RGB de status
  pinMode(statusLEDB, OUTPUT); //LED de azul do RGB de status
}

//Ascende os LEDs conforme o status de temperatura, umidade e erro
void changeLED(int t, int u){
  if(t == 1){
    digitalWrite(tempLED, HIGH);
  }
  if(u == 1){
    digitalWrite(umiLED, HIGH);
  }
  if(t == 2 || u == 2){
    digitalWrite(statusLEDR, HIGH);
    digitalWrite(statusLEDG, HIGH);
    digitalWrite(statusLEDB, HIGH);
    delay(200);
    digitalWrite(statusLEDR, LOW);
    digitalWrite(statusLEDG, LOW);
    digitalWrite(statusLEDB, HIGH);
  }
  if(t <= 1 && u <= 1){
    digitalWrite(statusLEDR, HIGH);
    digitalWrite(statusLEDG, LOW);
    digitalWrite(statusLEDB, HIGH);
  }
}

//Apaga todos os LEDs
void resetLED(){
  digitalWrite(tempLED, LOW);
  digitalWrite(umiLED, LOW);
  delay(200);
}