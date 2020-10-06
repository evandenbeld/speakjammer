#include "ports.h"

//FIXME hardware serial (indien beschikbaar)
SoftwareSerial speakjet = SoftwareSerial(0, SPEAKJET_TX);

void setupSpeakjet()
{
  pinMode(SPEAKJET_TX, OUTPUT);
  pinMode(SPEAKJET_SPK, INPUT);

  speakjet.begin(9600);

  pinMode(SPEAKJET_RDY, INPUT);
  pinMode(SPEAKJET_RES, OUTPUT);

  for (int i = SPEAKJET_E0; i <= SPEAKJET_E7; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  resetSpeakjet();
}

inline void resetSpeakjet()
{
  digitalWrite(SPEAKJET_RES, LOW);
  delay(100);
  digitalWrite(SPEAKJET_RES, HIGH);
}

void setSpeakjetVolume(int volume)
{
  speakjet.write(22);
  speakjet.write(volume);
}

boolean speakjetBussy()
{
  return digitalRead(SPEAKJET_SPK);
}

void speakjetWrite(byte controlCodes[])
{
  for (int i = 0; i < NUMBER_OF_ROWS; i++)
  {
    speakjet.write(controlCodes[i]);
  }
}
