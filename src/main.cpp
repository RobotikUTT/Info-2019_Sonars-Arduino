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

MCP2515* mcp2515;

SonarArray sonarArray(NB_SONARS, SONAR_ECHO_FRONT_L, SONAR_TRIG_FRONT_L,
                                 SONAR_ECHO_FRONT_R, SONAR_TRIG_FRONT_R,
                                 SONAR_ECHO_R, SONAR_TRIG_R,
                                 SONAR_ECHO_L, SONAR_TRIG_L,
                                 SONAR_ECHO_BACK, SONAR_TRIG_BACK);

void setup() {
  Serial.begin(57600);
  SPI.begin();

}

void loop() {
  sonarArray.update();
}
