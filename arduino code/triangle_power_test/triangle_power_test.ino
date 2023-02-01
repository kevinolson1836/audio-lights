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

#define  PRINT(s, x) { Serial.print(F(s)); Serial.print(x); }
#define PRINTS(x) Serial.print(F(x))
#define PRINTD(x) Serial.println(x, DEC)
#else

#define PRINT(s, x)
#define PRINTS(x)
#define PRINTD(x)
#endif

// Information about the LED strip itself
#define MAX_LED   113
#define LED_PIN   3
#define CHIPSET   WS2812
#define COLOR_ORDER GRB
#define BRIGHTNESS  126

CRGB leds[MAX_LED];

//MD_MSGEQ7
#define DATA_PIN    A0
#define RESET_PIN   16
#define STROBE_PIN  5


int prevy = 0;


MD_MSGEQ7 MSGEQ7(RESET_PIN, STROBE_PIN, DATA_PIN);

// Frequency for main loop function
#define READ_DELAY  75  // milliseconds

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

int highval = 0;
int x = 0;
int y = 0;
int valid = 0;

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
  highval = 0;
  for (int i = 0; i <6; i++) 
    {
      x = MSGEQ7.get(i);
      if (highval < x){
        highval = x;
        valid = i;
      }
    }
    x = map(highval, 0, 1024, 0, 255);
    y = map(highval, 0, 1024, 0, 255);
    Serial.print("x: ");
    Serial.print(x);
    Serial.print(" Y: ");
    Serial.println(y);
    //decay

    if (y > prevy){ 
      for (int i = 0; i < 113; i++) { 
        leds[i] = CHSV(y, 255, 255); /* The higher the value 4 the less fade there is and vice versa */ 
      } 
    }
    else {
      for (int i = 0; i < 113; i++) { 
        leds[i] = CHSV(prevy, 255, 255); /* The higher the value 4 the less fade there is and vice versa */ 
      } 
    }
      
      
    if ( y > prevy ){
      prevy = y;
    }

    prevy = prevy-8;
  
//    Serial.println();
    FastLED.show();
  }
}
