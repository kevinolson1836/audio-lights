#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "momo";
const char* password = "messinaolson1";

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
int sleep_count = 0;


MD_MSGEQ7 MSGEQ7(RESET_PIN, STROBE_PIN, DATA_PIN);

// Frequency for main loop function
#define READ_DELAY  75  // milliseconds

void setup() 
{
   WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
     ArduinoOTA.setHostname("power");

 ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
#if DEBUG
  Serial.begin(57600);
#endif
  PRINTS("[MD_MSGEQ7_FastLED]");

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, MAX_LED).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(BRIGHTNESS);

  FastLED.setDither(0);
  FastLED.show();

  MSGEQ7.begin();
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(16, INPUT);

}

int highval = 0;
int y = 0;
int valid = 0;

void loop() 
{
  
  ArduinoOTA.handle();
  digitalWrite(LED_BUILTIN, HIGH); 

  // read without delay
  static uint32_t prevTime = 0;
  static uint8_t  prevbands[6] = {0,0,0,0,0,0};
  
  
  if (millis() - prevTime > READ_DELAY) 
  {
    prevTime = millis();

    MSGEQ7.read();
  highval = 0;
  for (int i = 0; i <6; i++) 
    {
      y = MSGEQ7.get(i);
      if (highval < y){
        highval = y;
        valid = i;
      }
    }
    y = map(highval, 0, 1024, 0, 120);
    Serial.print("Y: ");
    Serial.println(y);
    //decay

    if (y > prevy){ 
      for (int i = 0; i < 113; i++) { 
        leds[i] = CHSV(120-y, 255, 255); /* The higher the value 4 the less fade there is and vice versa */ 
      } 
    }
    else {
      for (int i = 0; i < 113; i++) { 
        leds[i] = CHSV(120-prevy, 255, 255); /* The higher the value 4 the less fade there is and vice versa */ 
      } 
    }
      
      
    if ( y > prevy ){
      prevy = y;
    }

    prevy = prevy-2;

 if (digitalRead(16)){
//    pin is high, music is playing. do nothing
   } else {
    for (int i = 0; i < 113; i++) {
//      pin is low, turn off leds 
        leds[i].setRGB(0,0,0); 
      } 
   }
    FastLED.show();
  }
}
