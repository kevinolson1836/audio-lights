#include <SPI.h>
#include <WiFi.h>
#define WIFI_SSID "fuck"
#define WIFI_PASS "fuckthewifi"
int status = WL_IDLE_STATUS;

WiFiServer server(4210);

void setup() {
  // initialize serial:
  Serial.begin(9600);
  Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);

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
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client == true) {
       // read bytes from the incoming client and write them back
    // to any clients connected to the server:
    server.write("test data!");
  }
}
