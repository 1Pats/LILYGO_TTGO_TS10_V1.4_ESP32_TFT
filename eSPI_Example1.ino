/* 
 * Example 1 
 * LILYGO TTGO TS TFT test code,  display test
 * Code is based on the KISS principle
 * Created with one main goal - to use it for testing. I had spent a lot of time getting my breakout board to work. 
 * and I want to save time for other hobbyists. No big logics in this code - it is just for verification of the proposed functionality.
 * I bought this breakout board on AliExpress some time ago.  At some point I reworked my stuff and decided to use it.
 * Here is the full name of the item I bought:
 * LILYGO® TTGO TS V1.0 V1.4 ESP32 1.44 1.8 TFT MicroSD Card Slot Speaker MPU9250 Bluetooth Wifi Module 
 * I guess somewhere it is marked  as T10 V1.4
 * Cannot explain what each number means.  
 * See picture here: https://github.com/LilyGO/TTGO-TS/blob/master/Image/T10_V1.4.jpg
 * Module is implemented as a breakout board (display is soldered to ESP32)
 * This is my first TFT display, I was unfamiliar with all this stuff. I spent over a week trying to get it up and running, trying different libraries,
 * and searched through forums, YouTube...
 * Information is scarce, contradictory, code examples did not work, videos lack technical details, I almost gave up...
 * A few additional facts as I dig deeper:
 * Adafruit https://www.adafruit.com/category/97 has snmth similar  (but only similar)
 * ST7735 driver.  
 * Display size 1.8 inch
 * Resolution 128x160
 * Type R display
 * Screen PINS as shown:
 * MOSI    23
 * SCLK    5 
 * CS      16
 * DC (A0) 17
 * RST     9
 * https://github.com/Xinyuan-LilyGO/LilyGo_Txx - I found something there
 * Finally I stopped at the TFT_eSPI library.
 * https://github.com/Bodmer/TFT_eSPI
 * At a time when I was developing this code - the library had version 2.5.43.
 * The documentation is there, but unfortunately it was not up to date.
 * https://doc-tft-espi.readthedocs.io/
 * My first attempts were unsuccessful, but I finally got it to work.
 * The TFT_eSPI library has instructions on how to make adjustments for a particular display type, but it is quite difficult to capture all the nuances. 
 * and what exactly is required. Also - I was unable to find any technical details such as a datasheet.
 * What I did:
 * 1) deleted the KConfig file 
 * 2) in the file: user_setup_select.h 
 * commented line // #include <User_Setup.h> and uncommented #include <User_Setups/Setup2_ST7735.h>.
 * 3) The following has been done in file Setup2_ST7735.h
 * Replaced #define REDTAB with #define BLACKTAB (see around line 14)
 * This is a bit of a fuzzy parameter, initially stands for border colour around the screen, and as I understand it 
 * affects screen positioning and other behaviour.
 * You can experiment with it - all "TABS" are in ST7735_Defines.h.
 * 4) Commented previous pin definitions, added these (around line 28)
 * TFT display pins -----------------------------------------------------
 * #define TFT_MOSI       23                                          
 * #define TFT_SCLK       5
 * #define TFT_CS         16  
 * #define TFT_DC         17 
 * #define TFT_RST        -1
 *
 * Initially added GPIO for RST pin, but setup crashed. After hours of debugging - ola!
 * Reset pin should be defined like this:
 * #define TFT_RST -1 
 * When I did this, the display showed signs of life for the first time.
 *
 * To clear the screen, the fillScrean() function is usually used, but as I discovered, it sometimes leaves uncleared areas.
 * The orientation of the display is quite important, so the example shows its effect. Unfortunately, graphics behave a bit differently for each rotation.
 */

#include <TFT_eSPI.h>                                      // library for TFT displays
TFT_eSPI tft = TFT_eSPI();                                 // includes MUST(!) be corrected to make it work, otherwise, most likely it will crash

void setup(void) {
  Serial.begin(115200);                                    // do not forget to match output speed in Serial Monitor settings
  Serial.println(" LILYGO® TTGO TS Display test");         // print title as a sign- ESP32 is started
  Serial.printf("TFT_MOSI=   23 |%d\n",TFT_MOSI);          // print out defines to to see how includes work
  Serial.printf("TFT_SCLK=   5  |%d\n",TFT_SCLK);          // suggested pin definitions and real ones
  Serial.printf("TFT_CS=     16 |%d\n",TFT_CS);
  Serial.printf("TFT_DC=     17 |%d\n",TFT_DC);
  Serial.printf("TFT_RST=    9  |%d (actualy -1 is a right setting)\n",TFT_RST);
  Serial.printf("TFT_WIDTH=  %3d|%d\n",  TFT_WIDTH,  tft.width());
  Serial.printf("TFT_HEIGHT= %3d|%d\n",  TFT_HEIGHT, tft.height());

  tft.init();                                              // this is a critical command, usually code crashes here
  
  Serial.print("-");                                       // these printouts might help in debugging        
  tft.setTextFont(2);                                      // use a bit larger font (font size #2) 
  Serial.print("-");                                       // in printout it means - execution had passed init() and setTextFont();    
  tft.setTextColor(TFT_BLUE);                              // set text color
  Serial.println("Initialized");                           // initializatio is completed
}

/*
* very simple graphics test.
* unfortunately my display has coordinate shifts.
* (For example, (0,0) pixel is hidden)
* This is why I am not using border coordinates
*/ 
void loop() {
  static int16_t i = 0;                                    // iteration counter

  tft.fillScreen(TFT_BLACK);                               // sometimes fillScrean() leaves uncleared area
  tft.setRotation(2);                                      // this is a workaround to make sure - the screen is clear
  tft.fillScreen(TFT_BLACK);         
  tft.setRotation(i%4);                                    // final rotation
                                    
  uint8_t iW = tft.width();                                // width of the screen
  uint8_t iH = tft.height();                               // height of the screen (values swap due to rotation)

  tft.drawRect(2,    2,   iW-2, iH-2, TFT_BLUE);           // offsets here are due to physical defects of my screen
  tft.drawLine(2,    2,   iW-2, iH-2, TFT_BLUE);           // draw first crossing line- top, left to bottom, right
  tft.drawLine(iW-2, 2,   2,    iH-2, TFT_BLUE);           // draw second crossing line top, right to bottom, left

  char cBuff[15];                                          // buffer where to make string for output 
  sprintf(cBuff,"Rotation:%d", i++%4);                     // show rotation number
  tft.drawCentreString(cBuff, iW/2, iH/2-8, 2);            // draw centered string
  Serial.println(cBuff);

  delay(3000);                                             // allow to see                
}

