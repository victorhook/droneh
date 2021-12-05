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
int i = 0;

void loop() {
  // Update drivers
  //baro.update();
  //tof.update();
  imu.update();
  //radio.update();

  // Give estimates to estimator.
  ///const baro_measurement_t baro_measurement         = baro.read();
  ///const altitude_measurement_t altitude_measurement = tof.read();
  sensor_readings.imu  = imu.read();
  //sensor_readings.tof  = tof.read();
  //sensor_readings.baro = baro.read();

  estimator.estimate(sensor_readings.imu, &state_estimated);
  serial_link.update();

  // Set armed
  state_estimated.is_armed = state_target.is_armed;

  // Controller raw
  active_controller->setControlState(CONTROL_STATE_RAW_MOTOR);
  active_controller->update(state_target.motor_raw);

  led_control.setStatus(state_estimated.is_armed ? 1 : 0);
  //Serial.printf("Roll: %d, Pitch: %d, Yaw: %d\n", (int) (state_estimated.attitude.roll*(180 / PI)), (int) (state_estimated.attitude.pitch*(180 / PI)), (int) (state_estimated.attitude.yaw*(180 / PI)));
  //led_control.update();
  //sensors.read(&sensor_values);
  //motor_control.update();
}