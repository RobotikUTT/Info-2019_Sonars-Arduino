#include <Arduino.h>

//includes :
#include "pins.h"
#include "parameters.h"

//libs :
//sonars :
#include <HC_SR04.h>
#include <SonarArray.hpp>

//oled i2c screen :
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//CAN :
#include <arduino/serial.hpp>

void sendSonarsValueToCan();

SonarArray sonarArray(NB_SONARS, SONAR_ECHO_FRONT_L, SONAR_TRIG_FRONT_L,
                                 SONAR_ECHO_FRONT_R, SONAR_TRIG_FRONT_R,
                                 SONAR_ECHO_L, SONAR_TRIG_L,
                                 SONAR_ECHO_R, SONAR_TRIG_R,
                                 SONAR_ECHO_BACK, SONAR_TRIG_BACK);

SerialHandler serial;
unsigned long lastMillis = 0;

void setup() {
  Serial.begin(57600);
  SPI.begin();

  serial.setup();
}

void loop() {
  sonarArray.update();

  auto frame = serial.read();
	if (serial.is(frame, UPDATE_SCREEN)) {
    // First argument
    int points = frame[1];

		// TODO : Set screen points here
	}

  if (lastMillis + 200 < millis()) {
    sendSonarsValueToCan();
    lastMillis = millis();
  }
}

void sendSonarsValueToCan()
{
  std::vector<uint8_t> measures = sonarArray.getDistancesCM();
  serial.send(SONAR_DISTANCE, measures[0], measures[1], measures[2], measures[3], measures[4]);
}
