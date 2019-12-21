#ifndef SO2002A_I2C_H
#define SO2002A_I2C_H

#if defined(__SAM3X8E__) // Arduino Due
// confirmed board
#elif defined(ARDUINO_AVR_NANO) || defined(__AVR_ATmega328P__) // Arduino Nano (New bootloader)
// confirmed board
#else
#error "Unconfirmed Arduino Board"
#error "No garantee to work, so remove this with care"
#endif

#include "Print.h"

/* 
 * Macros for users
 */
#define SO2002A_I2C_CHARACTER_ROM_A 0x00
#define SO2002A_I2C_CHARACTER_ROM_B 0x04
#define SO2002A_I2C_CHARACTER_ROM_C 0x08

#define SO2002A_I2C_DISABLE_FADE_OUT_AND_BLINKING 0b00000000
#define SO2002A_I2C_ENABLE_FADE_OUT_MODE          0b00100000
#define SO2002A_I2C_ENABLE_BLINKING_MODE          0b00110000

/*
 * Macros for internal Use
 */
#define SO2002A_I2C_COMMAND_CLEAR_DISPLAY              0b00000001
#define SO2002A_I2C_COMMAND_RETURN_HOME                0b00000010
#define SO2002A_I2C_COMMAND_ENTRY_MODE_SET             0b00000100
#define SO2002A_I2C_COMMAND_DISPLAY_ON_OFF             0b00001000
#define SO2002A_I2C_COMMAND_CURSOR_OR_DISPLAY_SHIFT    0b00010000
#define SO2002A_I2C_COMMAND_FUNCTION_SET               0b00100000
#define SO2002A_I2C_COMMAND_SET_CGRAM_ADDRESS          0b01000000
#define SO2002A_I2C_COMMAND_SET_DDRAM_ADDRESS          0b10000000
#define SO2002A_I2C_COMMAND_SET_SCROLL_QUANTITY        0b10000000
#define SO2002A_I2C_COMMAND_SET_CONTRAST_CONTROL       0b10000001
#define SO2002A_I2C_COMMAND_SET_FADE_OUT_AND_BLINKING  0b00100011

#define SO2002A_I2C_COMMAND_FUNCTION_SELECTION_B    0b01110010
#define SO2002A_I2C_COMMAND_OLED_ENABLE_SD          0b01111001
#define SO2002A_I2C_COMMAND_OLED_DISABLE_SD         0b01111000

#define SO2002A_I2C_ENTRY_MODE_SHIFT_ON               (~SO2002A_I2C_ENTRY_MODE_SHIFT_OFF)
#define SO2002A_I2C_ENTRY_MODE_SHIFT_OFF                0b00000001
#define SO2002A_I2C_ENTRY_MODE_DIRECTION_LEFT_TO_RIGHT 0b00000010
#define SO2002A_I2C_ENTRY_MODE_DIRECTION_RIGHT_TO_LEFT (~SO2002A_I2C_ENTRY_MODE_DIRECTION_LEFT_TO_RIGHT)

#define SO2002A_I2C_DISPLAY_MODE_BLINK_ON    0b00000001
#define SO2002A_I2C_DISPLAY_MODE_BLINK_OFF   (~SO2002A_I2C_DISPLAY_MODE_BLINK_ON)
#define SO2002A_I2C_DISPLAY_CURSOR_ON        0b00000010
#define SO2002A_I2C_DISPLAY_CURSOR_OFF       (~SO2002A_I2C_DISPLAY_CURSOR_ON)
#define SO2002A_I2C_DISPLAY_MODE_DISPLAY_ON  0b00000100
#define SO2002A_I2C_DISPLAY_MODE_DISPLAY_OFF (~SO2002A_I2C_DISPLAY_MODE_DISPLAY_ON)

#define SO2002A_I2C_FUNCTION_EXT_IS_ON         0b00000001
#define SO2002A_I2C_FUNCTION_EXT_IS_OFF        (~SO2002A_I2C_FUNCTION_EXT_IS_ON)
#define SO2002A_I2C_FUNCTION_EXT_RE_ON         0b00000010
#define SO2002A_I2C_FUNCTION_EXT_RE_OFF        (~SO2002A_I2C_FUNCTION_EXT_RE_ON)
#define SO2002A_I2C_FUNCTION_DOUBLE_HEIGHT_ON  0b00000100
#define SO2002A_I2C_FUNCTION_DOUBLE_HEIGHT_OFF (~SO2002A_I2C_FUNCTION_DOUBLE_HEIGHT_ON)
#define SO2002A_I2C_FUNCTION_2LINE_MODE_ON     0b00001000
#define SO2002A_I2C_FUNCTION_2LINE_MODE_OFF    (~SO2002A_I2C_FUNCTION_2LINE_MODE_ON)

#define SO2002A_I2C_SCROLLQUANTITY_MASK     0b00111111
#define SO2002A_I2C_FADE_OUT_INTERVAL_MASK  0b00001111

#define SO2002A_I2C_SHIFT_CONTROL_ON 0b00001000
#define SO2002A_I2C_SHIFT_RIGHT      0b00000100
#define SO2002A_I2C_SHIFT_LEFT       (~SO2002A_I2C_SHIFT_RIGHT)

#define SO2002A_I2C_COMMAND 0x00
#define SO2002A_I2C_DATA    0x40

class SO2002A_I2C : public Print {
public:
  SO2002A_I2C(uint8_t i2cAddress);
      
  void begin(uint8_t columns, uint8_t rows);
  void clear();
  void home();
  void setCursor(uint8_t x, uint8_t y);
  void cursor();
  void noCursor();
  void blink();
  void noBlink();
  void display();
  void noDisplay();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void autoscroll();
  void noAutoscroll();
  void leftToRight();
  void rightToLeft();
  void createChar(uint8_t location, uint8_t charmap[]);

  void singleHeightFont();
  void doubleHeightFont();
  void setScrollQuantity(uint8_t dot);
  void selectCharacterRom(uint8_t font);
  void setContrastControl(uint8_t contrast);
  void setFadeOutAndBlinking(uint8_t mode, uint8_t interval);
  
  virtual size_t write(uint8_t);
  
  using Print::write;
private:
  uint8_t columns;      // width
  uint8_t rows;         // height
  uint8_t i2cAddress;   // I2C address
  uint8_t entryMode;
  uint8_t displayMode;
  uint8_t functionMode;
  
  
  void setEntryMode();
  void setDisplayMode();
  void setFunctionMode();
  void setFunctionMode(uint8_t mode);
  void oledEnableSd();
  void oledDisableSd();
  void setCgramAddress(uint8_t address);
  void setDdramAddress(uint8_t address);
  uint8_t writeCommand(uint8_t rs, uint8_t data);
};

#endif /* SO2002A_I2C_H */
