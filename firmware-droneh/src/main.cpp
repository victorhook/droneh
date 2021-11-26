#include "sys.h"
#include "drivers/radio.h"
#include "drivers/baro.h"
#include "drivers/mpu.h"
#include "drivers/tof.h"
#include "drivers/leds.h"
#include "drivers/motors.h"

#include "log.h"
#include "controller.h"
#include "estimator.h"


void initSystem()
{
  // Serial
  Serial.begin(9600);
  Serial.println("Booting up...");

  // I2C
  SYS_I2C.setSCL(SYS_I2C_SCL);
  SYS_I2C.setSDA(SYS_I2C_SDA);
  SYS_I2C.begin();

  // SPI
  SYS_RADIO_SPI.setCS(SYS_RADIO_CSN);
  SYS_RADIO_SPI.setRX(SYS_RADIO_MISO);
  SYS_RADIO_SPI.setTX(SYS_RADIO_MOSI);
  SYS_RADIO_SPI.setSCK(SYS_RADIO_SCK);
  SYS_RADIO_SPI.begin();

  // Initialize drivers
  while (!baro.init()) { Serial.println("Failed to init Baro :("); }
  while (!mpu.init()) { Serial.println("Failed to init MPU :("); }
  while (!tof.init()) { Serial.println("Failed to init ToF :("); }
  while (!radio.init()) { Serial.println("RF not working"); }
  while (!led_control.init()) { Serial.println("Led driver not working"); }
  while (!motor_control.init()) { Serial.println("Motor driver not working"); }
  while (!active_controller->init()) {Serial.println("Controller wont start!"); }

  led_control.startUpBlink();

  Serial.println("Boot completed successful");
}


void setup() {
  initSystem();
}

void printState()
{
  //Serial.printf("Pitch: %d, ", current_state.pitch);
  //Serial.printf("Roll: %d, ", current_state.roll);
  //Serial.printf("Yaw: %d, ", current_state.yaw);
  //Serial.printf("Pitch: %d\n", current_state.thrust);
}

void loop() {
  radio.update();
  //sensors.read(&sensor_values);
  //Estimator

  //motor_control.update();
}