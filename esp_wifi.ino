#include <incbin.h>
#include <ESP8266WiFi.h>
#include "html.h"
INCTXT(Body, "index.html");

WiFiServer server(80);
// Variable to store the HTTP request
String header;

QHTTP *qhttp;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

void WifiConnect() {
  int nets = WiFi.scanNetworks();
  Serial.printf("Networks: %d\n");
  for (int k = 0; k < nets; ++k) {
    const bss_info *info = WiFi.getScanInfoByIndex(k);
    Serial.printf("[%d] -> %s\n", k+1, info->ssid);
  }

  int idx = -1;
  while (idx < 0) {
    String res = Serial.readStringUntil('\n');
    int tidx = 0;
    if (res.length() > 0 && (tidx = res.toInt()) > 0 && tidx <= nets) {
      //Serial.printf("You entered: %d\n", tidx);
      idx = tidx - 1;
    }
  }
  const bss_info *conn = WiFi.getScanInfoByIndex(idx);

  String res("");
  Serial.printf("Selected %s\n", conn->ssid);
  if (conn->authmode != AUTH_OPEN) {
    Serial.print("Passkey required: ");
    while (res.length() < 1) {
      res = Serial.readStringUntil('\n');
    }
    Serial.print("\n");
    WiFi.begin((const char *)conn->ssid, res.c_str());
  } else {
    WiFi.begin((const char *)conn->ssid);
  }

  Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.printf("\nConnected to %s\n", conn->ssid);
  Serial.printf(" - IP: %s\n", WiFi.localIP().toString().c_str());
  Serial.printf(" - Gateway: %s\n", WiFi.gatewayIP().toString().c_str());
  Serial.printf(" - Broadcast: %s\n", WiFi.broadcastIP().toString().c_str());
}

void WifiHost() {
  IPAddress local_ip(192,168,4,1);
  IPAddress gateway(192,168,4,1);
  IPAddress subnet(255,255,255,0);
  printf("AP Configure - %s\n", WiFi.softAPConfig(local_ip, gateway, subnet) ? "Pass" : "Fail");
  
  printf("AP Create - %s\n", WiFi.softAP("ESP8266") ? "Pass" : "Fail");
}

void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.println("Boot complete");
  WifiHost();
  server.begin();
  qhttp = (QHTTP*)malloc(QHTTPSZ);
}

void loop() {
  WiFiClient client = server.available();
  parse("yuh");

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) { // Check for end of request   
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
            }
            client.print(headers);
            client.printf("%s\n\n", gBodyData);
            break;
          } else { // if newline
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
