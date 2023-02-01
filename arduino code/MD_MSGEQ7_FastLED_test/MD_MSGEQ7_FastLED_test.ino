/*
Example program for the MD_MSGEQ7 library

Display the data on a WS2812 RGB LED strip

LED strip hardware used is a strip of 8 WS2812 RGB LEDs
FastLED library, updates and news used can be found at http://fastled.io
*/
#include <FastLED.h>
#include <MD_MSGEQ7.h>

#define DEBUG 1
#if DEBUG

#define	PRINT(s, x)	{ Serial.print(F(s)); Serial.print(x); }
#define	PRINTS(x)	Serial.print(F(x))
#define	PRINTD(x)	Serial.println(x, DEC)
#else

#define	PRINT(s, x)
#define PRINTS(x)
#define PRINTD(x)
#endif

// Information about the LED strip itself
#define MAX_LED   113
#define LED_PIN   3
#define CHIPSET   WS2812
#define COLOR_ORDER GRB
#define BRIGHTNESS  128

CRGB leds[MAX_LED];

//MD_MSGEQ7
#define DATA_PIN    A0
#define RESET_PIN   16
#define STROBE_PIN  5


int prevx0 = 0;
int prevx1 = 0;
int prevx2 = 0;
int prevx3 = 0;
int prevx4 = 0;
int prevx5 = 0;

MD_MSGEQ7 MSGEQ7(RESET_PIN, STROBE_PIN, DATA_PIN);

// Frequency for main loop function
#define READ_DELAY  50  // milliseconds

void setup() 
{
#if DEBUG
  Serial.begin(57600);
#endif
  PRINTS("[MD_MSGEQ7_FastLED]");

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, MAX_LED).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(BRIGHTNESS);

  FastLED.setDither(0);
  FastLED.show();

  MSGEQ7.begin();
  int val1 = 0;
}

void loop() 
{
  // read without delay
//  static uint8_t  bandHue[MAX_BAND] = { HUE_PURPLE, HUE_PINK, HUE_RED, HUE_YELLOW, HUE_GREEN, HUE_AQUA, HUE_BLUE };
  static uint8_t  bandHue[MAX_BAND] = { HUE_RED, HUE_ORANGE, HUE_YELLOW, HUE_GREEN, HUE_BLUE, HUE_AQUA, HUE_PURPLE };
  static uint32_t prevTime = 0;
  static uint8_t  prevbands[6] = {0,0,0,0,0,0};
  
  
  if (millis() - prevTime > READ_DELAY) 
  {
    prevTime = millis();

    MSGEQ7.read();

    int x = map(MSGEQ7.get(0), 0, 1025, 0, 22);
//    for (int i = 0; i <x; i++) 
//    {
//      leds[i].setHSV(bandHue[0], 255, MSGEQ7.get(0) & 0xff);
//    }
    for (int i = x; i <22; i++) 
    {
      leds[i].setHSV(bandHue[0], 255, 0);
    }
    //decay
      for (int i = 0; i < prevx0-1; i++) 
      { 
        leds[i].setHSV(bandHue[0], 255, 250);
      } 
    prevx0 = prevx0-1;
    if ( x > prevx0 ){
      prevx0 = x;
    }




    x = map(MSGEQ7.get(1), 0, 1024, 22, 38);
    
    //initial led setting
//    for (int i = 22; i <x; i++) 
//    {
//      leds[i].setHSV(bandHue[1], 255, 250);
//    }
    //set all blank
    for (int i = x; i <38; i++) 
    {
      leds[i].setHSV(bandHue[1], 255, 0);
    }

    
    //decay
      for (int i = 22; i < prevx1-1; i++) 
      { 
        leds[i].setHSV(bandHue[1], 255, 250);
      } 
    prevx1 = prevx1-1;
    if ( x > prevx1 ){
      prevx1 = x;
    }



    
    x = map(MSGEQ7.get(2) & 0xff, 0, 1024, 38, 60);
//    for (int i = 38; i <x; i++) 
//    {
//      leds[i].setHSV(bandHue[2], 255, MSGEQ7.get(2) & 0xff);
//    }
    for (int i = x; i <60; i++) 
    {
      leds[i].setHSV(bandHue[2], 255, 0);
    }
        //decay
    for (int i = 38; i < prevx2-1; i++) 
    { 
      leds[i].setHSV(bandHue[2], 255, 250);
    } 
    prevx2 = prevx2-1;
    if ( x > prevx2 ){
      prevx2 = x;
    }


    
    x = map(MSGEQ7.get(3), 0, 1024, 60, 75);
//    for (int i = 60; i <x; i++) 
//    {
//      leds[i].setHSV(bandHue[3], 255, MSGEQ7.get(3) & 0xff);
//    }
    for (int i = x; i <75; i++) 
    {
      leds[i].setHSV(bandHue[3], 255, 0);
    }    //decay
      for (int i = 60; i < prevx3-1; i++) 
      { 
        leds[i].setHSV(bandHue[3], 255, 250);
      } 
    prevx3 = prevx3-1;
    if ( x > prevx3 ){
      prevx3 = x;
    }







    x = map(MSGEQ7.get(4), 0, 1024, 75, 97);
//    for (int i = 75; i <97; i++) 
//    {
//      leds[i].setHSV(bandHue[4], 255, 250);
//    }
    for (int i = x; i <97; i++) 
    {
      leds[i].setHSV(bandHue[4], 255, 0);
    }
    //decay
      for (int i = 75; i < prevx4-1; i++) 
      { 
        leds[i].setHSV(bandHue[4], 255, 250);
      } 
    prevx4 = prevx4-1;
    if ( x > prevx4 ){
      prevx4 = x;
    }





    x = map(MSGEQ7.get(5), 0, 1024, 97, 113);
//    for (int i = 97; i <x; i++) 
//    {
//      leds[i].setHSV(bandHue[5], 255, 250);
//    }
    for (int i = x; i <113; i++) 
    {
      leds[i].setHSV(bandHue[5], 255, 0);
    }
    //decay
      for (int i = 97; i < prevx5-1; i++) 
      { 
        leds[i].setHSV(bandHue[5], 255, 250);
      } 
    prevx5 = prevx5-1;
    if ( x > prevx5 ){
      prevx5 = x;
    }





    for (int i = 0; i <6; i++) 
    {
      Serial.print(MSGEQ7.get(i));
      Serial.print(' ');
    }
    Serial.println();
    FastLED.show();
  }
}
