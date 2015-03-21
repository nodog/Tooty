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
#define MAXBRIDELTA 8
#define SLOWCHANGESKIP 1
#define MAXCHANGECHANCE 80
#define MINCHANGECHANCE 10
#define MAXFRAMEDELAY 80
#define MINFRAMEDELAY 0
#define NOEASTERSTRENGTH 3

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 2, 2, PIN,
  NEO_MATRIX_BOTTOM  + NEO_MATRIX_LEFT +
  NEO_TILE_BOTTOM    + NEO_TILE_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE +
  NEO_TILE_COLUMNS   + NEO_TILE_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
  
//const uint16_t colors[] = {
//  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };
  
uint8_t cells[GRIDHEIGHT/2][GRIDWIDTH][COLORS];
uint8_t firstLine[GRIDWIDTH][COLORS];
byte change_chance;
byte frame_delay;
int slow_change_counter;

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(9600);
  matrix.begin();
  change_chance = (MAXCHANGECHANCE + MINCHANGECHANCE)/2;
  frame_delay = (MAXFRAMEDELAY + MINFRAMEDELAY)/2;
  slow_change_counter = 0;
  matrix.fillScreen(0);
  for (int i = 0; i < GRIDWIDTH/2; i++) {
    for (int k = 0; k < COLORS; k++) {
      firstLine[i][k] = random(MAXBRIGHT);
      for (int j = 0; j < GRIDHEIGHT; j++) {
        cells[i][j][k] = 0;
      }
    } 
  } 
}

void loop() {
  slow_change_counter++;
  if (0 == (slow_change_counter % SLOWCHANGESKIP)) {
    int temp_fd = frame_delay + random(-1, 2);
    frame_delay = (byte) constrain(temp_fd, MINFRAMEDELAY, MAXFRAMEDELAY);
    int temp_cc = change_chance + random(-1, 2);
    change_chance = (byte) constrain(temp_cc, MINCHANGECHANCE, MAXCHANGECHANCE);
    Serial.print("frame_delay = ");
    Serial.print(frame_delay);
    Serial.print("  change_chance = ");
    Serial.println(change_chance);
    slow_change_counter = 0;
  }
  delay(frame_delay);
  // update firstLine
  for (int i = 0; i < GRIDWIDTH/2; i++) {
    uint8_t pix_colors[COLORS];     
    for (int k = 0; k < COLORS; k++) {
      int pull_i = i;
      if (random(100) < change_chance) {
        byte change_type = random(2);
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
        }
      }
      uint8_t temp_c = firstLine[pull_i][k] + random(-MAXBRIDELTA + 1, MAXBRIDELTA);
      firstLine[i][k] = constrain(temp_c, 0, MAXBRIGHT);
    }
  }
  
  // draw cells
  for (int i = 0; i < GRIDWIDTH/2; i++) {
    for (int j = 0; j < GRIDHEIGHT; j++) {
      uint8_t pix_colors[COLORS];
      for (int k = 0; k < COLORS; k++) {
        if ( j < (GRIDHEIGHT - 1) ) {
          cells[i][j][k] = cells[i][j + 1][k];
        } else {
          cells[i][j][k] = firstLine[i][k];
        }
        pix_colors[k] = cells[i][j][k];
        if ((0 == i) && (0 == j) && (0 == k)) {
          Serial.print( "                           red @ 0 0 = ");
          Serial.println(pix_colors[k]);
        }
      }
      uint32_t pix_color = no_easter(pix_colors);
        //matrix.Color(pix_colors[0], pix_colors[1], pix_colors[2]);

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
  // reduce green 
  pix_colors[1] = pix_colors[1] * 3 / 4;

  // debug
  //pix_colors[1] = 0;
  //pix_colors[2] = 0;
  return matrix.Color(pix_colors[0], pix_colors[1], pix_colors[2]);
}
