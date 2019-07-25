## NanoOLED Display 128X64

Froked from [Seeed-Studio/OLED_Display_128X64](https://github.com/Seeed-Studio/OLED_Display_128X64)

Support OLED 128x64 module with I2C Interface for SSD1306 and SH1106 controller.

A Lightweight Library for Arduino and Energia or other platform support basic "Wire library" to use OLED Display module with basic function and 8x6-fonts of Ascii code.

## Usage 

```c++
#include <Wire.h>
// enable "Printf" support
#define NANOLED_PRINTF 
#include <NanoOLED.h>

static const uint8_t Logo[] PROGMEM = { /*...*/ }

// NanoOLED NanoOled; // default for SSD1306

NanoOLED NanoOled(SH1106);

void setup()
{
  Wire.begin();
  NanoOled.init();  //initialze SEEED OLED display
  NanoOled.clearDisplay();  // clear the screen and set start position to top left corner
  NanoOled.drawBitmap((uint8_t*) Logo, 0, 0, 8, 128); // display logo
  NanoOled.setCursor(0, 0); // set the position of output cursor
  NanoOled.Printf("number: [%d]", 512);
  NanoOled.setCursor(1, 0);
  NanoOled.print("Hello World\nHello NanoOLED");
}

```
