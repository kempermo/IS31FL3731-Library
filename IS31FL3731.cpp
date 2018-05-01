#include <Arduino.h>
#include <Wire.h>
#include <IS31FL3731.h>

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

IS31FL3731::IS31FL3731(uint8_t x, uint8_t y){
}

boolean Adafruit_IS31FL3731::begin(uint8_t addr) {
  Wire.begin();

  _i2caddr = addr;
  _frame = 0;

  // shutdown
  writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x00);

  delay(10);

  // out of shutdown
  writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x01);

  // picture mode
  writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_CONFIG, ISSI_REG_CONFIG_PICTUREMODE);

  displayFrame(_frame);

  // all LEDs on & 0 PWM
  clear(); // set each led to 0 PWM

  for (uint8_t f=0; f<8; f++) {
    for (uint8_t i=0; i<=0x11; i++)
      writeRegister8(f, i, 0xff);     // each 8 LEDs on
  }

  return true;
}

void IS31FL3731::clear(void) {
  // all LEDs on & 0 PWM

  selectBank(_frame);

  for (uint8_t i=0; i<6; i++) {
    Wire.beginTransmission(_i2caddr);
    Wire.write((byte) 0x24+i*24);
    // write 24 bytes at once
    for (uint8_t j=0; j<24; j++) {
      Wire.write((byte) 0);
    }
    Wire.endTransmission();
  }
}

void IS31FL3731::setLEDPWM(uint8_t lednum, uint8_t pwm, uint8_t bank) {
  if (lednum >= 144) return;
  writeRegister8(bank, 0x24+lednum, pwm);
}

void IS31FL3731::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= 16)) return;
  if ((y < 0) || (y >= 9)) return;
  if (color > 255) color = 255; // PWM 8bit max

  setLEDPWM(x + y*16, color, _frame);
  return;
}

void IS31FL3731::displayFrame(uint8_t f) {
  if (f > 7) f = 0;
  writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_PICTUREFRAME, f);
}

void IS31FL3731::selectBank(uint8_t b) {
 Wire.beginTransmission(_i2caddr);
 Wire.write((byte)ISSI_COMMANDREGISTER);
 Wire.write(b);
 Wire.endTransmission();
}

void IS31FL3731::writeRegister8(uint8_t b, uint8_t reg, uint8_t data) {
  selectBank(b);

  Wire.beginTransmission(_i2caddr);
  Wire.write((byte)reg);
  Wire.write((byte)data);
  Wire.endTransmission();
  //Serial.print("$"); Serial.print(reg, HEX);
  //Serial.print(" = 0x"); Serial.println(data, HEX);
}
