#include "FastLED.h"

#define NUM_FANS 4
#define NUM_FAN_LEDS 8

#define NUM_STRIP1_LEDS 22 //top
#define NUM_STRIP2_LEDS 15 //gpu
#define NUM_STRIP3_LEDS 9 //bottom front
#define NUM_STRIP4_LEDS 16 //bottom side
#define NUM_STRIP5_LEDS 93 //monitors
#define NUM_STRIP6_LEDS 130 //desk

byte rainbow_sync = 0;
int Delay = 3;

CRGB fans[NUM_FANS][NUM_FAN_LEDS];
CRGB strip1[NUM_STRIP1_LEDS];
CRGB strip2[NUM_STRIP2_LEDS];
CRGB strip3[NUM_STRIP3_LEDS];
CRGB strip4[NUM_STRIP4_LEDS];
CRGB strip5[NUM_STRIP5_LEDS];
CRGB strip6[NUM_STRIP6_LEDS];

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
  Serial.println("hello");
  FastLED.addLeds<NEOPIXEL, 2>(fans[0], NUM_FAN_LEDS);
  FastLED.addLeds<NEOPIXEL, 3>(fans[1], NUM_FAN_LEDS);
  FastLED.addLeds<NEOPIXEL, 4>(fans[2], NUM_FAN_LEDS);
  FastLED.addLeds<NEOPIXEL, 5>(fans[3], NUM_FAN_LEDS);
  FastLED.addLeds<NEOPIXEL, 8>(strip1, NUM_STRIP1_LEDS);
  FastLED.addLeds<NEOPIXEL, 9>(strip2, NUM_STRIP2_LEDS);
  FastLED.addLeds<NEOPIXEL, 10>(strip3, NUM_STRIP3_LEDS);
  FastLED.addLeds<NEOPIXEL, 11>(strip4, NUM_STRIP4_LEDS);
  FastLED.addLeds<NEOPIXEL, 6>(strip5, NUM_STRIP5_LEDS);
  FastLED.addLeds<NEOPIXEL, 7>(strip6, NUM_STRIP6_LEDS);
}

void loop() {
  if (Serial3.available()) {
    String message = Serial3.readStringUntil('\r\n');
    
    Serial.print("esp8266: ");
    Serial.println(message);
  }
  //led
  rainbow_sync+=4;
  for(int x=0;x<NUM_FANS;x++){
    for(int i=0;i<NUM_FAN_LEDS;i++){
      fans[x][i].setHSV(rainbow_sync+(((float)i/NUM_FAN_LEDS)*255),255,255);
    }
  }
  for(int i=0;i<NUM_STRIP1_LEDS;i++){
    strip1[i].setHSV(rainbow_sync,255,255);
  }
  for(int i=0;i<NUM_STRIP2_LEDS-1;i++){
    strip2[i].setHSV(rainbow_sync+((float)i/NUM_STRIP2_LEDS*255),255,255);
  }
  for(int i=0;i<NUM_STRIP3_LEDS;i++){
    strip3[i].setHSV(rainbow_sync+((float)i/NUM_STRIP3_LEDS*255),255,255);
  }
  for(int i=0;i<NUM_STRIP4_LEDS;i++){
    strip4[i].setHSV(rainbow_sync+((float)i/NUM_STRIP4_LEDS*255),255,255);
  }
  for(int i=0;i<NUM_STRIP5_LEDS;i+=2){
    strip5[i].setHSV(rainbow_sync+(255 - (float)i/NUM_STRIP5_LEDS*255),255,255);
  }
  for(int i=0;i<NUM_STRIP6_LEDS;i+=2){
    strip6[i].setHSV(rainbow_sync+((float)i/NUM_STRIP6_LEDS*255),255,255);
  }
  FastLED.show();
  //delay(Delay);
}
