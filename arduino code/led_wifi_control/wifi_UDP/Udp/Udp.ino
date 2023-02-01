#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
   
// Set WiFi credentials
#define WIFI_SSID "fuck"
#define WIFI_PASS "fuckthewifi"
#define UDP_PORT 4210
#define UDP_IP "192.168.1.140"

// UDP
WiFiUDP UDP;
char packet[255];
char reply[] = "Packet received!";
   
void setup() {
  // Setup serial port
  Serial.begin(115200);
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
void loop() {
  
    // Send packet
    int x = UDP.beginPacket(UDP_IP, UDP_PORT);
    UDP.write(reply);
    UDP.endPacket();
    Serial.println(x);
}
