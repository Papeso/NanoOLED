/*
 * NanoOLED => NanoOLED
 * SSD130x & SH1106 OLED Driver Library
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

#ifndef NANOOLED_H
#define NANOOLED_H

// NanoOLED Instruction set addresses

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif

const char OLED_Address = 0x3c;

enum OLED_CHIP
{
    SSD1306,
    SH1106
}

enum OLED_MEMODE
{
    PAGE_MODE = 0x02,
    VERTICAL_MODE = 0x01,
    HORIZONTAL_MODE = 0x00,
};

enum OLED_ADDRMSK
{
    OLED_MSK_Page_Addr = 0xB0,
    OLED_MSK_Col_LowAddr = 0x00,
    OLED_MSK_Col_HighAddr = 0x10,
};

enum OLED_CMD
{
    OLED_Command_Mode = 0x00,
    OLED_Data_Mode = 0x40,
    OLED_Memory_Mode = 0x20,
    OLED_Display_Off_Cmd = 0xAE,
    OLED_Display_On_Cmd = 0xAF,
    OLED_Normal_Display_Cmd = 0xA6,
    OLED_Inverse_Display_Cmd = 0xA7,
    OLED_Activate_Scroll_Cmd = 0x2F,
    OLED_Dectivate_Scroll_Cmd = 0x2E,
    OLED_Set_Brightness_Cmd = 0x81,

    /* 
    OLED_Set_Col_Range = 0x21,
    OLED_Set_Page_Range = 0x22,
    */
};

enum SCROLL_DIR
{
    Scroll_Left = 0x00,
    Scroll_Right = 0x01,
};

enum SCROLL_FRAMES
{
    Scroll_2Frames = 0x7,
    Scroll_3Frames = 0x4,
    Scroll_4Frames = 0x5,
    Scroll_5Frames = 0x0,
    Scroll_25Frames = 0x6,
    Scroll_64Frames = 0x1,
    Scroll_128Frames = 0x2,
    Scroll_256Frames = 0x3,
};

class NanoOLED
{

public:
    OLED_MEMODE memMode;
    OLED_CHIP chipType;

    NanoOLED();
    NanoOLED(OLED_CHIP chip);

    void init();
    void defaultInit();

    void setNormalDisplay();
    void setInverseDisplay();

    void sendCommand(unsigned char command);
    void sendData(unsigned char Data);

    void setPageMode();
    void setHorizontalMode();

    void setCursor(unsigned char row, unsigned char col);
    void clearDisplay();
    void setBrightness(unsigned char Brightness);
    void putChar(unsigned char c);
    void putString(const char *String);
    unsigned char putNumber(long n);
    unsigned char putFloat(float floatNumber, unsigned char decimal);
    unsigned char putFloat(float floatNumber);
    void drawBitmap(unsigned char *bitmaparray, int bytes);

    /*
    void setDisplayArea(unsigned char page_start, unsigned char page_end, unsigned char col_start, unsigned char col_end) {
        sendCommand(Set_Col_Range);
        sendCommand(col_start);
        sendCommand(col_end);
        sendCommand(Set_Page_Range);
        sendCommand(page_start);
        sendCommand(page_end);
    }*/

    void setHorizontalScrollProperties(unsigned char direction, unsigned char startPage, unsigned char endPage, unsigned char scrollSpeed);
    void activateScroll();
    void deactivateScroll();
};

#endif
