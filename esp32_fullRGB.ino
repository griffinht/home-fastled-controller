#define PORT 80

#define DEFAULT_PATTERN "off"

#define NUM_FANS 4
#define NUM_FAN_LEDS 8

#define NUM_STRIP1_LEDS 22
#define NUM_STRIP2_LEDS 16
#define NUM_STRIP3_LEDS 11
#define NUM_STRIP4_LEDS 93
#define NUM_STRIP5_LEDS 21

#define FASTLED_ESP32_I2S true

#include "FastLED.h"
#include <WiFi.h>
#include "arduino-secrets.h"

WiFiServer server(PORT);

String pattern = DEFAULT_PATTERN;
byte rainbow_sync = 0;
boolean off = true;

CRGB fans[NUM_FANS][NUM_FAN_LEDS];

void setup() {
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.print(SECRET_SSID);
  
  WiFi.begin(SECRET_SSID, SECRET_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("\nConnected with local IP address ");
  Serial.println(WiFi.localIP());

  server.begin();

  Serial.print("Listening on port ");
  Serial.println(PORT);

  FastLED.addLeds<NEOPIXEL, 12>(fans[0], NUM_FAN_LEDS);
  FastLED.addLeds<NEOPIXEL, 14>(fans[1], NUM_FAN_LEDS);
  FastLED.addLeds<NEOPIXEL, 27>(fans[2], NUM_FAN_LEDS);
  FastLED.addLeds<NEOPIXEL, 26>(fans[3], NUM_FAN_LEDS);
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
  
  rainbow_sync++;

  if (pattern.equals("off")) {
    if (!off) {
      off = true;
      solid(0, 0, 0);
      FastLED.show();
    }
  } else {
    off  = false;
    if (pattern.equals("rainbow")) {
      rainbow(rainbow_sync, 255, 255);
    } else {
      solid(0, 255, 255);
    }
    FastLED.show();
  }
}

void rainbow(int hOffset, int s, int v) {
  for (int i = 0; i < NUM_FANS; i++) {
    for (int j = 0; j < NUM_FAN_LEDS; j++) {
      fans[i][j].setHSV(hOffset + ((float) i / NUM_FANS * 255), s, v);
    }
  }
}

void solid(int h, int s, int v) {
  for (int i = 0; i < NUM_FANS; i++) {
    for (int j = 0; j < NUM_FAN_LEDS; j++) {
      fans[i][j].setHSV(h, s, v);
    }
  }
}
