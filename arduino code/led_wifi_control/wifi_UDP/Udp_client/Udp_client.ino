#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <FastLED.h>

#define DEBUG 1
#if DEBUG
   
// Set WiFi credentials
#define WIFI_SSID "momo"
#define WIFI_PASS "messinaolson1"
#define UDP_PORT 4210

// UDP
WiFiUDP UDP;
char packet[1000];
char reply[] = "Packet received!";

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
#define LED_PIN   5
#define CHIPSET   WS2812
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define BRIGHTNESS  126

CRGB leds[MAX_LED];

int prevx0 = 0;
int long ch1;
int long prev_ch1;
uint8_t ch2;
int first_read = 1;
int counter = 0;
int counter2 = 0;

   
void setup() {
//  led set up
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, MAX_LED)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  int val1 = 0;

  
  // Setup serial port
  Serial.begin(57600);
  Serial.println();
  
  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  // Connecting to WiFi...
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  
  // Connected to WiFi
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
 
  // Begin listening to UDP port
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_PORT);
  
}  

unsigned long highval = 0;
int x = 0;
int SATURATION = 255;

void loop() {
  // If packet received...
  int packetSize = UDP.parsePacket();
  if (packetSize) {
    int len = UDP.read(packet, packetSize);
    if (len > 0)
    {
      packet[len] = '\0';
    }
    Serial.print("Packet received: ");
    Serial.println(packet);
    
    unsigned long highval = strtoul(packet, NULL, 10);

    if (highval > 2 && highval < 1199){
    x = map(highval, 0, 1024, 0, MAX_LED);
    
    Serial.print("Packet received: ");
    Serial.print(packet);
    Serial.print("     number of leds on: ");
    Serial.println(x);
 
    fill_gradient(leds, MAX_LED, CHSV(95, 255, 255), CHSV(0, 255, 255));
    for (int i = prevx0; i < MAX_LED; i++)
    {
      leds[i].setRGB(0, 0, 0);
    }
    prevx0 = prevx0 - 1;
    if ( x > prevx0 ) {
      prevx0 = x;
    }
    
    FastLED.show();
//  delay(10);
//  return; 
  }
  
  else if (highval == 1200){
    Serial.println("rainbow");
    uint8_t thisSpeed = 10;
    uint8_t deltaHue= 1;
    uint8_t thisHue = beat8(thisSpeed,255); 
    fill_rainbow(leds, MAX_LED, (millis()/25) % 255, deltaHue);            
    FastLED.show();
  }

else if (highval == 1){
//      counter = 0;
      Serial.println("off");
      FastLED.clear();  // clear all pixel data
      leds[0].setRGB(0,0,0);
      FastLED.show();
      leds[0].setRGB(0,0,0);
      FastLED.show();      
  }

  else if (highval == 2){
//      counter = 0;
      Serial.println("white");
//      FastLED.clear();  // clear all pixel data
      for (int i = -1; i < MAX_LED; i++)
    {
      leds[i].setRGB(200, 200, 0);
    }
      FastLED.show();
  }

 }
}
