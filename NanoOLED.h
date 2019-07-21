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

#ifndef NanoOLED_data_H
#define NanoOLED_data_H

// NanoOLED Instruction set addresses

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#include "Wire.h"
#endif

#define PAGE_MODE 01
#define HORIZONTAL_MODE 02

#define NanoOLED_Address 0x3c
#define NanoOLED_Command_Mode 0x80
#define NanoOLED_Data_Mode 0x40
#define NanoOLED_Display_Off_Cmd 0xAE
#define NanoOLED_Display_On_Cmd 0xAF
#define NanoOLED_Normal_Display_Cmd 0xA6
#define NanoOLED_Inverse_Display_Cmd 0xA7
#define NanoOLED_Activate_Scroll_Cmd 0x2F
#define NanoOLED_Dectivate_Scroll_Cmd 0x2E
#define NanoOLED_Set_Brightness_Cmd 0x81

#define Set_Page_Addr 0xB0
#define Set_Col_LowAddr 0x00
#define Set_Col_HighAddr 0x10 

#define Scroll_Left 0x00
#define Scroll_Right 0x01

#define Scroll_2Frames 0x7
#define Scroll_3Frames 0x4
#define Scroll_4Frames 0x5
#define Scroll_5Frames 0x0
#define Scroll_25Frames 0x6
#define Scroll_64Frames 0x1
#define Scroll_128Frames 0x2
#define Scroll_256Frames 0x3

class NanoOLED
{

public:
    char addressingMode;
    bool isSH1106;

    NanoOLED();
    NanoOLED(bool sh1106);

    void init();
    void init(bool sh1106);
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

    void setHorizontalScrollProperties(unsigned char direction, unsigned char startPage, unsigned char endPage, unsigned char scrollSpeed);
    void activateScroll();
    void deactivateScroll();
};

#endif
