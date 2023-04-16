#include <MIDIUSB.h>


 

const int nBotaoPush = 4;
byte botaoPushPin[nBotaoPush] = {2,3,4,5};
byte botaoPushEstado[nBotaoPush] = {0};
byte botaoPushEstadoP[nBotaoPush] = {0};
byte outputValor[nBotaoPush] = {1};
byte botaoPushVar[nBotaoPush] = {0};
byte seguradoVar[nBotaoPush] = {0};
byte outputSegurado[nBotaoPush] = {1};
unsigned long tempoSegurado[nBotaoPush] = {0};
byte notaMidiPushBotao[nBotaoPush] = {64,65,66,67};
byte pcSegurado[nBotaoPush] = {00,01,02,03};

const int nBotaoToggle = 1;
byte botaoTogglePin[nBotaoToggle] = {14};
byte botaoToggleEstado[nBotaoToggle] = {0};
byte botaoToggleEstadoP[nBotaoToggle] = {0};
byte notaMidiToggleBotao[nBotaoToggle] = {70};


const int nLeds = 4;
byte ledPin[nLeds] = {6,7,8,9};

byte channel = 5;


const int nPots = 2;
byte potPin[nPots] = {A0,A1};
int potEstado[nPots] = {0};
int potEstadoP[nPots] = {0};
int mapPotEstado[nPots] = {0};
int mapPotEstadoP[nPots] = {0};
unsigned long tempoPerdido[nPots] = {0};
unsigned long potTimer[nPots] = {0};
byte notaMidiPot[nPots] = {100,101};

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  for(int i = 0; i < nBotaoPush; i++){
    pinMode(botaoPushPin[i], INPUT_PULLUP);
};
for(int i = 0; i < nBotaoToggle; i++){
  pinMode(botaoTogglePin[i], INPUT_PULLUP);
};
for(int i = 0; i < nLeds; i++){
    pinMode(ledPin[i], OUTPUT);
};
}

void loop() {
  // put your main code here, to run repeatedly:

 for(int i = 0; i < nBotaoPush; i++){
  botaoPushEstado[i] = digitalRead(botaoPushPin[i]);

  if((botaoPushEstado[i] == 0) && (botaoPushEstadoP[i] == 1)) {
    tempoSegurado[i] = millis();
if((botaoPushEstado[i] == 0) && (botaoPushEstadoP[i] == 1) && (millis() - tempoSegurado[i] < 25)){
  delay(50);
    outputValor[i]++;
  }
    // Botão pressionado
    
  } else if ((botaoPushEstado[i] == 1) && (botaoPushEstadoP[i] == 0)) {
    // Botão solto
    if (millis() - tempoSegurado[i] >= 1000) {
      // Botão segurado por tempo suficiente
      outputSegurado[i]++;
      Serial.println("segurado");
    } if(outputSegurado[i] > 2){
      outputSegurado[i] = 1;
    }
  }

  if(outputValor[i]>2){
    outputValor[i] = 1;
  }

  botaoPushEstadoP[i] = botaoPushEstado[i];

  if((outputValor[i] == 1) && (botaoPushVar[i] == 0)){
    botaoPushVar[i]++; 
    controlChange(channel, notaMidiPushBotao[i], 127);
    MidiUSB.flush();
    Serial.println("nota ON");
    digitalWrite(ledPin[i], LOW);
  }
  if((outputValor[i] == 2) && (botaoPushVar[i] == 1)){
    controlChange(channel, notaMidiPushBotao[i], 0);
    MidiUSB.flush();
    Serial.println("nota OFF");
    botaoPushVar[i] = 0;
    digitalWrite(ledPin[i], HIGH);
  }
if((outputSegurado[i] == 1) && (seguradoVar[i] == 0 )){
  programChange(channel, pcSegurado[i]);
  MidiUSB.flush();
  seguradoVar[i]++;
  } else if((outputSegurado[i] == 2) && (seguradoVar[i] == 1 )){
     programChange(channel, pcSegurado[i]);
  MidiUSB.flush();
  seguradoVar[i] = 0;

  }

}

// -------- Botao Toggle--------------------
 for(int i = 0; i < nBotaoToggle; i++){
   botaoToggleEstado[i] = digitalRead(botaoTogglePin[i]);

if(botaoToggleEstado[i] != botaoToggleEstadoP[i]){
  if(botaoToggleEstado[i] == 0){
    Serial.println("botao togglen on");
controlChange(channel, notaMidiToggleBotao[i], 127);
MidiUSB.flush();
 delay(50);
}
if(botaoToggleEstado[i] == 1){
  Serial.println("botao togglen off");
controlChange(channel, notaMidiToggleBotao[i], 0);
MidiUSB.flush();
delay(50);
};
 botaoToggleEstadoP[i] = botaoToggleEstado[i];
 }
 };

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
  controlChange(channel, notaMidiPot[i], mapPotEstado[i]);
MidiUSB.flush();
  Serial.println(mapPotEstado[i]);
}
potEstadoP[i] = potEstado[i];
mapPotEstadoP[i] = mapPotEstado[i];
}

}
}


void controlChange(byte channel, byte control, byte value) {

  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};

  MidiUSB.sendMIDI(event);
}

void programChange(byte channel, byte program) {
  midiEventPacket_t pc = {0x0C, 0xC0 | channel, program, 0};
  MidiUSB.sendMIDI(pc);
}