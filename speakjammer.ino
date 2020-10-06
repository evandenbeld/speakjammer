#include <U8g2lib.h>
#include <SoftwareSerial.h>
#include <ClickEncoder.h>
#include <TimerOne.h>
#include "ports.h"

#define NUMBER_OF_ROWS 19
#define NUMBER_OF_CHANNELS 4

#define STATE_SELECT_CHANNEL 0
#define STATE_SELECT_ROW 1
#define STATE_SELECT_CONTROL_CODE 2



U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R1, LCD_SCK_CLOCK, LCD_SID_DATA, LCD_CS, U8X8_PIN_NONE);

//FIXME hardware serial (indien beschikbaar)
SoftwareSerial speakjet = SoftwareSerial(0, SPEAKJET_TX);

byte controlCodes[NUMBER_OF_CHANNELS][NUMBER_OF_ROWS] = {
  {220, 221, 222, 223, 224, 225, 226, 227, 228, 229},
  {15, 187, 198, 8, 128, 196, 165, 8, 132, 8, 140, 152},
  {235, 3, 217, 200, 203, 207, 202, 255, 230, 231, 232, 141, 177, 255, 8, 132, 8, 141, 255},
  {235, 3, 217, 200, 203, 207, 202, 255, 230, 231, 232, 141, 177, 255, 8, 132, 8, 141, 255},
};

byte state = STATE_SELECT_CHANNEL;
byte currentRow = 0;
byte currentChannel = 0;

ClickEncoder *encoder;

void timerIsr() {
  encoder->service();
}

void setup()
{
  Serial.begin(9600);

  setupEncoder();
  setupSpeakjet();
  setupLCD();
  setupButtons();
}

void setupEncoder() {
  encoder = new ClickEncoder(ENCODER_PIN_A, ENCODER_PIN_B, ENCODER_CLICK);
  encoder->setAccelerationEnabled(true);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
}

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

void resetSpeakjet()
{
  digitalWrite(SPEAKJET_RES, LOW);
  delay(100);
  digitalWrite(SPEAKJET_RES, HIGH);
}

void setupButtons() {
  pinMode(PLAY_BUTTON_1, INPUT_PULLUP);
  pinMode(PLAY_BUTTON_2, INPUT_PULLUP);
}

void loop(void)
{
  //FIXME handle CV's
  //FIXME volume (20) =  0 - 127
  //FIXME speed (21 )=  0 - 127
  //FIXME pitch (22 )=  0 - 255
  //FIXME bend (23) =  0 - 15
  int volumeCV = map(analogRead(CV_VOLUME), 0, 1023, 0, 255);
  Serial.print("Volume CV:");
  Serial.print(volumeCV);
  Serial.print("\n");
  speakjet.write(22);
  speakjet.write(volumeCV);

  //FIXME handle play buttons
  if (!digitalRead(SPEAKJET_SPK)) {
    //FIXME use array for buttons!
    if (!digitalRead(PLAY_BUTTON_1)) {
      Serial.println("PLAY_BUTTON_1 Button clicked");
      for (int i = 0; i < NUMBER_OF_ROWS; i++)
      {
        speakjet.write(controlCodes[0][i]);
      }

    }
    if (!digitalRead(PLAY_BUTTON_2)) {
      Serial.println("PLAY_BUTTON_2 Button clicked");
      for (int i = 0; i < NUMBER_OF_ROWS; i++)
      {
        speakjet.write(controlCodes[1][i]);
      }
    }
  }
  else
  {
    Serial.println("Speaking");
  }



  //FIXME handle encoder button (state pattern?)
  ClickEncoder::Button button = encoder->getButton();
  if (button == ClickEncoder::DoubleClicked) {
    if (state != STATE_SELECT_CHANNEL) {
      state = STATE_SELECT_CHANNEL;
      xorCell(currentChannel, currentRow);
    }
  }
  //FIXME combine with switch below??
  if (button == ClickEncoder::Clicked) {

    switch (state) {
      case STATE_SELECT_CHANNEL: {
          state = STATE_SELECT_ROW;
          currentRow = 0;
          xorCell(currentChannel, currentRow);
          break;
        }
      case STATE_SELECT_ROW: {
          state = STATE_SELECT_CONTROL_CODE;
          break;
        }
      case STATE_SELECT_CONTROL_CODE: {
          state = STATE_SELECT_ROW;
          break;
        }
    }
    Serial.println("Button clicked, new state: " + state);
  }

  int16_t encoderValue = encoder->getValue();
  switch (state) {
    case STATE_SELECT_CHANNEL: {
        if ((encoderValue < 0 && currentChannel > 0) || (encoderValue > 0 && currentChannel < NUMBER_OF_CHANNELS - 1)) {
          xorChannel(currentChannel);
          currentChannel += encoderValue;
          xorChannel(currentChannel);
        }
        break;
      }
    case STATE_SELECT_ROW: {
        if ((encoderValue < 0 && currentRow > 0) || (encoderValue > 0 && currentRow < NUMBER_OF_ROWS - 1)) {
          xorCell(currentChannel, currentRow);
          currentRow += encoderValue;
          xorCell(currentChannel, currentRow);
        }
        break;
      }
    case STATE_SELECT_CONTROL_CODE: {
        byte controlCode = controlCodes[currentChannel][currentRow];

        if ((encoderValue < 0 && controlCode > 0) || (encoderValue > 0 && controlCode < 255)) {
          controlCode += encoderValue;
          updateCell(currentChannel, currentRow, controlCode);
          controlCodes[currentChannel][currentRow] = controlCode;
        }
        break;
      }

  }

  delay(100);
}
