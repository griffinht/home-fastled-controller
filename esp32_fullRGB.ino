
#include <WiFi.h>
#include "arduino-secrets.h"

const char* NETWORK_SSID = SECRET_SSID;
const char* NETWORK_PASSWORD = SECRET_PASSWORD;
const int PORT = 80;

WiFiServer server(PORT);

String DEFAULT_PATTERN = "off";
String pattern = DEFAULT_PATTERN;


void setup() {
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.print(NETWORK_SSID);
  
  WiFi.begin(NETWORK_SSID, NETWORK_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("\nConnected with local IP address ");
  Serial.println(WiFi.localIP());

  server.begin();

  Serial.print("Listening on port ");
  Serial.println(PORT);
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = "";
    boolean newLine = 0;
    while (client.connected() && client.available()) {
      char c = client.read();
      request += c;
      if (c == '\n') {
        if (newLine) {
          String path = "";
          int s = request.indexOf("GET /");
          int e = request.indexOf(' ', s + 4);
          path = request.substring(s + 4, e - s);
          
          client.println("HTTP 1.1 200 OK");
          client.println("Content-type:text/html");
          client.println("Connection: close");
          client.println();
          if (path.equals("/")) {
            Serial.println("empty");
            client.println("hello there, do as you wish");
          } else if (path.startsWith("/pattern=")) {
            pattern = path.substring(9);
            Serial.print("Pattern set to ");
            Serial.println(pattern);
          }
          break;   
        } else {
          newLine = true; 
        }
      }
    }
    client.stop();
  }
}
