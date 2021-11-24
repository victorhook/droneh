#include "drivers/bmp180.h"

static struct {
  short AC1;
  short AC2;
  short AC3;
  unsigned short AC4;
  unsigned short AC5;
  unsigned short AC6;
  short B1;
  short B2;
  short MB;
  short MC;
  short MD;
} bmp180;

static int8_t readAddress(int address)
{
    Wire.beginTransmission(BMP180_WRITE);
    Wire.write(address);
    Wire.requestFrom(BMP180_READ, 1);
    int8_t byte = Wire.read();
    return byte;
}

static void readCalibration()
{
    bmp180.AC1 = (readAddress(0xAA) << 8) | readAddress(0xAB);
    bmp180.AC2 = (readAddress(0xAC) << 8) | readAddress(0xAD);
    bmp180.AC3 = (readAddress(0xAE) << 8) | readAddress(0xAF);
    bmp180.AC4 = (unsigned short) (readAddress(0xB0) << 8) | readAddress(0xB1);
    bmp180.AC5 = (unsigned short) (readAddress(0xB2) << 8) | readAddress(0xB3);
    bmp180.AC6 = (unsigned short) (readAddress(0xB4) << 8) | readAddress(0xB5);
    bmp180.B1 = (readAddress(0xB6) << 8) | readAddress(0xB7);
    bmp180.B2 = (readAddress(0xB8) << 8) | readAddress(0xB9);
    bmp180.MB = (readAddress(0xBA) << 8) | readAddress(0xBB);
    bmp180.MC = (readAddress(0xBC) << 8) | readAddress(0xBD);
    bmp180.MD = (readAddress(0xBE) << 8) | readAddress(0xBF);
    Wire.endTransmission();
}

static short readTemperature()
{
    Wire.beginTransmission(BMP180_WRITE);
    Wire.write(0xF4);
    Wire.write(0x2E);
    delayMicroseconds(4500);
    short temp = (readAddress(0xF6) << 8) | readAddress(0xF7);
    Wire.endTransmission();
    return temp;
}

static short readPressure()
{
    Wire.beginTransmission(BMP180_WRITE);
    Wire.write(0xF4);
    Wire.write(0x34 + (BMP180_OSS << 6));
    delayMicroseconds(4500);
    short pressure = (readAddress(0xF6) << 8) | readAddress(0xF7);
    Wire.endTransmission();
    return pressure;
}

void BMP180::debug()
{
    readCalibration();
    Serial.println(bmp180.AC1);
    Serial.println(bmp180.AC2);
    Serial.println(bmp180.AC3);
    Serial.println(bmp180.AC4);
    Serial.println(bmp180.AC5);
    Serial.println(bmp180.AC6);
    Serial.println(bmp180.B1);
    Serial.println(bmp180.B2);
    Serial.println(bmp180.MB);
    Serial.println(bmp180.MC);
    Serial.println(bmp180.MD);
}

void BMP180::init()
{
    readCalibration();
}


short BMP180::getTemperature()
{
    return readTemperature();
}


short BMP180::getPressure()
{
    return readPressure();
}
