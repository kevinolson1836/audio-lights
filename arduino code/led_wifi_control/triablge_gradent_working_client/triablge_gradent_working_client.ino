
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "fuck"
#define STAPSK  "fuckthewifi"
#endif
const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.1.149";
const uint16_t port = 37018;


/**
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
#define MAX_LED   125
//#define num_led   46
#define LED_PIN   3
#define CHIPSET   WS2812
#define COLOR_ORDER GRB
#define BRIGHTNESS  126

CRGB leds[MAX_LED];

int prevx0 = 0;


// Frequency for main loop function
#define READ_DELAY  75  // milliseconds
WiFiClient client;

int long ch1;
int long prev_ch1;
uint8_t ch2;
int first_read = 1;
//int num_led = 46; // total number  of LEDS
void setup()
{

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //  Serial.println("");
  Serial.println("WiFi connected\n\n");
  //  Serial.println("IP address: ");
  //  Serial.println(WiFi.localIP());

#if DEBUG
  Serial.begin(57600);
#endif
  PRINTS("[MD_MSGEQ7_FastLED]");

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, MAX_LED).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(BRIGHTNESS);

  FastLED.setDither(0);
//  FastLED.show();

  //  MSGEQ7.begin();
  int val1 = 0;

  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  //  WiFiClient client;


  client.connect(host, port);
}

int highval = 0;
int x = 0;
int SATURATION = 255;
void loop()
{



  //    MSGEQ7.read();
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    //    delay(5000);
    return;
  }
  if (1) {
    //    if (first_read == 1) {
    client.setTimeout(50);
    ch1 = client.parseInt();
    if (ch1 == 0){
      ch1 = prev_ch1;
    }

  Serial.println(ch1);
  }
  prev_ch1 = ch1;
  highval = ch1;
  Serial.print("high val: ");
  Serial.println(highval);
  


  x = map(highval, 0, 1024, 0, MAX_LED);

  fill_gradient(leds, MAX_LED, CHSV(95, 255, 255), CHSV(0, 255, 255));

  for (int i = prevx0; i < MAX_LED; i++)
  {
    leds[i].setRGB(0, 0, 0);
  }
  prevx0 = prevx0 - 1;
  if ( x > prevx0 ) {
    prevx0 = x;
  }

  if (highval == 1200){
      Serial.println("rainbow");
       uint8_t thisSpeed = 10;
    uint8_t deltaHue= 1;
    uint8_t thisHue = beat8(thisSpeed,255); 
    fill_rainbow(leds, MAX_LED, thisHue, deltaHue);               // Use FastLED's fill_rainbow routine.
//    fill_rainbow(leds, MAX_LED, 10, 10);            // Use FastLED's fill_rainbow routine.
//      FastLED.show();
//      return;
  }

if (highval == 1){
//FastLED.clear();  // clear all pixel data
//  FastLED.show();
//    
      Serial.println("off");
          for (int i = 0; i < MAX_LED+1; i++)
       {
        leds[i].setRGB(0, 0, 0);
       }
       leds[0].setRGB(0, 0, 0);
//             FastLED.show();
//exit;
//      return;
  }
//  FastLED.clear();  // clear all pixel data

//  leds[124].setRGB(200,200,200);
  FastLED.show();
}
