#include "MIDIUSB.h"
byte botao = 2;
byte botaoEstado = 0;
byte botaoEstadoP = 0;
byte outputValor = 1;
byte midiEstado = 0;
byte midiEstadoP = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

pinMode(botao, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
botaoEstado = digitalRead(botao);

if (botaoEstado == 0) { 
  delay(250);
      outputValor++; 
if (outputValor > 2) { 
        outputValor = 1;
      }
      Serial.print("o valor e");
      Serial.println(outputValor); 
    }


  if ((midiEstado == 0)&&(outputValor == 2)) {
    controlChange(5,64,127);
    MidiUSB.flush();
    midiEstado++;
  }
  if ((midiEstado == 1)&&(outputValor == 1)){
    controlChange(5,64,0);
    MidiUSB.flush();
    midiEstado = 0;
  }

  //Serial.println(midiEstado);
} 



void controlChange(byte channel, byte control, byte value) {

  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};

  MidiUSB.sendMIDI(event);
}