/*
 * SeeedOLED.cpp => NanoOLED.cpp
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
#ifndef pgm_read_byte
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
const uint8_t BasicFont[][6] PROGMEM =
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
  const uint8_t initializeCmd[] = {
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
  for (int idx = 0; idx < cmdLen; idx++)
  {
    sendCommand(initializeCmd[idx]);
  }
}

void NanoOLED::sendCommand(uint8_t command)
{
  Wire.beginTransmission(OLED_Address); // begin I2C communication
  Wire_Write(OLED_Command_Mode);        // Set OLED Command mode
  Wire_Write(command);
  Wire.endTransmission(); // End I2C communication
}

void NanoOLED::setBrightness(uint8_t Brightness)
{
  sendCommand(OLED_Set_Brightness_Cmd);
  sendCommand(Brightness);
}

void NanoOLED::setHorizontalMode()
{
  memMode = HORIZONTAL_MODE;
  sendCommand(OLED_Memory_Mode); //set addressing mode
  sendCommand(HORIZONTAL_MODE);  //set horizontal addressing mode
}

void NanoOLED::setPageMode()
{
  memMode = PAGE_MODE;
  sendCommand(OLED_Memory_Mode); //set addressing mode
  sendCommand(PAGE_MODE);        //set page addressing mode
}

void NanoOLED::setCursor(uint8_t row, uint8_t col)
{
  if (chipType == SH1106)
  {
    col = col + 2;
  }
  sendCommand(OLED_MSK_Page_Addr + row);                    //set page address
  sendCommand(OLED_MSK_Col_LowAddr + (col & 0x0F));         //set column lower address
  sendCommand(OLED_MSK_Col_HighAddr + ((col >> 4) & 0x0F)); //set column higher address
}

void NanoOLED::clearDisplay()
{
  uint8_t i, j;
  sendCommand(OLED_Display_Off_Cmd); //display off
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
  sendCommand(OLED_Display_On_Cmd); //display on
  setCursor(0, 0);
}

void NanoOLED::sendData(uint8_t Data)
{
  Wire.beginTransmission(OLED_Address); // begin I2C transmission
  Wire_Write(OLED_Data_Mode);           // data mode
  Wire_Write(Data);
  Wire.endTransmission(); // stop I2C transmission
}

// Max 15 pixels
size_t NanoOLED::sendPixels(const uint8_t *pix, size_t len)
{
  Wire.beginTransmission(OLED_Address); // begin I2C transmission
  Wire_Write(OLED_Data_Mode);           // data mode
  if (len > MAX_I2C_TRANSFER_BYTES - 1)
  {
    len = MAX_I2C_TRANSFER_BYTES - 1;
  }
  for (size_t idx = 0; idx < len; idx++)
  {
    Wire_Write(pgm_read_byte(&(pix[idx])));
  }
  Wire.endTransmission(); // stop I2C transmission
  return len;
}

void NanoOLED::putChar(uint8_t c)
{
  if (c < 32 || c > 127) //Ignore non-printable ASCII characters. This can be modified for multilingual font.
  {
    c = ' '; //Space
  }
  sendPixels(&BasicFont[c - 32][0], 6); //font array starts at 0, ASCII starts at 32. Hence the translation
}

void NanoOLED::drawBitmap(uint8_t *bitmaparray, uint8_t row_start, uint8_t col_start, uint8_t row, uint8_t col)
{

  if (chipType == SH1106)
  {
    int idx = 0;
    for (uint8_t p = row_start; p < row_start + row; p++)
    {
      setCursor(p, col_start);
      for (uint8_t c = 0; c < col;)
      {
        uint8_t n = sendPixels(&bitmaparray[idx], col - c);
        idx += n;
        c += n;
      }
    }
  }

  if (chipType == SSD1306)
  {
    char localAddressMode = memMode;
    if (memMode != HORIZONTAL_MODE)
    {
      //Bitmap is drawn in horizontal mode
      setHorizontalMode();
    }
    setCursor(row_start, col_start);
    int len = row * col;
    for (int idx = 0; idx < len;)
    {
      idx += sendPixels(&bitmaparray[idx], len - idx);
    }
    if (localAddressMode == PAGE_MODE)
    {
      //If pageMode was used earlier, restore it.
      setPageMode();
    }
  }
}

void NanoOLED::setHorizontalScrollProperties(uint8_t direction, uint8_t startPage, uint8_t endPage, uint8_t scrollSpeed)
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
  sendCommand(OLED_Activate_Scroll_Cmd);
}

void NanoOLED::deactivateScroll()
{
  sendCommand(OLED_Dectivate_Scroll_Cmd);
}

void NanoOLED::setNormalDisplay()
{
  sendCommand(OLED_Normal_Display_Cmd);
}

void NanoOLED::setInverseDisplay()
{
  sendCommand(OLED_Inverse_Display_Cmd);
}

#ifdef NANOLED_PRINTF
void NanoOLED::Printf(const char *fmt, ...)
{
  char buf[NANOLED_FMT_BUF_SIZE];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, NANOLED_FMT_BUF_SIZE, fmt, args);
  va_end(args);
  Serial.print(buf);
}
#endif

size_t NanoOLED::write(uint8_t c)
{
  // Serial.write(c);
  putChar(c);
  return 1;
}

/*
void NanoOLED::setDisplayArea(uint8_t page_start, uint8_t page_end, uint8_t col_start, uint8_t col_end)
{
  sendCommand(Set_Col_Range);
  sendCommand(col_start);
  sendCommand(col_end);
  sendCommand(Set_Page_Range);
  sendCommand(page_start);
  sendCommand(page_end);
}
*/
