/*
   this is code for the use of air pressure MXP5050GP.
*/
#include "Keyboard.h"

const int togglePin = 4;          // input pin for toggle switch
int sensorPin = A0;
int sensorValue;
int toggleState = 1;
int previousPuffState = 0;   // for checking the state of the sensor
int previousSipState = 0;
int counter = 0;                  // button push counter
int sipState;
int puffState;
char character = 32;       // currently displayed character
int timestampPuff;          // timestamp
int timestampSip;          // timestamp
int pace = 1000;        // sip, speed of the selection
int space = 1000;        // puff, next
//bool space = false;
//const float SensorOffset = 102.0;

void setup() {
  Serial.begin(9600);
  // make the pushButton pin an input:
  pinMode(togglePin, INPUT);
  // initialize control over the keyboard:
  Keyboard.begin();
}

void loop() {
  //  toggleState = digitalRead(togglePin);
  //  if (toggleState == 0) {
  //    Keyboard.begin();
  //  } else if (toggleState == 1) {
  //    Keyboard.end();
  //  }

  sensorValue = analogRead(sensorPin);

  if (sensorValue >= 50) {
    if (sensorValue >= 170) {
      puffState = 1;
    } else if (sensorValue < 170) {
      puffState = 0;
    }
  }

  if (sensorValue < 50) {
    if (sensorValue <= 28 ) {
      sipState = 1;
    } else if (sensorValue > 28) {
      sipState = 0;
    }
  }

  if (puffState != previousPuffState) {
    //for next
    delay(10);
    if (puffState == 1) {
      Keyboard.write(8);
      Keyboard.write(character);
      Keyboard.write(32);
      timestampPuff = millis();
    }
    previousPuffState = puffState;
  }


  // for space
  if (puffState == 1) {
    // after the delay passes:
    if (millis() - timestampPuff > space) {
      delay(100);
      Keyboard.write(32);
      //      delay(250);
      //      Keyboard.write(32);
      //      delay(1000);
    }
  }

  // if the sensor state has changed,
  if (sipState != previousSipState) {
    delay(10);
    // it's currently blown:
    if (sipState == 1) {
//      if (millis() - timestampSip < pace) {
        character ++;
        Keyboard.write(8);
        Keyboard.write(character);
        if (character == 128) {
          character = 32;
          timestampSip = millis();
        }
        delay(150);
//      } else 
//      if (millis() - timestampSip >= pace) {
//          character ++;
//          Keyboard.write(8);
//          Keyboard.write(character);
//          if (character == 128) {
//            character = 32;
//          }
//        }
      }
      // there are several edge cases:
      switch (character) {
        case 0X1F:        // unprintables below space in ASCII table
          // replace with tab instead:
          character = '\t';
          Keyboard.write(character);
          break;
        case '\t':        // tab:
          // since last character was tab, replace with delete:
          Keyboard.write(KEY_DELETE);
          // reset current character:
          //        character = 'a';
          break;
        case 127:         // DEL
          // print newline and carriage return instead:
          Keyboard.println();
          break;
          //      default:
          // delete, then print character, then advance
          //        Keyboard.write(8);
          //        Keyboard.write(character);
          //        break;
      }
      // constrain character to between tab and ~:
      character =  constrain(character, '\t', 127);
      previousSipState = sipState;
    }

    Serial.print(sensorValue);
    Serial.print(",");
    Serial.println(character);
    //  Serial.println(toggleState);
    // save the current button state for comparison next time:
  }
