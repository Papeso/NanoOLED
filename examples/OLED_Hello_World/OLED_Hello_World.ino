#include <Wire.h>
#include <NanoOLED.h>

NanoOLED NanoOled;
void setup()
{
  Wire.begin();
  NanoOled.init();  //initialze SEEED OLED display

  NanoOled.clearDisplay();          //clear the screen and set start position to top left corner
  NanoOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  NanoOled.setPageMode();           //Set addressing mode to Page Mode
  NanoOled.setCursor(0,0);          //Set the cursor to Xth Page, Yth Column  
  NanoOled.putString("Hello World!"); //Print the String

}

void loop()
{
  
}

