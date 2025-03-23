/* 
 * Example 4 
 * LILYGO TTGO TS TFT test code, with display, buttons, speaker
 * 1Pats March 2025
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
 * affects screen positioning and other behavior.
 * You can experiment with it - all "TABS" are in ST7735_Defines.h.
 * 4) Commented previous pin definitions, added these (around line 28)
 * TFT display pins -----------------------------------------------------
 * #define TFT_MOSI       23                                          
 * #define TFT_SCLK       5
 * #define TFT_CS         16  
 * #define TFT_DC         17 
 * #define TFT_RST        -1
 * Buttons -----------------------------------------------------------
 * #define BUTTON_MIDDLE  34                                            
 * #define BUTTON_LEFT    35
 * #define BUTTON_RIGHT   39
 * Speaker -------------------------------------------------------------
 * #define SPEAKER_OUT    25
 * // #define SPEAKER_PWD -1
 * ---------------------------------------------------------------------
 * #define SD_MISO         2
 * #define SD_MOSI        15
 * #define SD_SCLK        14
 * #define SD_CS          13
 * ----------------------------------------------------------------------
 * #define I2C_SDA        19
 * #define I2C_SCL        18
 * #define TOUCH_CS       21
 * // #define ADC_IN      35      
 *
 * Initially added definition for RST pin, but setup crashed. After hours of debugging - ola!
 * Reset pin should be defined like this:
 * #define TFT_RST -1 
 * When I did this, the display showed signs of life for the first time.
 *
 * To clear the screen, the fillScrean() function is usually used, but as I discovered, it sometimes leaves uncleared areas.
 * Developed a workaround: vDeepClearAndRotate(iRotate); It clears the screen and sets it to rotate iRotate.
 * The orientation of the display is quite important, so the example shows its effect. Unfortunately, graphics behave a bit differently for each rotation.
 *
 * March 12th
 * Added colour support
 * Noticed - TFT displays use colour coding I was not familiar with.
 * Researched it a bit - they are coded in RGB565 standard vs usual RGB888.
 * RGB888 uses one byte for each code component and can be stored in 3 bytes. R, G, B
 * RGB565 fits into 16 bits and can be stored in uint16_t (int on 16 bit processors)
 * Each colour occupies its part (bits) of the variable. The first 5 reserved for red colour, the next 6 - for green, remaining 5 - for blue
 * Here is a bit of theory:
 * https://support.touchgfx.com/docs/basic-concepts/color-formats
 * See converter example
 * Developed a few test functions as existing ones seemed too complex for simple tests.
 *
 * March 13rh
 * Added test for buttons
 * Read the middle button state at the end of each iteration (ineffective way, you should press and hold until it triggers)
 * Reading left and right buttons are implemented as interrupts.
 * When one of these buttons is pressed, show it on the screen and reset rotation to 0.
 *
 * March 14h
 * Added speaker test. The speaker is a small device soldered to the main board with two wires and looks like this: 
 * https://www.aliexpress.com/item/1005003557965245.html?spm=a2g0o.productlist.main.1.ef0f17e8QJavXb&algo_pvid=8db04d76-0577-4c9f-b037-d15eb010dcd5&algo_exp_id=8db04d76-0577-4c9f-b037-d15eb010dcd5-0&pdp_ext_f=%7B%22order%22%3A%223%22%2C%22eval%22%3A%221%22%7D&pdp_npi=4%40dis%21EUR%210.55%210.55%21%21%210.58%210.58%21%40210390b817419549562573866eac91%2112000026278525230%21sea%21LV%21119660369%21X&curPageLogUid=YGxgNtBbECoQ&utparam-url=scene%3Asearch%7Cquery_from%3A
 * Sound support can be implemented in two ways:
 * as direct writing to the port (see vBeepShort(); vBeepLong());
 * as PWM manipulation based on ledc type functions (see vPWMSiren()).
 * If everything works, you should hear the sound at the beginning of each iteration.
 * 
 * March 18th
 * Tried to revive the touchscreen, MPU9260. Tried different examples. Doesn't work. 
 * Gave up. Probably the pins I am using are wrong, but no idea what they should be.
 * Touch screen - was able to run a test example (TFT_eSPI/Test and Diagnostic/Test_touch_Controller) that prints out touch coordinates. But they are always static.
 * I2C scan does not return a valid address, which I believe is required for the MPU9250.
 * Per myy investigation- Gyroscope I2C address should be 0x71. Magnetometer- 0x48
 * Conclusion: My breakout board does not have these sensors, despite the description
 * or the information is so hidden that I can't find it.
 *
 * Assume MicroSD,Wifi and Bluetooth are standard one, not testing here
 */

// taken from MPU9250.h  Hope this is the right way to do diagnostics
#define AK8963_ADDRESS          0x0C                                           // Magnetometer Registers
#define WHO_AM_I_AK8963         0x00 
#define AK8963_I2C              0x48
#define MPU9250_ADDRESS1        0x68                                           // Gyro 
#define MPU9250_ADDRESS2        0x69                                           // not sure which is right 
#define WHO_AM_I_MPU9250        0x75                                           // Should return 0x71
#define MPU9250_I2C             0x71

#define BAR_WIDTH               4                                              // For bar testing function - bar width
#define BAR_DIST                2                                              // distance between bars

#define DELAY_TIME              3000                                           // delay between scenes
#define DELAY_ELEMENTS          20                                             // delay between elements drawing

volatile uint16_t uiStatus    = 0;                                             // system status
#define BUTTON_LEFT_PRESSED     0x0001                                         // set this bit on if the left button is pressed
#define BUTTON_RIGHT_PRESSED    0x0002                                         // ... right
#define BUTTON_MIDDLE_PRESSED   0x0004                                         // ... left

#include <TFT_eSPI.h>                                                          // libray for TFT displays 

TFT_eSPI tft = TFT_eSPI();                                                     // includes MUST(!) be corrected to make it working


// Speaker support utilities
/* 
 * Simplest implementation of sound
 * long beep
 * argument is the number of loops to perform
 * default value is 16
 */
void vBeepLong(uint8_t bTimes = 16){
  for (uint8_t i = 0; i < bTimes; i++){
      long lStart = micros();                                                   // millis() = 1000*micros()
      digitalWrite(SPEAKER_OUT,HIGH);
      while(micros()-lStart < 4000);
      digitalWrite(SPEAKER_OUT,LOW);
      while(micros()-lStart < 8000);
  }
}

/* 
 * Simplest implementation of sound
 * short beep
 */
void vBeepShort(void){
  for (uint8_t i = 0; i < 10; i++){
      long lStart = millis();
      digitalWrite(SPEAKER_OUT,HIGH);
      while(millis()-lStart < 1);
      digitalWrite(SPEAKER_OUT,LOW);
      while(millis()-lStart < 2);
  }
}

/* 
 * increasing sound
 * implementation based on ledc type functions
 */
void vPWMSiren(void){
    ledcAttach(SPEAKER_OUT,5000,8);                                            // if you are selected this method, this call might be in setup
    for (uint32_t uiVal = 0; uiVal <= 255; uiVal++){ 
        uint32_t ulT = millis();
        ledcWrite(SPEAKER_OUT, uiVal);
        while(millis()-ulT < 5);
    }
    ledcDetach(SPEAKER_OUT);                                                   //These calls are not necessary if you are using only this method
    pinMode(SPEAKER_OUT, OUTPUT);                                              // restore output mode
}

// color support utilities
#define RGB_MAX  255                                                           // max color value
#define I_RED    2                                                             // color component index 
#define I_GREEN  1
#define I_BLUE   0
typedef union {                                                                // this is a structure to quickly get color 32 bits value from bytes  
  uint32_t ulColor888;                                                         // 32 bits in memory (4 bytes)
  byte bRGB[4];                                                                // Blue, Green,Red, dummy Each 8 bits
} tColor888;                                                                   // type of this union
tColor888 ulColorStore;                                                        // variable used for color conversion

/*
 * RGB565 standard support
 * converts color code stored in separate bytes bR,bG,bB
 * to uint32_t value (however only the last 16 bits are used,
 * but TFT displays frequently use uint32_t as an argument in functions)
 * Colors are reduced, as Red and Blue can store 2**5 values, green 2**6 
 * while RGB888 standard can store 2**8 values for each color component 
 */
uint32_t ulRGBto565(byte bR, byte bG, byte bB){
   return  (((uint32_t)bR >> 3) << 11) | (((uint32_t)bG >> 2) << 5) | ((uint32_t)bB >> 3);
}

/* 
 * Extracts separate color components- bytes bR, bG, bR 
 * from value uilRGB565, where color is stored in RGB565 format
 * Note: as RGB656 contains less number of colors (65536) than RGB888 standard (16777216), code:
 * uint32_t  ulRGB656 = ulRGBto565(bR,bG,bB);
 * vRGB565toBytes(ulRGB565, bR,bG,bB);
 * does not produce the same color component values. (RGB565 reduces color depth)
 *
 * After calling this function variables bR, bG, bB contain resulting values. (Variables are passed by adresses)
 */
void vRGB565toBytes(uint32_t ulRGB565, byte &bR, byte &bG, byte &bB){  // 5+6+5 bits = 16 bits total
    bR = byte(((ulRGB565 >> 11) & 0x0000001F));                                //       5 bits  11 bits from end
    bG = byte(((ulRGB565 >> 5)  & 0x0000003F));                                //       6 bits  5 bits from end 
    bB = byte(ulRGB565          & 0x0000001F);                                 //       5 bits  0 bits from end
}

/*
 * Call this function, if left button is pressed
 */
void vCheckLeftButton(){                                       
   int iBtn = digitalRead(BUTTON_LEFT);                            
   if (iBtn == LOW) uiStatus |= BUTTON_LEFT_PRESSED;  
}

/*
 * Call this function, if right button is prssed 
 */
void vCheckRightButton(){
   int iBtn = digitalRead(BUTTON_RIGHT);                            
   if (iBtn == LOW) uiStatus |= BUTTON_RIGHT_PRESSED;  
}

/*
 * Fill screen with horizontal lines
 */
void vHorLines(){
   int16_t iHeight = tft.height()-2;                                           // real height of screen
   int16_t iWidth  = tft.width()- 2;                                           // real width of screen
   for (int16_t iL = 2; iL <  iHeight+2; iL++){                                // draw horizonontal line one  by one         
       tft.drawFastHLine(2,iL,iWidth,TFT_WHITE);                               // till screen is full
       if (iL < 8) continue;                                                   // text part - draw it quickly
       tft.setCursor(2,2);                                                     // set cursor position
       tft.printf("%3d,%3D",iWidth,iL);                                        // length of line, x position, print it out  on tft display
       if (uiStatus != 0) return;
       delay(DELAY_ELEMENTS);                                                  // give time to see
   }
}

/*
* Fill screen with vertical lines
*/
void vVertLines(){ 
    int16_t iHeight = tft.height()- 2;                                         // real height of the screen
    int16_t iWidth  = tft.width() - 2;                                         // real width of the screen
    for (int16_t iL = 2; iL < iWidth+2; iL++){                                 // fills screen with vertical red lines
       tft.drawFastVLine(iL,2,iHeight,TFT_RED);                                // draw vertical line   
       if (iL < 56) continue;                                                  // text part
       tft.setCursor(2,2);                                                     // set cursor position
       tft.printf("%3d,%3d",iL,iHeight);                                       // y coordinate, length of line, print it out  
       if (uiStatus != 0) return;
       delay(DELAY_ELEMENTS);                                                  // give time to see
   }
}

/* 
 * Draw one bar
 * iBarNumber - number of bar
 * iBarHeight - bar height
 * ulColor bar color (RGB565 code)
 */
void vDrawBar(int32_t iBarNumber, int32_t iBarHeight, uint32_t ulColor = TFT_WHITE){ 
   int16_t iX = (BAR_WIDTH+BAR_DIST)*iBarNumber+2;                             // bar position in screen
   int16_t iW = tft.width()- 2;                                                // real width of screen
   int16_t iH = tft.height()-2;                                                // real height of screen  
   if (iX+BAR_WIDTH > iW) return;                                              // don't draw  this bar as it is out of the screen
   if (iBarHeight > iH) iBarHeight = iH;                                       // if bar heigh exceeds screen height, correct height
   int16_t iYClear = iH-iBarHeight;                                            // remaining par needs to be cleared
   if (iYClear > 0) tft.fillRect(iX, 2, BAR_WIDTH+BAR_DIST, iYClear, TFT_BLACK);   // clear the top of the bar 
   tft.fillRect(iX, iH-iBarHeight, BAR_WIDTH,iH,ulColor);                      // draw bar
} 

/* 
 * generate bars
 * random height, random color
 */
void vBar(){
   int16_t iW= tft.width()- 2;                                                 // real width of screen 
   int16_t iH= tft.height()-2;                                                 // real height of dcreen
   for (int16_t i = 0; i < iW/(BAR_WIDTH+BAR_DIST); i++){                      // reuse all screen
      int32_t iRandomHeight      = random(iH);                                 // random height
      ulColorStore.bRGB[I_RED]   = byte(random(RGB_MAX));                      // generate RGB tripple Red color component
      ulColorStore.bRGB[I_GREEN] = byte(random(RGB_MAX));                      // generate RGB tripple Green color component
      ulColorStore.bRGB[I_BLUE]  = byte(random(RGB_MAX));                      // generate RGB tripple Blue color component
      uint32_t ulColor = ulRGBto565(ulColorStore.bRGB[I_RED] ,ulColorStore.bRGB[I_GREEN],ulColorStore.bRGB[I_BLUE]);           // Convert it to RGB565 standard
      //Serial.printf("(%02X,%02X,%02X)==%04X | %08X  (orginal)\n",   ulColorStore.bRGB[I_RED] ,ulColorStore.bRGB[I_GREEN],ulColorStore.bRGB[I_BLUE], ulColor,ulColorStore.ulColor888);
      //vRGB565toBytes(ulColor, ulColorStore.bRGB[I_RED] ,ulColorStore.bRGB[I_GREEN],ulColorStore.bRGB[I_BLUE]);                 // Conver back to separate bytes                 
      //Serial.printf("(%02X,%02X,%02X)==%04X | %08X  (converted)\n\n",ulColorStore.bRGB[I_RED] ,ulColorStore.bRGB[I_GREEN],ulColorStore.bRGB[I_BLUE], ulColor,ulColorStore.ulColor888);
      for (int32_t j = 0; j < iRandomHeight; j++)  vDrawBar(i,j,ulColor);      // "grow" it
      if (uiStatus != 0) return;
      delay(DELAY_ELEMENTS);                                                   // let it see
   }
}
  
/*
 * very simple graphics test:
 * draw rectangle in blue color
 * and two crossig lines
 * ofssets are used as otherwise  no all lines on screen
 */
void vDrawEnvelope(){
  uint16_t iW = tft.width();                                                   // width of the screen
  uint16_t iH = tft.height();                                                  // height of the screen
  tft.drawRect(2,     2,   iW-2, iH-2, TFT_BLUE);                              // offsets might be  different for different rotations
  tft.drawLine(2,     2,   iW-2, iH-2, TFT_BLUE);                              // draw first crossing line- top, left to bottom, right
  tft.drawLine(iW-2,  2,   2,    iH-2, TFT_BLUE);                              // draw second crossing line top, right to bottom, left
}

/*
 * sometimes fillScrean() function leaves uncleared area
 * following function is workaround to bypass it.
 * defect can be seen after initiation of the screen. 
 * Next calls, seems, works well  
 * Probably this is a defect of my beakout board
 */
void vDeepClearAndRotate(uint8_t bRotate){
   tft.fillScreen(TFT_BLACK);
   tft.setRotation(2);
   tft.fillScreen(TFT_BLACK);
   tft.setRotation(bRotate);
}

/* 
 * if button is not pressed delay execution for some time
 */
bool bBreak(){
  if (uiStatus != 0) return true;
  delay(DELAY_TIME);
  return false;
}

void setup(void) {
  Serial.begin(115200);                                                        // do not forget to verify Serial Monitor speed  
  Serial.printf("TFT_MISO=         | %d\n", TFT_MISO);                         // print out pin definitions to make sure they are right                                             
  Serial.printf("TFT_MOSI=      23 | %d\n",TFT_MOSI);                          // to see how  #includes work
  Serial.printf("TFT_SCLK=      5  | %d\n",TFT_SCLK);                          // suggested pin definitions and real ones
  Serial.printf("TFT_CS=        16 | %d\n",TFT_CS);
  Serial.printf("TFT_DC=        17 | %d\n",TFT_DC);
  Serial.printf("TFT_RST=        9 | %d (actualy -1 is a right setting)\n",TFT_RST);
  Serial.printf("TOUCH_CS=         | %d\n",TOUCH_CS);
  Serial.printf("BUTTON_MIDDLE= 34 | %d\n",BUTTON_MIDDLE);
  Serial.printf("BUTTON_RIGHT=  35 | %d\n",BUTTON_RIGHT);
  Serial.printf("BUTTON_LEFT=   39 | %d\n",BUTTON_LEFT); 
  Serial.printf("SD_MISO=        2 | %d\n",SD_MISO); 
  Serial.printf("SD_MOSI=       15 | %d\n",SD_MOSI); 
  Serial.printf("SD_SCLK=       14 | %d\n",SD_SCLK); 
  Serial.printf("SD_CS=         13 | %d\n",SD_CS); 
  Serial.printf("I2C_SDA=       19 | %d\n",I2C_SDA); 
  Serial.printf("i2C_SCL=       18 | %d\n",I2C_SCL); 
  Serial.printf("TOUCH_CS=      21 | %d\n", TOUCH_CS);
  Serial.printf("TFT_WIDTH=    %3d | %d\n",TFT_WIDTH,  tft.width());
  Serial.printf("TFT_HEIGHT=   %3d | %d\n", TFT_HEIGHT, tft.height());
  byte bMPU9250_ADDRESS1 = bReadByte(MPU9250_ADDRESS1, WHO_AM_I_MPU9250);
  byte bMPU9250_ADDRESS2 = bReadByte(MPU9250_ADDRESS2, WHO_AM_I_MPU9250);
  Serial.printf("MPU9250 Addrr=%03Xx| %03Xx %03Xx", MPU9250_I2C,bMPU9250_ADDRESS1, bMPU9250_ADDRESS2);
  if ((bMPU9250_ADDRESS1 != MPU9250_I2C) && (bMPU9250_ADDRESS1 != MPU9250_I2C)) Serial.printf(" -MPU9250 is not supported\n");
  else Serial.println();
  byte bAK8963_ADDRESS = bReadByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
  Serial.printf("AK8963 Addrr= %03Xx| %03Xx", AK8963_I2C, bAK8963_ADDRESS);
  if (bAK8963_ADDRESS != AK8963_I2C) Serial.printf(" -AK8963 is not supported\n");
  else Serial.println();
  vScanI2C();

  ulColorStore.ulColor888 = 0L;                                                // reset color bytes

  pinMode(BUTTON_MIDDLE, INPUT_PULLUP);                                        // involve built-in resistor 
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(SPEAKER_OUT, OUTPUT);

  Serial.print("-");                                                           // these printouts might help in debugging  

  // Create interrupts for button processing
  attachInterrupt(digitalPinToInterrupt(BUTTON_LEFT),vCheckLeftButton,FALLING);   // interrupt execution, if left button is pressed
  attachInterrupt(digitalPinToInterrupt(BUTTON_RIGHT),vCheckRightButton,FALLING); // interrupt execution, if right button is pressed

  Serial.print("-");                                                           // these printouts might help in debugging  
  tft.init();                                                                  //This is a critical command, usually setup code crashes here
  Serial.print("-");                                                           // these printouts might help in debugging                                     
  tft.setTextFont(2);                                                          // use a bit larger font (font size #2) 
 
  vPWMSiren();                                                                 // Play sound at the end of setup
 
  Serial.println("Initialized");                                               // Setup completed successfully
}

void loop() {
  char cBuff[15];                                                              // buffer to create output string
  static int16_t i = 0;                                                        // iteration counter
  vDeepClearAndRotate(i%4);                                                    // clear screen, set rotation
  switch(i%4) {                                                                // play different song at the beginning of each iteration 
    case 0:
         vBeepLong(32);                       
         break;
    case 1:
         vBeepShort();
         break;
    case 2:       
         vBeepLong();                                    
         break;
    case 3:
         vPWMSiren();
         break;
  }
  tft.setTextColor(TFT_BLUE);                                                  // set text color
  vDrawEnvelope();                                                             // make outer frame (test margins)
  sprintf(cBuff,"Rotation:%d", i%4);                                           // show rotation number
  tft.drawCentreString(cBuff, tft.width()/2, tft.height()/2-8, 2);             // draw centered string
  if (uiStatus & BUTTON_MIDDLE_PRESSED) {                                      // play song if button is pressed
    tft.drawCentreString("MIDDLE PRESSED", tft.width()/2, tft.height()/2+10, 2);   // draw centered string
    Serial.printf("MIDDLE PRESSED\n");  
    vBeepShort();
  }
  if (uiStatus & BUTTON_LEFT_PRESSED)   {
     tft.drawCentreString("LEFT PRESSED",   tft.width()/2, tft.height()/2+20, 2);   // draw centered string
     Serial.printf("LEFT PRESSED\n"); 
     vBeepShort();
  }
  if (uiStatus & BUTTON_RIGHT_PRESSED)  {
      tft.drawCentreString("RIGHT PRESSED",  tft.width()/2, tft.height()/2+30, 2);   // draw centered string
  

      vBeepShort();
  }
  delay(DELAY_TIME);                                                           // allow to see
  if (uiStatus != 0) {                                                         // if one of the buttons is pressed- just start iterations from the beginning                                                    
      uiStatus &= ~(BUTTON_LEFT_PRESSED|BUTTON_MIDDLE_PRESSED|BUTTON_RIGHT_PRESSED);    // reset button status bits
      i = 0;
      return;
  }                          
 
  tft.fillScreen(TFT_BLACK);                                                   // clear screen
  tft.setTextColor(TFT_BLACK,TFT_WHITE);                                       // now use black font
  vHorLines();                                                                 // draw horizontal lines
  if (bBreak()) return;                                                        // allow to see, but break, if button is pressed. Reaction to the left or right button whivh might be pressed asynchronously 

  tft.setTextColor(TFT_BLACK,TFT_RED); 
  vVertLines();                                                                // draw vertical lines
  if (bBreak()) return;                                                        // allow to see, but break, if buton is pressed 

  tft.fillScreen(TFT_BLACK);
  vBar();                                                                      // draw bars
  if (bBreak()) return;                                                        // allow to see, but break execution , if buton is pressed 

  tft.setTextColor(TFT_BLACK,TFT_BLACK);                                       // black fonts, transparent background
  tft.fillScreen(TFT_YELLOW);                                                  // now make screen yellow                            
  sprintf(cBuff,"Width:%d-%d", tft.width(),2);                                 // make string in buffer
  tft.drawCentreString(cBuff, tft.width()/2, tft.height()/2-16, 2);            // draw centered string
  sprintf(cBuff,"Height:%d-%d", tft.height(),2);
  tft.drawCentreString(cBuff, tft.width()/2, tft.height()/2+0, 2);
  
  if (bBreak()) return;                                                        // allow time to see or interrupt furher excecution
  i++;                                                                         // next iteration

  // here button press is effective only on press and hold mode- an inconvenient way. Here it is only for illustration purposses
  int iBtnMiddle = digitalRead(BUTTON_MIDDLE);                                 // read button state in an usual way
  if (iBtnMiddle == LOW) uiStatus |= BUTTON_MIDDLE_PRESSED;                    // if pressed, set the bit                               
} 

#include <Wire.h>
/*
 * taken from MPU9520 code
 */
uint8_t bReadByte(uint8_t bAddress, uint8_t bSubAddress) {
  uint8_t bData;                                                               // data` will store the register data   
  Wire.beginTransmission(bAddress);                                            // Initialize the Tx buffer
  Wire.write(bSubAddress);                                                     // Put slave register address in Tx buffer
  Wire.endTransmission(false);                                                 // Send the Tx buffer, but send a restart to keep the connection alive
  Wire.requestFrom(bAddress, (uint8_t) 1);                                      // Read one byte from slave register address 
  bData = Wire.read();                                                         // Fill Rx buffer with result
  return bData;                                                                // Return data read from slave register
}

/*
 * Scan I2C devices
 */
void vScanI2C() {
    Serial.println("Searching for I2C devices...");
    bool bFound = false;
    for (uint8_t i = 0x00; i < 0xFF; ++i) {
        Wire.beginTransmission(i);
        if (Wire.endTransmission() == 0){
            Serial.printf("I2C device= %03x\n", i);
            bFound = true;
            delay(10);
        }
    }
    if (!bFound) Serial.printf("No I2C devices\n");
}

