#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN 6
#define GRIDWIDTH 16
#define GRIDHEIGHT 16
#define COLORS 3
#define MAXBRIGHT 93 
#define MAXBRIDELTA 3
#define CHANGECHANCE 1
#define FRAMEDELAY 0
#define NOEASTERSTRENGTH 3

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 2, 2, PIN,
  NEO_MATRIX_BOTTOM  + NEO_MATRIX_LEFT +
  NEO_TILE_BOTTOM    + NEO_TILE_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE +
  NEO_TILE_COLUMNS   + NEO_TILE_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
  
uint32_t brightWheel(byte WheelPos, byte bri);

//const uint16_t colors[] = {
//  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };
  
uint8_t cells[GRIDHEIGHT/2][GRIDWIDTH][COLORS];

void setup() {
  randomSeed(analogRead(0));
  matrix.begin();
  matrix.fillScreen(0);
  for (int i = 0; i < GRIDWIDTH/2; i++) {
    for (int j = 0; j < GRIDHEIGHT; j++) {
      for (int k = 0; k < COLORS; k++) {
        cells[i][j][k] = random(MAXBRIGHT);
      }
    } 
  } 
}

void loop() {
  delay(FRAMEDELAY);
  for (int i = 0; i < GRIDWIDTH/2; i++) {
    for (int j = 0; j < GRIDHEIGHT; j++) {
      for (int k = 0; k < COLORS; k++) {
        int pull_i = i;
        int pull_j = j;
        if (random(10000) < CHANGECHANCE) {
          byte change_type = random(4);
          switch (change_type) {
            case 0:
              if (0 != i) {
                pull_i = i - 1;
              }
              break;
            case 1:
              if ((GRIDWIDTH/2 -1) != i) {
                pull_i = i + 1;
              }
              break;
            case 2:
              if (0 != j) {
                pull_j = j - 1;
              }
              break;
            case 3:
              if ((GRIDHEIGHT - 1) != j) {
                pull_j = j + 1;
              }
              break;

           }
        }
        uint8_t temp_c = cells[pull_i][pull_j][k] + random(-MAXBRIDELTA, MAXBRIDELTA);
        cells[i][j][k] = constrain(temp_c, 0, MAXBRIGHT);
      }
    }
  }
  
  // draw cells
  for (int i = 0; i < GRIDWIDTH/2; i++) {
    for (int j = 0; j < GRIDHEIGHT; j++) {
      uint8_t pix_colors[COLORS];
      for (int k = 0; k < COLORS; k++) {
        pix_colors[k] = cells[i][j][k];
      }
      uint32_t pix_color = no_easter(pix_colors);
      matrix.drawPixel(i, j, pix_color);
      matrix.drawPixel(GRIDWIDTH - 1 - i, j, pix_color); 
    }
  }
  matrix.show();
}

uint32_t no_easter(uint8_t pix_colors[]) {
  byte min_index = 0;
  byte max_index = 0;
  for (int k = 1; k < COLORS; k++) {
    if (pix_colors[k] < pix_colors[min_index]) {
     min_index = k;
    }
    if (pix_colors[k] > pix_colors[max_index]) {
     max_index = k;
    }
  }
  if (0 != pix_colors[max_index]) {
    for (int k = 0; k < COLORS; k++) {
      pix_colors[k] = map(pix_colors[k], pix_colors[min_index], pix_colors[max_index], 
          pix_colors[min_index]/NOEASTERSTRENGTH + 1, pix_colors[max_index]);
    }
  }
  return matrix.Color(pix_colors[0], pix_colors[1], pix_colors[2]);
}
