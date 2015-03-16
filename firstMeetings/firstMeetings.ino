#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN 6

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 2, 2, PIN,
  NEO_MATRIX_BOTTOM  + NEO_MATRIX_LEFT +
  NEO_TILE_BOTTOM    + NEO_TILE_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE +
  NEO_TILE_COLUMNS   + NEO_TILE_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

void setup() {
  randomSeed(analogRead(0));
  matrix.begin();
  //matrix.setTextWrap(false);
  //matrix.setBrightness(40);
  //matrix.setTextColor(colors[0]);
  matrix.fillScreen(0);
}

int x    = matrix.width();
int pass = 0;

void loop() {
  //stadiumCellPhones
  if (random(2) == 1) {
    //delay(100);
    matrix.fillScreen(0);
  }
  int16_t tx = pass%16;
  int16_t ty = (pass/16)%16;
  uint8_t tcr = (pass/8)%128;
  uint8_t tcg = (pass/1024)%64;
  uint8_t tcb = (pass/2048)%64;
  for (int16_t i = 0; i < 16; i++) {
    for (int16_t j = 0; j < 16; j++) {
      matrix.drawPixel(i, j, Wheel((i*j + pass)%256));
    
    }
  }
  //matrix.drawPixel(tx, ty, matrix.Color(tcr, tcg, tcb));
  //matrix.fillScreen(matrix.Color(tcr, tcg, tcb));
  matrix.show();
  pass++;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
