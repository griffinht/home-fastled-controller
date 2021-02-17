#include "FastLED.h"

#define NUM_FANS 4
#define NUM_FAN_LEDS 8

#define NUM_STRIP1_LEDS 22 //top
#define NUM_STRIP2_LEDS 15 //gpu
#define NUM_STRIP3_LEDS 9 //bottom front
#define NUM_STRIP4_LEDS 16 //bottom side
#define NUM_STRIP5_LEDS 93 //monitors
#define NUM_STRIP6_LEDS 130 //desk

#define DELTA 255/NUM_STRIP1_LEDS

byte rainbow_sync = 0;

CRGBArray<NUM_FAN_LEDS> fans[NUM_FANS];
CRGBArray<NUM_STRIP1_LEDS> strip1;
CRGBArray<NUM_STRIP2_LEDS> strip2;
CRGBArray<NUM_STRIP3_LEDS> strip3;
CRGBArray<NUM_STRIP4_LEDS> strip4;
CRGBArray<NUM_STRIP5_LEDS> strip5;
CRGBArray<NUM_STRIP6_LEDS> strip6;

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
  long start = micros();
  if (Serial3.available()) {
    String message = Serial3.readStringUntil('\r\n');
    
    Serial.print("esp8266: ");
    Serial.println(message);
  }
  //led
  rainbow_sync++;
  for(int i=0;i<NUM_FANS;i++){
    fans[i].fill_rainbow(rainbow_sync, 255/NUM_FAN_LEDS);
  }
  strip1.fill_solid(CHSV(rainbow_sync, 255, 255));
  strip2.fill_rainbow(rainbow_sync, DELTA);
  strip4.fill_rainbow(rainbow_sync - 4*DELTA, DELTA);//makes strip4 look combined with strip3
  strip3.fill_rainbow(rainbow_sync - (4 + NUM_STRIP3_LEDS)*DELTA, DELTA);
  strip5.fill_rainbow(rainbow_sync, 255 - (255/NUM_STRIP5_LEDS));//reverse direction
  for (int i = 0; i < NUM_STRIP5_LEDS; i++) {
    if (i % 2 == 0) {
      strip5[i] = 0;
    } else {
      //strip6[i] *= 255 - 255/NUM_STRIP5_LEDS;
    }
  }
  //strip6.fill_rainbow(rainbow_sync, 255/NUM_STRIP6_LEDS);
  strip6.fill_rainbow(rainbow_sync, 255/NUM_STRIP6_LEDS);
  for (int i = 0; i < NUM_STRIP6_LEDS; i++) {
    if (i % 2 == 0) {
      strip6[i] = 0;
    } else {
      //strip6[i] *= 255 - 255/NUM_STRIP5_LEDS;
    }
  }
  FastLED.show();
  //delay(Delay);
  if (rainbow_sync % 10 == 0) {
    Serial.print((micros() - start) / 1000);
    Serial.println("ms");
  }
}
