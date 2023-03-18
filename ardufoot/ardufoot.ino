#include "MIDIUSB.h"

byte botao = 2;
byte botaoEstado = 0;
byte botaoEstadoP = 1;
byte outputValor = 1;
byte midiEstado = 0;
byte midiEstadoP = 0;
unsigned long tempoPressionado = 0;
byte led = 6;
byte outputSegurado = 1;
byte botaoSegurado = 0;


byte pot = A1;
byte mapaPotMidi = 0;
byte mapaPotMidiP = 0;
byte potEstado = 0;
byte potEstadoP = 0;
unsigned long ultimoPot =  0 ;
unsigned long potTimer =  0 ;


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

pinMode(botao, INPUT_PULLUP);

pinMode(led, OUTPUT);


  
}




void loop() {
  // put your main code here, to run repeatedly:
botaoEstado = digitalRead(botao);

if (botaoEstado == 0) {
    if (botaoEstadoP == 1) {
      // O botão acabou de ser pressionado
      tempoPressionado = millis(); // Armazena o tempo atual
    } else {
      // O botão está sendo mantido pressionado
      if (millis() - tempoPressionado > 1000 ) {
        Serial.println("SEGURADO");
        Serial.println(":outputvalor: ");
        Serial.print(outputSegurado);
        outputSegurado++;
        delay(400);

      }if (outputSegurado > 2) {
        outputSegurado = 1;
      }

      
    }
  } else {
    tempoPressionado = 0; // Reinicia o tempo pressionado
  }
//Serial.println(botaoSegurado);
  // Atualiza o estado anterior do botão
  botaoEstadoP = botaoEstado;

if (botaoEstado == 0) { 
  delay(300);
      outputValor++; 
if (outputValor > 2) { 
        outputValor = 1;
      }
    }


  if ((midiEstado == 0)&&(outputValor == 2)) {
    if (tempoPressionado < 25) {
    controlChange(5, 64, 127);
     MidiUSB.flush();

   Serial.println("midi2");

   digitalWrite(led, LOW);
    midiEstado++;
  }
  }
  if ((midiEstado == 1)&&(outputValor == 1)){
    if (tempoPressionado < 25) {
    controlChange(5, 64, 0);
    MidiUSB.flush();

    Serial.println("midi1");

    digitalWrite(led, HIGH);
    midiEstado = 0;
  }
  }
  if((botaoSegurado == 0) && (outputSegurado == 1)) {
    controlChange(5, 67, 0);
    MidiUSB.flush();

    Serial.println("MIDI botaoSegurado1");
    botaoSegurado++;    
  }
  if((botaoSegurado == 1) && (outputSegurado == 2)) {
    controlChange(5, 67, 127);
    MidiUSB.flush();


    Serial.println("MIDI botaoSegurad2");
    botaoSegurado = 0;
  }

  //Serial.println(midiEstado);


potEstado = analogRead(pot);

mapaPotMidi = map(potEstado, 0, 1023, 0, 127);

int potVar = abs(potEstado - potEstadoP);
if(potVar > 25 ) {
  ultimoPot = millis();
}
potTimer = millis() - ultimoPot;

if(potTimer < 350) {
if (mapaPotMidi != mapaPotMidiP) {

controlChange(4, 100, mapaPotMidi);
MidiUSB.flush();

  Serial.print("pot ");
  Serial.print(": ");

  Serial.println(mapaPotMidi);

  mapaPotMidiP = mapaPotMidi;
  potEstadoP = potEstado;
}
}
}





void controlChange(byte channel, byte control, byte value) {

  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};

  MidiUSB.sendMIDI(event);
}