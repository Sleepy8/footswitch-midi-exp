#include <Control_Surface.h>

#include "MIDIUSB.h"
USBMIDI_Interface midi;

const int nBotaoPush = 4; // botao momentaneo
byte botaoPushPin[nBotaoPush] = { 2,3,4,5};
byte botaoPushEstado[nBotaoPush] = {0};
byte botaoPushEstadoP[nBotaoPush] = {0};
byte botaoPushMidiN[nBotaoPush] = {64,65,66,67};
byte botaoSegMidiN[nBotaoPush] = {68,69,70,71};
byte botaoSeg[nBotaoPush] = {0};
byte outputSeg[nBotaoPush] = {1};

const int nBotaoToggle = 1; //botao com trava 
byte botaoTogglePin[nBotaoToggle] = {14};
byte botaoToggleEstado[nBotaoToggle] = {0};
byte botaoToggleMidiN[nBotaoToggle] = {90};
byte botaoToggleEstadoP[nBotaoToggle] = {0};

byte botaoChannel = {5}; //canal midi (0 à 17)
byte potChannel = {7}; //canal midi (0 à 17)

byte outputSegurado[nBotaoPush] = {1};
byte outputValor[nBotaoPush] = {1};
byte midiEstado[nBotaoPush] = {0};

byte tempoPressionado[nBotaoPush]={0};

const int nLed = 4; // leds para os botoes momentaneos ( botao com trava pode acender só com o botao...)
byte ledPin[nLed] = {6,7,8,9};


CCPotentiometer pot2 {
   A1, //Pino do Potênciometro 
  {MIDI_CC::Channel_Volume, CHANNEL_1}, 
};
CCPotentiometer pot1 {
   A0, //Pino do Potênciometro */
  {MIDI_CC::Channel_Volume, CHANNEL_1}, 
};

void setup() {
  // put your setup code here, to run once:

  Control_Surface.begin();

Serial.begin(115200);

  for(int i = 0; i < nBotaoPush; i++){
    pinMode(botaoPushPin[i], INPUT_PULLUP);
  };

  for(int i = 0; i < nBotaoToggle; i++){
    pinMode(botaoTogglePin[i], INPUT_PULLUP);
  };

for(int i = 0; i < nLed; i++){
  pinMode(ledPin[i], OUTPUT);
};
}

void loop() {
  // put your main code here, to run repeatedly:

Control_Surface.loop(); 

for (int i = 0; i < nBotaoPush; i++){
botaoPushEstado[i] = digitalRead(botaoPushPin[i]);

if (botaoPushEstado[i] == 0) {
    if (botaoPushEstadoP[i] == 1) {
      tempoPressionado[i] = millis(); // Armazena o tempo atual
    } else {
      // O botão está sendo mantido pressionado
      if (millis() - tempoPressionado[i] > 1000 ) {
        Serial.println("SEGURADO");
        Serial.println(":outputvalor: ");
        Serial.print(outputSeg[i]);
        outputSeg[i]++;
        delay(400);
        

      }if (outputSeg[i] > 2) {
        outputSeg[i] = 1;
        
        
      }

      
    }
  } else {
    tempoPressionado[i] = 0; // Reinicia o tempo pressionado
  }
//Serial.println(botaoSegurado);
  // Atualiza o estado anterior do botão
  botaoPushEstadoP[i] = botaoPushEstado[i];

  if (botaoPushEstado[i] == 0) { 
  delay(300);
      outputValor[i]++; 
if (outputValor[i] > 2) { 
        outputValor[i] = 1;
      }
    }


  if ((midiEstado[i] == 0)&&(outputValor[i] == 2)) {
    if (tempoPressionado[i] < 25 ) {
    controlChange(botaoChannel, botaoPushMidiN[i], 127); // MANDAR MIDI
     MidiUSB.flush();

   Serial.println("midi2");

   digitalWrite(ledPin[i], LOW);
    midiEstado[i]++;
    
  }
  }
  if ((midiEstado[i] == 1)&&(outputValor[i] == 1)){
    if (tempoPressionado[i] < 25) {
    controlChange(botaoChannel, botaoPushMidiN[i], 0);
    MidiUSB.flush();

    Serial.println("midi1");

    digitalWrite(ledPin[i], HIGH);
    midiEstado[i] = 0;
  }
  }

if((botaoSeg[i] == 0) && (outputSeg[i] == 1)) {
    controlChange(botaoChannel, botaoSegMidiN[i], 0);
    MidiUSB.flush();

    Serial.println("MIDI botaoSegurado1");
    botaoSeg[i]++;
    delay(100);    
  }
  if((botaoSeg[i] == 1) && (outputSeg[i] == 2)) {
    controlChange(botaoChannel, botaoSegMidiN[i], 127);
    MidiUSB.flush();


    Serial.println("MIDI botaoSegurad2");
    botaoSeg[i] = 0;
    delay(100); 
  }


}//i botao push

for (int i = 0; i < nBotaoToggle; i++){
botaoToggleEstado[i] = digitalRead(botaoTogglePin[i]);

if (botaoToggleEstado[i] != botaoToggleEstadoP[i]) {
if(botaoToggleEstado[i] == 0){
  controlChange(botaoChannel,botaoToggleMidiN[i], 127);
MidiUSB.flush();
Serial.println("botao toggle nota on");
  delay(200);
} else {
  controlChange(botaoChannel,botaoToggleMidiN[i], 0);
MidiUSB.flush();
Serial.println("botao toggle nota off");
  delay(200);
}
}
botaoToggleEstadoP[i] = botaoToggleEstado[i];
};


}


//loop

//-----------------------------------------------------------------

void controlChange(byte channel, byte control, byte value) {

  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};

  MidiUSB.sendMIDI(event);
}
