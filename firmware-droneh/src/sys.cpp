#include "sys.h"


bool driverErrors()
{
  return error != 0;
}

void scan()
{
   byte error, address;
  int nDevices;
 
  Serial.println("Scanning...");
 
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire1.beginTransmission(address);
    error = Wire1.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
 
  delay(5000);           // wait 5 seconds for next scan
}


/*
void printVLX()
{
  Serial.print(sensor.read());
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  Serial.println();
}

void printAcc()
{
      sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    Serial.print("Accelerometer "); Serial.print("X: "); Serial.print(a.acceleration.x, 1); Serial.print(" m/s^2, "); Serial.print("Y: "); Serial.print(a.acceleration.y, 1); Serial.print(" m/s^2, "); Serial.print("Z: "); Serial.print(a.acceleration.z, 1); Serial.println(" m/s^2");
    Serial.print("Gyroscope "); Serial.print("X: "); Serial.print(g.gyro.x, 1); Serial.print(" rps, "); Serial.print("Y: "); Serial.print(g.gyro.y, 1); Serial.print(" rps, "); Serial.print("Z: "); Serial.print(g.gyro.z, 1); Serial.println(" rps");

}

void printTemp()
{
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature()); Serial.print(" *C  |  ");
  Serial.print("Pressure = "); Serial.print(bmp.readPressure()); Serial.print(" Pa  |  ");
  Serial.print("Altitude = "); Serial.print(bmp.readAltitude()); Serial.print(" meters  |  ");
  Serial.print("Pressure at sealevel (calculated) = "); Serial.print(bmp.readSealevelPressure()); Serial.print(" Pa  |  ");
  Serial.print("Real altitude = "); Serial.print(bmp.readAltitude(101500)); Serial.println(" meters");

}*/