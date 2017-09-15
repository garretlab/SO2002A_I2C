#include "SO2002A_I2C.h"
#include "Arduino.h"
#include "Wire.h"

SO2002A_I2C::SO2002A_I2C(uint8_t i2cAddress) {
  this->i2cAddress = i2cAddress;
}

// LiquidCrystal compatible functions
void SO2002A_I2C::begin(uint8_t columns, uint8_t rows) {
  this->columns = columns;
  this->rows = rows;

  entryMode =    0b00000010;
  displayMode =  0b00000000;
  functionMode = 0b00000000;

  Wire.begin();
  noDisplay();
  clear();
  display();
  
  setDisplayMode();
  setFunctionMode();
}

void SO2002A_I2C::clear() {
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_CLEAR_DISPLAY);
  delayMicroseconds(1520);
}

void SO2002A_I2C::home() {
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_RETURN_HOME);
  delayMicroseconds(1520);
}

void SO2002A_I2C::setCursor(uint8_t col, uint8_t row) {
  int row_begin[] = {0, 32};
  setDdramAddress(col + row_begin[row] );
}

void SO2002A_I2C::cursor() {
  displayMode |= SO2002A_I2C_DISPLAY_CURSOR_ON;
  setDisplayMode();
}

void SO2002A_I2C::noCursor() {
  displayMode &= SO2002A_I2C_DISPLAY_CURSOR_OFF;
  setDisplayMode();
}

void SO2002A_I2C::blink() {
  displayMode |= SO2002A_I2C_DISPLAY_MODE_BLINK_ON;
  setDisplayMode();
}

void SO2002A_I2C::noBlink() {
  displayMode &= SO2002A_I2C_DISPLAY_MODE_BLINK_OFF;
  setDisplayMode();
}

void SO2002A_I2C::display() {
  displayMode |= SO2002A_I2C_DISPLAY_MODE_DISPLAY_ON;
  setDisplayMode();
}

void SO2002A_I2C::noDisplay() {
  displayMode &= SO2002A_I2C_DISPLAY_MODE_DISPLAY_OFF;
  setDisplayMode();
}

void SO2002A_I2C::scrollDisplayLeft() {
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_CURSOR_OR_DISPLAY_SHIFT | SO2002A_I2C_SHIFT_CONTROL_ON & SO2002A_I2C_SHIFT_LEFT);
  delayMicroseconds(37);
}

void SO2002A_I2C::scrollDisplayRight() {
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_CURSOR_OR_DISPLAY_SHIFT | SO2002A_I2C_SHIFT_CONTROL_ON | SO2002A_I2C_SHIFT_RIGHT);
  delayMicroseconds(37);
}

void SO2002A_I2C::autoscroll() {
  entryMode |= SO2002A_I2C_ENTRY_MODE_SHIFT_OFF;
  setEntryMode();
}

void SO2002A_I2C::noAutoscroll() {
  entryMode &= SO2002A_I2C_ENTRY_MODE_SHIFT_ON;
  setEntryMode();
}

void SO2002A_I2C::leftToRight() {
  entryMode |= SO2002A_I2C_ENTRY_MODE_DIRECTION_LEFT_TO_RIGHT;
  setEntryMode();
}

void SO2002A_I2C::rightToLeft() {
  entryMode &= SO2002A_I2C_ENTRY_MODE_DIRECTION_RIGHT_TO_LEFT;
  setEntryMode();
}

void SO2002A_I2C::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7;
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_SET_CGRAM_ADDRESS | (location << 3));
  delayMicroseconds(37);
  for (int i = 0; i < 8; i++) {
    writeCommand(SO2002A_I2C_DATA, charmap[i]);
    delayMicroseconds(37);
  }
}

// OLED original functions
void SO2002A_I2C::singleHeightFont() {
  functionMode &= SO2002A_I2C_FUNCTION_DOUBLE_HEIGHT_OFF;
  setFunctionMode();
}

void SO2002A_I2C::doubleHeightFont() {
  functionMode |= SO2002A_I2C_FUNCTION_DOUBLE_HEIGHT_ON;
  setFunctionMode();
}

void SO2002A_I2C::setScrollQuantity(uint8_t dot) {
  setFunctionMode(functionMode | SO2002A_I2C_FUNCTION_EXT_RE_ON);
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_SET_SCROLL_QUANTITY | (dot & SO2002A_I2C_SCROLLQUANTITY_MASK));
  delayMicroseconds(37);
  setFunctionMode(functionMode & SO2002A_I2C_FUNCTION_EXT_RE_OFF);
}

void SO2002A_I2C::selectCharacterRom(uint8_t font) {
  setFunctionMode(functionMode | SO2002A_I2C_FUNCTION_EXT_RE_ON);
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_FUNCTION_SELECTION_B);
  writeCommand(SO2002A_I2C_DATA, font);
  delayMicroseconds(37);
  setFunctionMode(functionMode & SO2002A_I2C_FUNCTION_EXT_RE_OFF);
  clear();
}

void SO2002A_I2C::setContrastControl(uint8_t contrast) {
  setFunctionMode(functionMode | SO2002A_I2C_FUNCTION_EXT_RE_ON);
  oledEnableSd();
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_SET_CONTRAST_CONTROL);
  writeCommand(SO2002A_I2C_COMMAND, contrast);
  delayMicroseconds(37);
  oledDisableSd();
  setFunctionMode(functionMode & SO2002A_I2C_FUNCTION_EXT_RE_OFF);
}

void SO2002A_I2C::setFadeOutAndBlinking(uint8_t mode, uint8_t interval) {
  setFunctionMode(functionMode | SO2002A_I2C_FUNCTION_EXT_RE_ON);
  oledEnableSd();
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_SET_FADE_OUT_AND_BLINKING);
  writeCommand(SO2002A_I2C_COMMAND, mode | (interval & SO2002A_I2C_FADE_OUT_INTERVAL_MASK));
  delayMicroseconds(37);
  oledDisableSd();
  setFunctionMode(functionMode & SO2002A_I2C_FUNCTION_EXT_RE_OFF);
}

size_t SO2002A_I2C::write(uint8_t value) {
  writeCommand(SO2002A_I2C_DATA, value);
  delayMicroseconds(37);
  return 1;
}

void SO2002A_I2C::setEntryMode() {
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_ENTRY_MODE_SET | entryMode);
  delayMicroseconds(37);
}

void SO2002A_I2C::setDisplayMode() {
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_DISPLAY_ON_OFF | displayMode);
  delayMicroseconds(37);
}

void SO2002A_I2C::setFunctionMode() {
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_FUNCTION_SET | functionMode);
  delayMicroseconds(37);
}

void SO2002A_I2C::setFunctionMode(uint8_t mode) {
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_FUNCTION_SET | mode);
  delayMicroseconds(37);
}

void SO2002A_I2C::oledEnableSd() {
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_OLED_ENABLE_SD);
}

void SO2002A_I2C::oledDisableSd() {
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_OLED_DISABLE_SD);
}

void SO2002A_I2C::setCgramAddress(uint8_t address) {
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_SET_CGRAM_ADDRESS | address);
  delayMicroseconds(37);
}

void SO2002A_I2C::setDdramAddress(uint8_t address) {
  writeCommand(SO2002A_I2C_COMMAND, SO2002A_I2C_COMMAND_SET_DDRAM_ADDRESS | address);
  delayMicroseconds(37);
}

/*
 * Low Level Control
 */
uint8_t SO2002A_I2C::writeCommand(uint8_t rs, uint8_t data) {
  Wire.beginTransmission(i2cAddress);
  Wire.write(rs);
  Wire.write(data);
  return Wire.endTransmission();
}
