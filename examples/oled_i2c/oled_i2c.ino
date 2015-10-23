#include <Wire.h>
#include "SO2002A_I2C.h"

SO2002A_I2C oled(0x3c);

byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  
  oled.begin(20, 2);
  oled.createChar(0, smiley);
}

void loop() {
  // put your main code here, to run repeatedly:
  char data1[] = "I2C OLED YELLOW 20x2";
  char data2[] = "HELLO WORLD !!";

  // clear() and print()
  oled.clear();
  oled.print(data1);
  delay(1000);
  
  // home() -> data1 is not deleted
  oled.home();
  oled.print(data2);
  delay(1000);
  
  // setCursor()
  oled.clear();
  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 20; x++) {
      oled.setCursor(x, y);
      oled.print(x % 10);
      delay(10);
    }
  }
  delay(1000);
  
  // write()
  oled.clear();
  for (int i = 0; i < 20; i++) {
    oled.write(i + 0xb1);
  }
  delay(1000);
  
  // cursor() and noCursor();
  oled.clear();
  oled.print("cursor()");
  oled.cursor();
  delay(2000);
  oled.clear();
  oled.print("noCursor()");
  oled.noCursor();
  delay(2000);

  // blink() and noBlink()
  oled.clear();
  oled.print("blink()");
  oled.cursor();
  oled.blink();
  delay(2000);
  oled.clear();
  oled.print("noBlink()");
  oled.cursor();
  oled.noBlink();
  delay(2000);
  oled.noCursor();
  
  // noDisplay() and Display()
  oled.noDisplay();
  delay(1000);
  oled.display();
  delay(1000);

  // scrollDisplayLeft() and scrollDisplayRight()
  oled.clear();
  oled.print(data2);
  for (int i = 0; i < strlen(data2); i++) {
    oled.scrollDisplayLeft();
    delay(200);
  }
  for (int i = 0; i < strlen(data2); i++) {
    oled.scrollDisplayRight();
    delay(200);
  }

  // autoscroll() and noAutoscroll()
  oled.clear();
  oled.autoscroll();
  for (int i = 0; i < 20; i++) {
    oled.print(data1[i]);
    delay(200);
  }
  oled.clear();
  oled.noAutoscroll();
  oled.print(data1);
  delay(1000);
  
  // rightToLeft() and leftToRight()
  oled.clear();
  oled.setCursor(19, 0);
  oled.rightToLeft();
  oled.print(data1);
  delay(1000);
  oled.clear();
  oled.leftToRight();
  oled.print(data1);
  delay(1000);
  
  // createChar()
  oled.clear();
  oled.write((uint8_t)0);
  delay(1000);

  // doubleHeightFont() and singleHeightFont()
  oled.clear();
  oled.doubleHeightFont();
  oled.print(data1);
  delay(1000);
  oled.clear();
  oled.singleHeightFont();
  oled.print(data1);
  delay(1000);
  
  // setContrastControl
  for (int i = 0; i < 0xff; i++) {
    oled.setContrastControl(i);
    delay(30);
  }
  
  // selectCharacterRom()
  oled.clear();
  oled.selectCharacterRom(SO2002A_I2C_CHARACTER_ROM_A);
  for (int i = 0; i < 20; i++) {
    oled.write(i + 0xb1);
  }
  delay(1000);
  
  oled.clear();
  oled.selectCharacterRom(SO2002A_I2C_CHARACTER_ROM_B);
  for (int i = 0; i < 20; i++) {
    oled.write(i + 0xb1);
  }
  delay(1000);
  
  oled.clear();
  oled.selectCharacterRom(SO2002A_I2C_CHARACTER_ROM_C);
  for (int i = 0; i < 20; i++) {
    oled.write(i + 0xb1);
  }
  delay(1000);  

  // setFadeOutAndBlinking()
  oled.clear();
  oled.print(data1);
  oled.setFadeOutAndBlinking(SO2002A_I2C_ENABLE_BLINKING_MODE, 1);
  delay(7000);
  oled.setFadeOutAndBlinking(SO2002A_I2C_DISABLE_FADE_OUT_AND_BLINKING, 0);
}

