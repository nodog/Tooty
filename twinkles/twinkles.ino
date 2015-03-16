#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN 6
#define GRIDHEIGHT 16
#define GRIDWIDTH 16

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 2, 2, PIN,
  NEO_MATRIX_BOTTOM  + NEO_MATRIX_LEFT +
  NEO_TILE_BOTTOM    + NEO_TILE_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE +
  NEO_TILE_COLUMNS   + NEO_TILE_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

byte hues[GRIDHEIGHT][GRIDWIDTH]; 
byte brightnesses[GRIDHEIGHT][GRIDWIDTH]; 


void setup() {
  randomSeed(analogRead(0));
  matrix.begin();
  matrix.fillScreen(0);
  for (byte i = 0; i < 16; i++) {
    for (byte j = 0; j < 16; j++) {
        if (random(2) == 0) {
          hues[i][j] = random(16) + 192;
        } else {
          hues[i][j] = random(16) + 92;
        }
        brightnesses[i][j] = 0;        
        if (random(1024) == 0) {
          brightnesses[i][j] = random(64) + 64;
        }
    }
  }
}

int x    = matrix.width();
int pass = 0;

void loop() {
  for (byte i = 0; i < 16; i++) {
    for (byte j = 0; j < 16; j++) {
      if (random(1024) == 0) {
        //hues[i][j] = random(256);
        brightnesses[i][j] = random(64) + 64;
      }
      if (brightnesses[i][j] > 0) {
        for (int8_t k = -1; k <= 1; k++) {
          for (int8_t l = -1; l <= 1; l++) {
            if (((i + k) >= 0) && ((i + k) < GRIDWIDTH) && ((j + l) >= 0) && ((j + l) < GRIDHEIGHT)) {  
              brightnesses[i + k][j + l] = max( 0, brightnesses[i + k][j + l] - 1);
              byte sub = 0;
              if (0 != k) {
                sub += 1;
              }
              if (0 != l) {
                sub += 1;
              }
              matrix.drawPixel(i + k, j + l, brightWheel( (hues[i][j] + 10*sub) % 256, max(0, brightnesses[i][j] - 30*sub)));
            }
          }
        }
      }
    }
  }
  matrix.show();
  pass++;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t brightWheel(byte WheelPos, byte bri) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return matrix.Color(bri*(255 - WheelPos * 3)/256, 0, bri*WheelPos * 3/256);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return matrix.Color(0, bri*WheelPos * 3/256, bri*(255 - WheelPos * 3)/256);
  } else {
   WheelPos -= 170;
   return matrix.Color(bri*WheelPos * 3/256, bri*(255 - WheelPos * 3)/256, 0);
  }
}
