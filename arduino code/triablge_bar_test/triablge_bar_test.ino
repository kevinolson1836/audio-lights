//OTA update includes 


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
#define BRIGHTNESS  126

CRGB leds[MAX_LED];

//MD_MSGEQ7
#define DATA_PIN    A0
#define RESET_PIN   13
#define STROBE_PIN  5

int rainbow_pin = 15;

int prevx0 = 0;
int prevx1 = 0;
int prevx2 = 0;
int prevx3 = 0;
int prevx4 = 0;
int prevx5 = 0;
int prevx6 = 0;


int sleep_count = 0;

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
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(16, INPUT);
//
//
}

int highval = 0;
int x = 0;
int valid = 0;
uint8_t hue = 0;


void loop() 
{
  // read without delay
//  static uint8_t  bandHue[MAX_BAND] = { HUE_PURPLE, HUE_PINK, HUE_RED, HUE_YELLOW, HUE_GREEN, HUE_AQUA, HUE_BLUE };
//  static uint8_t  bandHue[MAX_BAND] = { HUE_RED, HUE_ORANGE, HUE_YELLOW, HUE_GREEN, HUE_BLUE, HUE_AQUA, HUE_PURPLE };
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

    x = map(highval, 0, 1024, 0, 113);

    fill_gradient(leds, 113, CHSV(95,255,255),CHSV(0,255,255));

      for (int i = prevx0; i < 113; i++) 
      { 
        leds[i].setRGB(0,0,0);
      } 
    prevx0 = prevx0-1;
    if ( x > prevx0 ){
      prevx0 = x;
    }

      Serial.print("sleep_count: ");
      Serial.print(sleep_count);
      Serial.print(" x: ");
      Serial.println(x);


   if (digitalRead(16)){
//    pin is high, music is playing. do nothing
   } else {
    Serial.println("lights off");
    for (int i = 0; i < 113; i++) {
//      pin is low, turn off leds 
        leds[i].setRGB(0,0,0); 
      } 
        
   }
   
      Serial.println(digitalRead(rainbow_pin));
   if (digitalRead(rainbow_pin) == 1){
//    rainbow effect
    for (int i = 0; i < 113; ++i) {
      leds[i] = CHSV(hue + (i * 10), 255, 255);
     }

  //You can change the pattern speed here
  EVERY_N_MILLISECONDS(15){
    hue++;
  }
   }
    FastLED.show();
  }
  digitalWrite(LED_BUILTIN, HIGH);
//  Serial.print("pin value: ");
//  Serial.println(digitalRead(16));
}
