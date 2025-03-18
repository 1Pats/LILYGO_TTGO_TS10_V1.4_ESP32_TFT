# LILYGO_TTGO_TS10_V1.4_ESP32_TFT
Testing LILYGO TTGO TFT display TS10 V1.4

Test Code - Screen, Colors, Keys, Speakers  
TFT displays are manufactured in large quantities. Vary with interface (pins, initialization), size, versions, mounting, power, quality, color depth, and touchscreen presence.  
This one is only one, specific, and therefore cannot be generalized to others anyway.

I bought this breakout board on AliExpress some time ago.  At some point, I checked my stuff and decided to use it.
The full name is:  
LILYGO® TTGO TS V1.0 V1.4 ESP32 1.44 1.8 TFT MicroSD Card Slot Speaker MPU9250 Bluetooth Wifi Module   
As I had spent a lot of time, decided to share my experience to save it for others

See picture here:   
https://github.com/LilyGO/TTGO-TS/blob/master/Image/T10_V1.4.jpg  

The module is implemented as a breakout board based on ESP32.  
ST7735 driver.    
Display size 1.8 inch  
Resolution 128x160 pixels  
Display type R  

Collected info bit by bit, here are some links to sources  
https://github.com/LilyGO/TTGO-TS/tree/master  
https://github.com/Xinyuan-LilyGO/LilyGo_Txx  
https://lilygo.cc/collections/all  

Library:  
https://github.com/Bodmer/TFT_eSPI  
Documentation:   
https://doc-tft-espi.readthedocs.io/  

Here are 4 examples, starting with the simplest one

Important warning, don't ignore this!  
__TO MAKE THIS WORK, YOU HAVE TO ADAPT THE TFT_eSPI LIBRARY FILES - INCLUDES__   
See instructions on how to do this, Verify the example of how it was done in my case.  
Corrections are based on information about GPIO pins. You MUST know this information.  
Without it, the TFT display will not work!  


 Example 1 - simplest code to bring the display to life   
 Example 2 - added more graphics, color support  
 Example 3 - added button support  
 Example 4 - added speaker, MPU9520 detection    
 
Code is developed according to the KISS principle without using additional libraries.  
See some info in the source file headers

I could not find much information - it might lead to the wrong conclusion about missing sensors on the breakout board. Please share your experience if you are familiar with this stuff.
