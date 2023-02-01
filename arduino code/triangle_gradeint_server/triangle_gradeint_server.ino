
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "momo"
#define STAPSK  "messinaolson1"
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


int prevx0 = 0;
int prevx1 = 0;
int prevx2 = 0;
int prevx3 = 0;
int prevx4 = 0;
int prevx5 = 0;
int prevx6 = 0;

MD_MSGEQ7 MSGEQ7(RESET_PIN, STROBE_PIN, DATA_PIN);

// Frequency for main loop function
#define READ_DELAY  75  // milliseconds
 WiFiClient client;
 
uint8_t ch1;
uint8_t ch2;
int first_read = 1;
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
  FastLED.show();

//  MSGEQ7.begin();
  int val1 = 0;
  
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
//  WiFiClient client;
  

//client.connect(host, port)
}

int highval = 0;
int x = 0;
int valid = 0;

void loop()
{

  // wait for data to be available
//  unsigned long timeout = millis();
//  while (client.available() == 0) {
//    if (millis() - timeout > 50000) {
//      Serial.println(">>> Client Timeout !");
//      client.stop();
//      delay(5000);
//      return;
//    }
//  }

  // read without delay
  //  static uint8_t  bandHue[MAX_BAND] = { HUE_PURPLE, HUE_PINK, HUE_RED, HUE_YELLOW, HUE_GREEN, HUE_AQUA, HUE_BLUE };
  static uint8_t  bandHue[MAX_BAND] = { HUE_RED, HUE_ORANGE, HUE_YELLOW, HUE_GREEN, HUE_BLUE, HUE_AQUA, HUE_PURPLE };
  static uint32_t prevTime = 0;
  static uint8_t  prevbands[6] = {0, 0, 0, 0, 0, 0};


  prevTime = millis();

  //    MSGEQ7.read();
if (!client.connect(host, port)) {
    Serial.println("connection failed");
//    delay(5000);
    return;
  }
//  while (client.available()) {
//    if (first_read == 1) {
       client.setTimeout(1000);    
//       ch1 = client.parseInt();
//      first_read = 0;
//    } else {
//      ch2 = client.read();
//      first_read = 1; 
//    }
//    
  Serial.println(ch1);
  
//  Serial.print(" ");
//  }

  Serial.print(ch1);
  Serial.print(" ");
  Serial.println(ch2);
  highval = ch1;
  Serial.print("high val: ");
  Serial.println(highval);
//  for (int i = 0; i < 6; i++)
//  {
//    x = MSGEQ7.get(i);
//    if (highval < x) {
//      highval = x;
//      valid = i;
//    }
//  }

  x = map(highval, 0, 1024, 0, 113);

  //    for (int i = x; i <113; i++)
  //    {
  //      leds[i].setRGB(0,0,0);
  //    }
  //decay
  //    uint8_t thisSpeed = 0;
  //    uint8_t deltaHue= 2;
  //    uint8_t thisHue = beat8(thisSpeed,255);
  //    fill_rainbow(leds, 113, thisHue, deltaHue);
  fill_gradient(leds, 113, CHSV(95, 255, 255), CHSV(0, 255, 255));
  //
  //        for (int j = 0; j < 255; j++) {
  //    for (int i = 0; i < x; i++) {
  //      leds[i] = CHSV(i - (j * 2), 255, BRIGHTNESS); /* The higher the value 4 the less fade there is and vice versa */
  //    }
  //        }
  //      for (int i = 0; i < prevx0-1; i++)
  //      {
  //        leds[i].setRGB(25,25,25);
  //      }

  for (int i = prevx0; i < 113; i++)
  {
    leds[i].setRGB(0, 0, 0);
  }
  prevx0 = prevx0 - 1;
  if ( x > prevx0 ) {
    prevx0 = x;
  }


  //    Serial.println();
  FastLED.show();
}
