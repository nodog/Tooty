#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN 6
#define GRIDWIDTH 16
#define GRIDHEIGHT 16

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 2, 2, PIN,
  NEO_MATRIX_BOTTOM  + NEO_MATRIX_LEFT +
  NEO_TILE_BOTTOM    + NEO_TILE_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE +
  NEO_TILE_COLUMNS   + NEO_TILE_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
  
uint32_t brightWheel(byte WheelPos, byte bri);

//const uint16_t colors[] = {
//  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };
  
uint8_t cells[GRIDHEIGHT][GRIDWIDTH]; 

int calcNeighbors(uint8_t arr[][GRIDWIDTH], int x, int y) {
  int sum = 0;
  for( int i = x - 1; i <= x + 1; i++) {
    for( int j = y - 1; j <= y + 1; j++) {
      int index, jndex;
      if (i < 0) {
        index = GRIDWIDTH - 1;
      } else if (i == GRIDWIDTH) {
        index = 0; 
      } else {
        index = i;
      }
      if (j < 0) {
        jndex = GRIDHEIGHT - 1;
      } else if (j == GRIDHEIGHT) {
        jndex = 0; 
      } else {
        jndex = j;
      }
      if (arr[index][jndex] > 0){
        sum += 1;
      }
    } 
  } 
  if (arr[x][y] > 0) {
    sum -= 1;
  }      
  return sum;
}

void deepCopy(uint8_t fromArr[][GRIDWIDTH], uint8_t toArr[][GRIDWIDTH]) {
  for (int i = 0; i < GRIDWIDTH; i++) {
    for (int j = 0; j < GRIDHEIGHT; j++) {
      toArr[i][j] = fromArr[i][j];
    }
  }
}

void setup() {
  randomSeed(analogRead(0));
  matrix.begin();
  //matrix.setTextWrap(false);
  //matrix.setBrightness(40);
  //matrix.setTextColor(colors[0]);
  matrix.fillScreen(0);
  for (int i = 0; i < GRIDWIDTH; i++) {
    for (int j = 0; j < GRIDHEIGHT; j++) {
      if ( random(100) < 0 ) {
        cells[i][j] = 1;
      } else {
        cells[i][j] = 0;
      }
    }
  }
}

int x    = matrix.width();
int pass = 0;

void loop() {
  uint8_t newcells[GRIDHEIGHT][GRIDWIDTH];

  //delay(10);
  for (int16_t i = 0; i < 16; i++) {
    for (int16_t j = 0; j < 16; j++) {
      int neighbors = calcNeighbors(cells, i, j);
      //println( neighbors );
      if (cells[i][j] > 0) {
        if (neighbors < 2) {
          newcells[i][j] = 0;
        } else if ( (2 == neighbors) || (3 == neighbors)) {
        //if ( (2 == neighbors) || (3 == neighbors) || (4 == neighbors)) {
          newcells[i][j] = (cells[i][j] + 1) % 256;
        } else if ( neighbors > 3 ) {
          newcells[i][j] = 0;
        }
      } else if (cells[i][j] == 0) {
        //if ((1 == neighbors) || (2 == neighbors)  || (3 == neighbors)) {
        //if ((2 == neighbors)  || (3 == neighbors)) {
        if ((3 == neighbors)) {
          newcells[i][j] = 1;
        }
      }
      byte bri = 100;
      if (0 < cells[i][j]) {
        bri = 120;
      } 
     
      matrix.drawPixel(i, j, brightWheel( (cells[i][j] * 12 - pass/16) % 256, bri));
      //matrix.drawPixel(i, j, brightWheel( (pass) % 256, bri));
    }
  }
  //matrix.drawPixel(tx, ty, matrix.Color(tcr, tcg, tcb));
  //matrix.fillScreen(matrix.Color(tcr, tcg, tcb));
  matrix.show();
  deepCopy(newcells, cells);
  if ( random(100) < 10 ) {
    uint8_t xBegin = random(GRIDWIDTH);
    uint8_t yBegin = random(GRIDWIDTH);
    for( int i = xBegin; i < xBegin + 3; i++) {
      for( int j = yBegin; j < yBegin + 3; j++) {
        if (random(100) < 80) {
          cells[i%GRIDWIDTH][j%GRIDHEIGHT] += 1;
        }
      }
    }      
  }
  delay(100);
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
