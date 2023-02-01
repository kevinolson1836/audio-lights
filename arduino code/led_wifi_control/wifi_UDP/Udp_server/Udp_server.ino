#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <MD_MSGEQ7.h>
#include <FastLED.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 

// Set WiFi credentials
#define WIFI_SSID "momo"
#define WIFI_PASS "messinaolson1"
#define UDP_PORT 4210

#define IP1 "192.168.1.143"
#define IP2 "192.168.1.139"

// UDP
WiFiUDP UDP;
char packet[255];
char data[] = "1";

int val = 0;

//MD_MSGEQ7
#define DATA_PIN    A0
#define RESET_PIN   13
#define STROBE_PIN  5

int off_prev_time = 0; 
int off_DELAY = 1;
int rainbow_pin =15;

int off_pin = 16;

MD_MSGEQ7 MSGEQ7(RESET_PIN, STROBE_PIN, DATA_PIN);


ESP8266WebServer server(80);

void lights_off() {
  server.send(200, "text/plain", "lights off\r\n");
  val = 2;
}

void lights_rainbow() {
  server.send(200, "text/plain", "lights rainbow\r\n");
  val = 1;
}


void lights_white() {
  server.send(200, "text/plain", "lights white\r\n");
  val = 3;
}


void lights_audio() {
  server.send(200, "text/plain", "lights audio\r\n");
  val = 4;
}


void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}


void setup() {
  // Setup serial port
  Serial.begin(57600);

//  pin set  up
  pinMode(rainbow_pin, INPUT);
  pinMode(off_pin, INPUT);

// init the audio decoder
  MSGEQ7.begin();

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


   if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
  
   server.on("/off", lights_off);
   server.on("/rainbow", lights_rainbow);
   server.on("/white", lights_white);
   server.on("/audio", lights_audio); 
   server.onNotFound(handleNotFound);

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}  

int count = 0;
int highval = 0;
int x = 0;
int prevTime = 0;
int READ_DELAY = 75;
char end_of_req = 'r';
int off_val = 0;
int full_byte = 255;
 
void loop() {
  
//  check for rainbow send
//  val = digitalRead(rainbow_pin);
//  val = 1; 
  server.handleClient();                    // Listen for HTTP requests from clients

  if (val == 1){  
    Serial.println("server sending rainbow.");
        
    long thisHue = beat8(10,255);
    char data[2]={255, 300};
//    thisHue = 0-thisHue;
    // Send packet to client 1

    UDP.beginPacket(IP1, UDP_PORT);
//    thisHue = 0 - thisHue;
//    UDP.write(String("D"));
    UDP.write("1200");
    UDP.endPacket();
    
    // Send packet to client 2
    UDP.beginPacket(IP2, UDP_PORT); 
//    UDP.write(String(thisHue));
//    byte data[5]={255, 255, 255, 255, thisHue};
    UDP.write("1200");
    UDP.endPacket();
    delay(10);
//    Serial.print("sending the value: ");
//    Serial.println(thisHue);
//    return;
//     EVERY_N_MILLISECONDS( 20 ) { sendUpdate();; } // slowly cycle the "base color" through the rainbow
  }
  
////  check for off send
//  off_val = digitalRead(off_pin);
  else if (val == 2){  
      Serial.println("server sending off. real value:1");
    // Send packet to client 1
    UDP.beginPacket(IP1, UDP_PORT);
    UDP.write("1", 255);
    UDP.endPacket();
    
    // Send packet to client 2
    UDP.beginPacket(IP2, UDP_PORT);
    UDP.write("1", 255);
    UDP.endPacket();
//    return;
  } 
  else if (val == 3) {
//    send white
// Send packet to client 1
    UDP.beginPacket(IP1, UDP_PORT);
    UDP.write("2", 255);
    UDP.endPacket();
    
    // Send packet to client 2
    UDP.beginPacket(IP2, UDP_PORT);
    UDP.write("2", 255);
    UDP.endPacket();
  }
  else if (val == 4) {
  //  read data from audio decoder
      highval = highval-20;
      if (millis() - prevTime > READ_DELAY) {
        prevTime = millis();
        MSGEQ7.read();
        for (int i = 0; i < 6; i++)
        {
          x = MSGEQ7.get(i);
          if (highval < x) {
            highval = x;
          }  
        }
  //  all pins off, send sound data
  //    highval = String(highval);
      char send_highval[4];
      String str;
      str=String(highval);
      str.toCharArray(send_highval,4);
      
      Serial.print("server sending: ");
      Serial.println(highval);
      
  // Send packet to client 1
      UDP.beginPacket(IP1, UDP_PORT);
      UDP.write(send_highval);
      UDP.endPacket();
      
      // Send packet to client 2
      UDP.beginPacket(IP2, UDP_PORT);
      UDP.write(send_highval);
      UDP.endPacket();
      
      if (highval <= 0){
         highval = 1;
      }
      Serial.println();
    }
  }
}

int sendUpdate(){
  
      UDP.beginPacket(IP1, UDP_PORT);
      UDP.write("1201");
      UDP.endPacket();
      
      // Send packet to client 2
      UDP.beginPacket(IP2, UDP_PORT);
      UDP.write("1201");
      UDP.endPacket();
        
}
