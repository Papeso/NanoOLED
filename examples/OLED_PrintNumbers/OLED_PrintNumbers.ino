#include <Wire.h>
#include <NanoOled.h>

NanoOLED NanoOled;

void setup()
{
  Wire.begin();	
  NanoOled.init();  //initialze SEEED OLED display

  NanoOled.clearDisplay();           //clear the screen and set start position to top left corner
  NanoOled.setNormalDisplay();       //Set display to Normal mode
  NanoOled.setPageMode();            //Set addressing mode to Page Mode
  NanoOled.setCursor(0,0);           //Set the cursor to 0th Page, 0th Column  
  NanoOled.putNumber(123);           //Print number
  NanoOled.setCursor(1,0);           //Set the cursor to 1st Page, 0th Column  
  NanoOled.putNumber(0xFFFF);        //Print number
  NanoOled.setCursor(2,0);           //Set the cursor to 2nd Page, 0th Column  
  NanoOled.putNumber(0xFFFFFFFF);    //Print number
  NanoOled.setCursor(3,0);           //Set the cursor to 3rd Page, 0th Column  
  NanoOled.putNumber(-12345);        //Print number


}

void loop()
{
  
}


