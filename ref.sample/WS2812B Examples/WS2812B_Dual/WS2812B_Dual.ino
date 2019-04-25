/*

Demonstrates the use of the Parallax WS2812B RGB Led Module (#28085)

This demo cycles two LEDs through the three primary colors, red, blue, and
green, slowly fading in for each color.

For a demonstration using just one WS2812B RGB LED module, please see the 
'WS2812B_Single' sketch.

------------

This sketch REQUIRES the use of a third-party library, Adafruit_NeoPixel. 
Refer to the 'WS2812B_Single' sketch for important setup information
for this library.

This example code is for the Arduino Uno and direct compatible boards, using the
Arduino 1.0 or later IDE software. It has not been tested, nor designed for, other 
Arduino boards, including the Arduino Due.

------------

Connections:
First LED    Arduino
GND          GND
5V           5V
SI           Digital Pin 6

First LED    Second LED
GND          GND
5V           5V
SO           SI

Note: Both the 5V and GND connections on the LED modules are thru-connected.
You may connect ground and power to either side of the module.

*/

#include <Adafruit_NeoPixel.h>         // Include Adafruit NeoPixel library
#define PIN            7               // First LED on digital pin 6
#define NUMLEDS        2               // Use total of 2 LEDs

// Create 'leds' object to drive LEDs
Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUMLEDS, PIN, NEO_GRB + NEO_KHZ800);

int wait = 5;                         // Standard wait delay of 5 milliseconds 

void setup() {
  leds.begin();                       // Initialize 'leds' object

  //Make sure both LEDs are off to demonstrate start of sketch
  leds.setPixelColor(0, leds.Color(0, 0, 0));  // Turn off LED0
  leds.setPixelColor(1, leds.Color(0, 0, 0));  // Turn off LED1
  leds.show();                        // Set new values
  delay(500);
  
}

// Loop through changing R, G, and B colors
// Each color fades in from off (i=0) to fully on (i=255)
void loop() {
  for(int i=0; i<255; i++) {            // Count up color units from 0 to 255
    leds.setPixelColor(0, leds.Color(i, 0, 0));  // Color mix for LED0
    leds.setPixelColor(1, leds.Color(0, i, 0));  // Color mix for LED1
    leds.show();                        // Set the LEDs
    delay(wait);                        // Delay for visual effect
  }
  for(int i=0; i<255; i++) {            // Loop through next color change
    leds.setPixelColor(0, leds.Color(0, i, 0)); 
    leds.setPixelColor(1, leds.Color(0, 0, i)); 
    leds.show(); 
    delay(wait); 
  }
  for(int i=0; i<255; i++) {            // Loop through next color change
    leds.setPixelColor(0, leds.Color(0, 0, i));
    leds.setPixelColor(1, leds.Color(i, 0, 0)); 
    leds.show(); 
    delay(wait);
  }
}
