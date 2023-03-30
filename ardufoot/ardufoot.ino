
const int nBotaoPush = 3;
byte botaoPushPin[nBotaoPush] = {2,3,4};
byte botaoPushEstado[nBotaoPush] = {0};
byte botaoPushEstadoP[nBotaoPush] = {0};
byte outputValor[nBotaoPush] = {1};
byte botaoPushVar[nBotaoPush] = {0};


const int nLeds = 3;
byte ledPin[nLeds] = {6,7,8};


const int nPots = 2;
byte potPin[nPots] = {A0,A1};
int potEstado[nPots] = {0};
int potEstadoP[nPots] = {0};
int mapPotEstado[nPots] = {0};
int mapPotEstadoP[nPots] = {0};
unsigned long tempoPerdido[nPots] = {0};
unsigned long potTimer[nPots] = {0};


void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  for(int i = 0; i < nBotaoPush; i++){
    pinMode(botaoPushPin[i], INPUT_PULLUP);
};
for(int i = 0; i < nLeds; i++){
    pinMode(ledPin, OUTPUT);
};
}

void loop() {
  // put your main code here, to run repeatedly:

 for(int i = 0; i < nBotaoPush; i++){
   botaoPushEstado[i] = digitalRead(botaoPushPin[i]);
   
if((botaoPushEstado[i] == 0) && (botaoPushEstadoP[i] == 1)) {
 delay(50);
  
  outputValor[i]++;
}if(outputValor[i]>2){
  outputValor[i] = 1;
}

botaoPushEstadoP[i] = botaoPushEstado[i];

if((outputValor[i] == 1) && (botaoPushVar[i] == 0)){
botaoPushVar[i]++; 
Serial.println("nota ON");
digitalWrite(ledPin[i], LOW);
}
if((outputValor[i] == 2) && (botaoPushVar[i] == 1)){
  Serial.println("nota OFF");
  botaoPushVar[i] = 0;
digitalWrite(ledPin[i], HIGH);
}

 }
//------------------POTS POTS POTS-----------------

for(int i = 0; i < nPots; i++){
potEstado[i] = analogRead(potPin[i]);

mapPotEstado[i] = map(potEstado[i], 0, 1023, 0, 127);

int potVar = abs(potEstado[i] - potEstadoP[i]);
if (potVar > 35){
tempoPerdido[i] = millis();  
}
potTimer[i] = millis() - tempoPerdido[i];

if(potTimer[i] < 300){
if(mapPotEstado[i] != mapPotEstadoP[i]){
  Serial.println(mapPotEstado[i]);
}
potEstadoP[i] = potEstado[i];
mapPotEstadoP[i] = mapPotEstado[i];
}

}
}


