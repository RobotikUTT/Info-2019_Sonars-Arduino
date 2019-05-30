#include <Arduino.h>

//includes :
#include "pins.h"
#include "parameters.h"

//libs :
//sonars :
//#include <HC_SR04.h>
//#include <SonarArray.hpp>

//oled i2c screen :
#include <LiquidCrystal_I2C.h>

//CAN :
#include <arduino/serial.hpp>

#include <Ultrasonic.h>

// Sonars functions and objects
void sendSonarsValueToCan();

/*SonarArray sonarArray(NB_SONARS,
  SONAR_TRIG_FRONT_L, SONAR_ECHO_FRONT_L,
  SONAR_TRIG_FRONT_R, SONAR_ECHO_FRONT_R,
  SONAR_TRIG_L, SONAR_ECHO_L,
  SONAR_TRIG_R, SONAR_ECHO_R,
  SONAR_TRIG_BACK, SONAR_ECHO_BACK
);*/

Ultrasonic sonar0(SONAR_TRIG_FRONT_L, SONAR_ECHO_FRONT_L);
Ultrasonic sonar1(SONAR_TRIG_FRONT_R, SONAR_ECHO_FRONT_R);
Ultrasonic sonar2(SONAR_TRIG_BACK_L, SONAR_ECHO_BACK_L);
Ultrasonic sonar3(SONAR_TRIG_BACK_R, SONAR_ECHO_BACK_R);


unsigned long lastMillis = 0;

// Serial handler for frames coming from serial
SerialHandler serialHandler;

// OLED display (width, height, I2C, reset pin = -1 for sharing arduino reset pin)
LiquidCrystal_I2C display(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  // Serial frames
  serialHandler.begin(57600);

  // LCD screen
  display.init();

  // Print a message to the LCD.
  display.backlight();
  display.setCursor(0, 0);
  display.print("waiting for master :)");
}

void loop() {
  //sonarArray.update();

  std::vector<int> frame = serialHandler.read();

	if (serialHandler.is(frame, UPDATE_SCREEN)) {
    // First argument
    int points = frame[1];

    display.setCursor(0, 0);

    display.print("*");

    if (points < 100) {
      display.print("0");
      if (points < 10) {
        display.print("0");
      }
    }

    display.print(points);
    display.print("*        ");

    // Beautitul smileys
    switch (frame[2]) {
    case 0: // booting
      display.print("@_-");
      break;
    case 1: // ready
      display.print(" o/");
      break;
    case 2: // running
      display.print("/o/");
      break;
    case 3: // error
      display.print("x_x");
      break;
    case 4: // done
      display.print(">;)");
      break;
    default:
      display.print("wtf");
      break;
    }
	}

  if (lastMillis + 200 < millis()) {
    sendSonarsValueToCan();
    lastMillis = millis();
  }

}

void sendSonarsValueToCan()
{
  uint8_t measures[4] = {(uint8_t)sonar0.Ranging(CM), (uint8_t)sonar1.Ranging(CM), (uint8_t)sonar2.Ranging(CM), (uint8_t)sonar3.Ranging(CM), };
  serialHandler.send(SONAR_DISTANCE, measures[0], measures[1], measures[2], measures[3], measures[4]);
  /*for (size_t i = 0; i < 4; i++) {
    Serial.print(measures[i]);
    Serial.print("\t");
  }*/
  //Serial.print(measures[3]);
  Serial.flush();
}
