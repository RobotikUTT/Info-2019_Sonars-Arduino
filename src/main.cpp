#include <Arduino.h>

//includes :
#include "pins.h"
#include "parameters.h"

//libs :
//sonars :
#include <HC_SR04.h>
#include <SonarArray.h>

//oled i2c screen :
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//CAN :
#include <SPI.h>
#include <mcp2515.h>
#include <arduino/can.hpp>

void sendSonarsValueToCan();

MCP2515* mcp2515;

SonarArray sonarArray(NB_SONARS, SONAR_ECHO_FRONT_L, SONAR_TRIG_FRONT_L,
                                 SONAR_ECHO_FRONT_R, SONAR_TRIG_FRONT_R,
                                 SONAR_ECHO_L, SONAR_TRIG_L,
                                 SONAR_ECHO_R, SONAR_TRIG_R,
                                 SONAR_ECHO_BACK, SONAR_TRIG_BACK);

CanHandler can;


void setup() {
  Serial.begin(57600);
  SPI.begin();

  can.setup();
}

void loop() {
  sonarArray.update();
  //measures = sonarArray.getDistancesCM();
  //Serial.println(measures[0]);

  auto frame = can.read();

	if (can.is(frame, SONAR_DISTANCE)) {
		// CODE HERE
	}

  Serial.println("coucou");
}

void sendSonarsValueToCan()
{
  std::vector<uint8_t> measures = sonarArray.getDistancesCM();
  can.send(SONAR_DISTANCE, measures[0], measures[1], measures[2], measures[3], measures[4]);
}
