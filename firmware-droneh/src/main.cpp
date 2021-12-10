#include "sys.h"
#include "error.h"
#include "drivers/driver.h"
#include "drivers/sensor.h"
#include "drivers/radio.h"
#include "drivers/baro.h"
#include "drivers/imu.h"
#include "drivers/tof.h"
#include "drivers/leds.h"
#include "drivers/motors.h"
#include "drivers/serial_link.h"
#include "drivers/battery.h"

#include "controller.h"
#include "filter_complementary.h"
#include "estimator.h"
#include "log.h"
#include "state.h"

#include "debug.h"


void panic()
{
  led_control.error();
}

void initSystem()
{
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
  init_driver(baro, DRIVER_BARO);
  init_driver(imu, DRIVER_IMU);
  init_driver(tof, DRIVER_TOF);
  init_driver(radio, DRIVER_RADIO);
  init_driver(led_control, DRIVER_LED_CONTROL);
  init_driver(motor_control, DRIVER_MOTOR_CONTROL);
  init_driver(serial_link, DRIVER_SERIAL_LINK);
  init_driver(battery, DRIVER_BATTERY);

  if (driverErrors()) {
    panic();
  }

  //init_driver(active_controller, DRIVER_ACTIVE_CONTROLLER);
  //while (!active_controller->init()) {Serial.println("Controller wont start!"); }
  led_control.startUpBlink();
}


void setup() {
  initSystem();
}

void loop() {
  // Update Sensors
  imu.update();
  radio.update();

  estimator.estimate(imu.read(), &state_estimated);
  //serial_link.update();
  controller.update();
  led_control.update();
  motor_control.update();

}