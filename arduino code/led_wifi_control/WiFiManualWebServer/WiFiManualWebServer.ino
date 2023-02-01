/*
    This sketch demonstrates how to set up a simple HTTP-like server.
    The server will set a GPIO pin depending on the request
      http://server_ip/gpio/0 will set the GPIO2 low,
      http://server_ip/gpio/1 will set the GPIO2 high
    server_ip is the IP address of the ESP8266 module, will be
    printed to Serial when the module is connected.
*/

#include <ESP8266WiFi.h>
#include <MD_MSGEQ7.h>

#ifndef STASSID
#define STASSID "fuck"
#define STAPSK  "fuckthewifi"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(37018);

//MD_MSGEQ7
#define DATA_PIN    A0
#define RESET_PIN   13
#define STROBE_PIN  5

int off_prev_time = 0; 
int off_DELAY = 1;
int rainbow_pin =15;

int off_pin = 16;

MD_MSGEQ7 MSGEQ7(RESET_PIN, STROBE_PIN, DATA_PIN);

void setup() {
  Serial.begin(57600);

  // prepare LED
//  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);
  
  pinMode(rainbow_pin, INPUT);
  
  pinMode(off_pin, INPUT);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));

  // Start the server
  server.begin();
  Serial.println(F("Server started"));

  // Print the IP address
  Serial.println(WiFi.localIP());

  MSGEQ7.begin();

}

int count = 0;
int highval = 0;
int x = 0;
int prevTime = 0;
int READ_DELAY = 75;
char end_of_req = 'r';
int val = 0;
int off_val = 0;
void loop() {
  highval = highval-20;
  if (millis() - prevTime > READ_DELAY) 
  {
    prevTime = millis();

    MSGEQ7.read();

  for (int i = 0; i < 6; i++)
  {
    x = MSGEQ7.get(i);
    if (highval < x) {
      highval = x;
    }  
  }
  }

  // Check if a client has connected
  WiFiClient client = server.available();
//  if (!client) {
////     Serial.println("sever not aviable");
//
//    return;
//  }
  Serial.print("highval: ");
  client.setTimeout(50); // default is 1000
  Serial.println(highval);

  val = digitalRead(rainbow_pin);
  if (val == 1){
    
      Serial.println("sending rainbow. real value:1200");
      client.print(1200);
      return;
  }
  // Send the response to the client
  // it is OK for multiple small client.print/write,
  // because nagle algorithm will group them into one single packet
   off_val = digitalRead(off_pin);
   Serial.print("off val: ");
   Serial.println(off_val);
   if (!off_val){
    Serial.println("sending off. real value:1");
    client.print(1); // change this back to 1 after button has been added ????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
    return;
}else{
  Serial.print("sending: ");
  Serial.println(highval);
  client.print(highval);
}
    //print the data from music 
//  client.println(end_of_req);
//  Serial.println(highval);
//  highval = highval-20;
  if (highval <= 0){
    highval = 1;
  }
  Serial.println();
}
