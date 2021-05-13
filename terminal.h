/**
 * Class: Terminal
 * 
 * Purpose: This class provides methods to print data on an attached OLED 
 * screen. It is set up for the TTGO 0.96 screen and can be altered 
 * (hopefully) for the HW-630
 * 
 * @author  David Argles
 * @date    30January2021 @ 11:35h
 */
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

float  version = 1;   // version # of terminal class

/* Update the pin numbers according to your setup. Use U8X8_PIN_NONE
   if the reset pin is not connected */
//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 2, /* data=*/ 1, /* reset=*/ U8X8_PIN_NONE);         // Digispark ATTiny85
U8X8_SSD1306_128X32_UNIVISION_SW_I2C u8x8(/* clock=*/ 14, /* data=*/ 2, /* reset=*/ 4);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED

/* setup the terminal (U8X8LOG) and connect to u8g2 for automatic refresh 
   of the display. The size (width * height) depends on the display*/ 

#define U8LOG_WIDTH 16
#define U8LOG_HEIGHT 4 // x8 for screen pixel size
uint8_t u8log_buffer[U8LOG_WIDTH*U8LOG_HEIGHT];
U8X8LOG u8x8log;

class terminal
{
  public:

  // The constructor
  terminal(){
    /* U8g2 Project: SSD1306 Test Board */
    //pinMode(10, OUTPUT);
    //pinMode(9, OUTPUT);
    //digitalWrite(10, 0);
    //digitalWrite(9, 0);    
  
    /* U8g2 Project: KS0108 Test Board */
    //pinMode(16, OUTPUT);
    //digitalWrite(16, 0); 
    u8x8.begin();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8log.begin(u8x8, U8LOG_WIDTH, U8LOG_HEIGHT, u8log_buffer);
    u8x8log.setRedrawMode(1);    // 0: Update screen with newline, 1: Update screen for every char  
    u8x8log.print("=Terminal v");
    u8x8log.print(version);
    u8x8log.print("=");
 
    return;
  }

  void print(String text){
    u8x8log.print(text);
    return;
  }
};
