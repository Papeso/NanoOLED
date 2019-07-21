#include <Wire.h>
#include <NanoOled.h>


NanoOLED NanoOled;

void setup()
{
  Wire.begin();	
  NanoOled.init();  //initialze SEEED OLED display

  NanoOled.clearDisplay();           //clear the screen and set start position to top left corner
  NanoOled.setNormalDisplay();       //Set display to Normal mode
  NanoOled.setHorizontalMode();      //Set addressing mode to Horizontal Mode
  NanoOled.putString("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");  //Print String (ASCII 32 - 126 )

}

void loop()
{
  
}


