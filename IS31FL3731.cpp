#include <Arduino.h>
#include <Wire.h>
#include <IS31FL3731.h>

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

uint8_t conversion_table_one [] = {0,1,2,3,4,5,6,7,8,9,10,16,17,18,19,20,21,22,23,24,25,26,32,33,34,35,36,37,38,39,40,41,42,48,49,50,51,52,53,54,55,56,57,58,64,65,66,67,68,69,70,71,72,73,74,80,81,82,83,84,85,86,87,88,89,90,96,97,98,99,100,101,102,103,104,105,106,112,113,114,115,116,117,118,119,120,121,122,15,31,47,63,79,95,111,127,143,142,141,14,30,46,62,78,94,110,126,107,108,76,13,29,45,61,77,93,109,125,12,28,44,27,11,59};
uint8_t conversion_table_two [] = {13,14,15,112,96,80,64,48,32,16,0,29,30,31,113,97,81,65,49,33,17,1,45,46,47,114,98,82,66,50,34,18,2,61,62,63,115,99,83,67,51,35,19,3,77,78,79,116,100,84,68,52,36,20,4,93,94,95,117,101,85,69,53,37,21,5,109,110,111,118,102,86,70,54,38,22,6,125,126,127,119,103,87,71,55,39,23,7,12,107,143,120,104,88,72,56,40,24,8,28,108,142,121,105,89,73,57,41,25,9,44,76,141,122,106,90,74,58,42,26,10,27,11,59};

IS31FL3731::IS31FL3731(uint8_t x, uint8_t y){
}

boolean IS31FL3731::begin(uint8_t addr) {
  Wire.begin();
  Wire.setClock(400000);

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
  writeRegister8(bank, 0x24+conversion_table_two[lednum], pwm);
}

void IS31FL3731::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= 11)) return;
  if ((y < 0) || (y >= 11)) return;
  if (color > 255) color = 255; // PWM 8bit max

  setLEDPWM(x + y*11, color, _frame);
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
