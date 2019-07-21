/*
 * NanoOled.cpp => NanoOLED.cpp
 * SSD130x OLED Driver Library
 *
 * Copyright (c) 2011 seeed technology inc.
 * Author        :   Visweswara R
 * Create Time   :   Dec 2011
 * Change Log    :   Support SH1106 (2019-7)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Wire.h"
#include "NanoOLED.h"

#if (defined(__AVR__) || defined(__SAMD21G18A__))
#include <avr/pgmspace.h>
#else
  #ifndef (pgm_read_byte)
    #define pgm_read_byte(p) (*(p))
  #endif
#endif

#if defined(ARDUINO) && ARDUINO >= 100
#define Wire_Write(x) Wire.write(x)
#else
#define Wire_Write(x) Wire.send(x)
#endif

// 8x8 Font ASCII 32 - 127 Implemented
// Users can modify this to support more characters(glyphs)
// BasicFont is placed in code memory.

// This font be freely used without any restriction(It is placed in public domain)
const unsigned char BasicFont[][6] PROGMEM =
    {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x5F, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x07, 0x00, 0x07, 0x00},
        {0x00, 0x14, 0x7F, 0x14, 0x7F, 0x14},
        {0x00, 0x24, 0x2A, 0x7F, 0x2A, 0x12},
        {0x00, 0x23, 0x13, 0x08, 0x64, 0x62},
        {0x00, 0x36, 0x49, 0x55, 0x22, 0x50},
        {0x00, 0x00, 0x05, 0x03, 0x00, 0x00},
        {0x00, 0x1C, 0x22, 0x41, 0x00, 0x00},
        {0x00, 0x41, 0x22, 0x1C, 0x00, 0x00},
        {0x00, 0x08, 0x2A, 0x1C, 0x2A, 0x08},
        {0x00, 0x08, 0x08, 0x3E, 0x08, 0x08},
        {0x00, 0xA0, 0x60, 0x00, 0x00, 0x00},
        {0x00, 0x08, 0x08, 0x08, 0x08, 0x08},
        {0x00, 0x60, 0x60, 0x00, 0x00, 0x00},
        {0x00, 0x20, 0x10, 0x08, 0x04, 0x02},
        {0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E},
        {0x00, 0x00, 0x42, 0x7F, 0x40, 0x00},
        {0x00, 0x62, 0x51, 0x49, 0x49, 0x46},
        {0x00, 0x22, 0x41, 0x49, 0x49, 0x36},
        {0x00, 0x18, 0x14, 0x12, 0x7F, 0x10},
        {0x00, 0x27, 0x45, 0x45, 0x45, 0x39},
        {0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30},
        {0x00, 0x01, 0x71, 0x09, 0x05, 0x03},
        {0x00, 0x36, 0x49, 0x49, 0x49, 0x36},
        {0x00, 0x06, 0x49, 0x49, 0x29, 0x1E},
        {0x00, 0x00, 0x36, 0x36, 0x00, 0x00},
        {0x00, 0x00, 0xAC, 0x6C, 0x00, 0x00},
        {0x00, 0x08, 0x14, 0x22, 0x41, 0x00},
        {0x00, 0x14, 0x14, 0x14, 0x14, 0x14},
        {0x00, 0x41, 0x22, 0x14, 0x08, 0x00},
        {0x00, 0x02, 0x01, 0x51, 0x09, 0x06},
        {0x00, 0x32, 0x49, 0x79, 0x41, 0x3E},
        {0x00, 0x7E, 0x09, 0x09, 0x09, 0x7E},
        {0x00, 0x7F, 0x49, 0x49, 0x49, 0x36},
        {0x00, 0x3E, 0x41, 0x41, 0x41, 0x22},
        {0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C},
        {0x00, 0x7F, 0x49, 0x49, 0x49, 0x41},
        {0x00, 0x7F, 0x09, 0x09, 0x09, 0x01},
        {0x00, 0x3E, 0x41, 0x41, 0x51, 0x72},
        {0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F},
        {0x00, 0x41, 0x7F, 0x41, 0x00, 0x00},
        {0x00, 0x20, 0x40, 0x41, 0x3F, 0x01},
        {0x00, 0x7F, 0x08, 0x14, 0x22, 0x41},
        {0x00, 0x7F, 0x40, 0x40, 0x40, 0x40},
        {0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F},
        {0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F},
        {0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E},
        {0x00, 0x7F, 0x09, 0x09, 0x09, 0x06},
        {0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E},
        {0x00, 0x7F, 0x09, 0x19, 0x29, 0x46},
        {0x00, 0x26, 0x49, 0x49, 0x49, 0x32},
        {0x00, 0x01, 0x01, 0x7F, 0x01, 0x01},
        {0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F},
        {0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F},
        {0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F},
        {0x00, 0x63, 0x14, 0x08, 0x14, 0x63},
        {0x00, 0x03, 0x04, 0x78, 0x04, 0x03},
        {0x00, 0x61, 0x51, 0x49, 0x45, 0x43},
        {0x00, 0x7F, 0x41, 0x41, 0x00, 0x00},
        {0x00, 0x02, 0x04, 0x08, 0x10, 0x20},
        {0x00, 0x41, 0x41, 0x7F, 0x00, 0x00},
        {0x00, 0x04, 0x02, 0x01, 0x02, 0x04},
        {0x00, 0x80, 0x80, 0x80, 0x80, 0x80},
        {0x00, 0x01, 0x02, 0x04, 0x00, 0x00},
        {0x00, 0x20, 0x54, 0x54, 0x54, 0x78},
        {0x00, 0x7F, 0x48, 0x44, 0x44, 0x38},
        {0x00, 0x38, 0x44, 0x44, 0x28, 0x00},
        {0x00, 0x38, 0x44, 0x44, 0x48, 0x7F},
        {0x00, 0x38, 0x54, 0x54, 0x54, 0x18},
        {0x00, 0x08, 0x7E, 0x09, 0x02, 0x00},
        {0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C},
        {0x00, 0x7F, 0x08, 0x04, 0x04, 0x78},
        {0x00, 0x00, 0x7D, 0x00, 0x00, 0x00},
        {0x00, 0x80, 0x84, 0x7D, 0x00, 0x00},
        {0x00, 0x7F, 0x10, 0x28, 0x44, 0x00},
        {0x00, 0x41, 0x7F, 0x40, 0x00, 0x00},
        {0x00, 0x7C, 0x04, 0x18, 0x04, 0x78},
        {0x00, 0x7C, 0x08, 0x04, 0x7C, 0x00},
        {0x00, 0x38, 0x44, 0x44, 0x38, 0x00},
        {0x00, 0xFC, 0x24, 0x24, 0x18, 0x00},
        {0x00, 0x18, 0x24, 0x24, 0xFC, 0x00},
        {0x00, 0x00, 0x7C, 0x08, 0x04, 0x00},
        {0x00, 0x48, 0x54, 0x54, 0x24, 0x00},
        {0x00, 0x04, 0x7F, 0x44, 0x00, 0x00},
        {0x00, 0x3C, 0x40, 0x40, 0x7C, 0x00},
        {0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C},
        {0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C},
        {0x00, 0x44, 0x28, 0x10, 0x28, 0x44},
        {0x00, 0x1C, 0xA0, 0xA0, 0x7C, 0x00},
        {0x00, 0x44, 0x64, 0x54, 0x4C, 0x44},
        {0x00, 0x08, 0x36, 0x41, 0x00, 0x00},
        {0x00, 0x00, 0x7F, 0x00, 0x00, 0x00},
        {0x00, 0x41, 0x36, 0x08, 0x00, 0x00},
        {0x00, 0x02, 0x01, 0x01, 0x02, 0x01},
        {0x00, 0x02, 0x05, 0x05, 0x02, 0x00}};

void NanoOLED::init()
{
  defaultInit();
}

void NanoOLED::defaultInit()
{
  const unsigned char initializeCmd[] = {
    0xAE, /* Entire Display OFF */
    0xD5, /* Set Display Clock Divide Ratio and Oscillator Frequency */
    0x80, /* Default Setting for Display Clock Divide Ratio and Oscillator Frequency that is recommended */
    0xA8, /* Set Multiplex Ratio */
    0x3F, /* 64 COM lines */
    0xD3, /* Set display offset */
    0x00, /* 0 offset */
    0x40, /* Set first line as the start line of the display */
    0x8D, /* Charge pump */
    0x14, /* Enable charge dump during display on */
    0x20, /* Set memory addressing mode */
    0x00, /* Horizontal addressing mode */
    0xA1, /* Set segment remap with column address 127 mapped to segment 0 */
    0xC8, /* Set com output scan direction, scan from com63 to com 0 */
    0xDA, /* Set com pins hardware configuration */
    0x12, /* Alternative com pin configuration, disable com left/right remap */
    0x81, /* Set contrast control */
    0x80, /* Set Contrast to 128 */
    0xD9, /* Set pre-charge period */
    0xF1, /* Phase 1 period of 15 DCLK, Phase 2 period of 1 DCLK */
    0xDB, /* Set Vcomh deselect level */
    0x20, /* Vcomh deselect level ~ 0.77 Vcc */
    0xA4, /* Entire display ON, resume to RAM content display */
    0xA6, /* Set Display in Normal Mode, 1 = ON, 0 = OFF */
    0x2E, /* Deactivate scroll */
    0xAF  /* Display on in normal mode */
  };
  const int cmdLen = sizeof(initializeCmd) / sizeof(initializeCmd[0]);
  for(int idx = 0; idx < cmdLen; idx++) {
    sendCommand(initializeCmd[idx]);
  }
}

void NanoOLED::sendCommand(unsigned char command)
{
  Wire.beginTransmission(NanoOLED_Address); // begin I2C communication
  Wire_Write(NanoOLED_Command_Mode);        // Set OLED Command mode
  Wire_Write(command);
  Wire.endTransmission(); // End I2C communication
}

void NanoOLED::setBrightness(unsigned char Brightness)
{
  sendCommand(NanoOLED_Set_Brightness_Cmd);
  sendCommand(Brightness);
}

void NanoOLED::setHorizontalMode()
{
  addressingMode = HORIZONTAL_MODE;
  sendCommand(0x20); //set addressing mode
  sendCommand(0x00); //set horizontal addressing mode
}

void NanoOLED::setPageMode()
{
  addressingMode = PAGE_MODE;
  sendCommand(0x20); //set addressing mode
  sendCommand(0x02); //set page addressing mode
}

void NanoOLED::setCursor(unsigned char row, unsigned char col)
{
  if (isSH1106)
  {
    col = col + 2;
  }
  sendCommand(Set_Page_Addr + row);                    //set page address
  sendCommand(Set_Col_LowAddr + (col & 0x0F));         //set column lower address
  sendCommand(Set_Col_HighAddr + ((col >> 4) & 0x0F)); //set column higher address
}

void NanoOLED::clearDisplay()
{
  unsigned char i, j;
  sendCommand(NanoOLED_Display_Off_Cmd); //display off
  for (j = 0; j < 8; j++)
  {
    setCursor(j, 0);
    {
      for (i = 0; i < 128; i++) //clear all columns
      {
        sendData(0x00);
      }
    }
  }
  sendCommand(NanoOLED_Display_On_Cmd); //display on
  setCursor(0, 0);
}

void NanoOLED::sendData(unsigned char Data)
{
  Wire.beginTransmission(NanoOLED_Address); // begin I2C transmission
#if defined(ARDUINO) && ARDUINO >= 100
  Wire.write(NanoOLED_Data_Mode); // data mode
  Wire.write(Data);
#else
  Wire.send(NanoOLED_Data_Mode); // data mode
  Wire.send(Data);
#endif
  Wire.endTransmission(); // stop I2C transmission
}

void NanoOLED::putChar(unsigned char c)
{
  if (c < 32 || c > 127) //Ignore non-printable ASCII characters. This can be modified for multilingual font.
  {
    c = ' '; //Space
  }
  unsigned char i = 0;
  for (i = 0; i < 6; i++)
  {
    //read bytes from code memory
    sendData(pgm_read_byte(&BasicFont[c - 32][i])); //font array starts at 0, ASCII starts at 32. Hence the translation
  }
}

void NanoOLED::putString(const char *String)
{
  unsigned char i = 0;
  while (String[i])
  {
    putChar(String[i]);
    i++;
  }
}

unsigned char NanoOLED::putNumber(long long_num)
{
  unsigned char char_buffer[10] = "";
  unsigned char i = 0;
  unsigned char f = 0;

  if (long_num < 0)
  {
    f = 1;
    putChar('-');
    long_num = -long_num;
  }
  else if (long_num == 0)
  {
    f = 1;
    putChar('0');
    return f;
  }

  while (long_num > 0)
  {
    char_buffer[i++] = long_num % 10;
    long_num /= 10;
  }

  f = f + i;
  for (; i > 0; i--)
  {
    putChar('0' + char_buffer[i - 1]);
  }
  return f;
}

unsigned char NanoOLED::putFloat(float floatNumber, unsigned char decimal)
{
  unsigned int temp = 0;
  float decy = 0.0;
  float rounding = 0.5;
  unsigned char f = 0;
  if (floatNumber < 0.0)
  {
    putString("-");
    floatNumber = -floatNumber;
    f += 1;
  }
  for (unsigned char i = 0; i < decimal; ++i)
  {
    rounding /= 10.0;
  }
  floatNumber += rounding;

  temp = floatNumber;
  f += putNumber(temp);
  if (decimal > 0)
  {
    putChar('.');
    f += 1;
  }
  decy = floatNumber - temp;                  //decimal part,
  for (unsigned char i = 0; i < decimal; i++) //4
  {
    decy *= 10;  // for the next decimal
    temp = decy; //get the decimal
    putNumber(temp);
    decy -= temp;
  }
  f += decimal;
  return f;
}

unsigned char NanoOLED::putFloat(float floatNumber)
{
  unsigned char decimal = 2;
  unsigned int temp = 0;
  float decy = 0.0;
  float rounding = 0.5;
  unsigned char f = 0;
  if (floatNumber < 0.0)
  {
    putString("-");
    floatNumber = -floatNumber;
    f += 1;
  }
  for (unsigned char i = 0; i < decimal; ++i)
  {
    rounding /= 10.0;
  }
  floatNumber += rounding;

  temp = floatNumber;
  f += putNumber(temp);
  if (decimal > 0)
  {
    putChar('.');
    f += 1;
  }
  decy = floatNumber - temp;                  //decimal part,
  for (unsigned char i = 0; i < decimal; i++) //4
  {
    decy *= 10;  // for the next decimal
    temp = decy; //get the decimal
    putNumber(temp);
    decy -= temp;
  }
  f += decimal;
  return f;
}

void NanoOLED::drawBitmap(unsigned char *bitmaparray, int bytes)
{
  char localAddressMode = addressingMode;
  if (addressingMode != HORIZONTAL_MODE)
  {
    //Bitmap is drawn in horizontal mode
    setHorizontalMode();
  }

  for (int i = 0; i < bytes; i++)
  {
    sendData(pgm_read_byte(&bitmaparray[i]));
  }

  if (localAddressMode == PAGE_MODE)
  {
    //If pageMode was used earlier, restore it.
    setPageMode();
  }
}

void NanoOLED::setHorizontalScrollProperties(unsigned char direction, unsigned char startPage, unsigned char endPage, unsigned char scrollSpeed)
{
  /*
Use the following defines for 'direction' :

 Scroll_Left
 Scroll_Right

Use the following defines for 'scrollSpeed' :

 Scroll_2Frames
 Scroll_3Frames
 Scroll_4Frames
 Scroll_5Frames
 Scroll_25Frames
 Scroll_64Frames
 Scroll_128Frames
 Scroll_256Frames

*/

  if (Scroll_Right == direction)
  {
    //Scroll Right
    sendCommand(0x26);
  }
  else
  {
    //Scroll Left
    sendCommand(0x27);
  }
  sendCommand(0x00);
  sendCommand(startPage);
  sendCommand(scrollSpeed);
  sendCommand(endPage);
  sendCommand(0x00);
  sendCommand(0xFF);
}

void NanoOLED::activateScroll()
{
  sendCommand(NanoOLED_Activate_Scroll_Cmd);
}

void NanoOLED::deactivateScroll()
{
  sendCommand(NanoOLED_Dectivate_Scroll_Cmd);
}

void NanoOLED::setNormalDisplay()
{
  sendCommand(NanoOLED_Normal_Display_Cmd);
}

void NanoOLED::setInverseDisplay()
{
  sendCommand(NanoOLED_Inverse_Display_Cmd);
}

NanoOLED::NanoOLED() : isSH1106(false)
{
  return;
}

NanoOLED::NanoOLED(bool sh1106) : isSH1106(sh1106)
{
  return;
}
