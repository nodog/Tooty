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

int fx[COLORS];
int fy[COLORS];
short ftx[COLORS];
short fty[COLORS];
byte fphix[COLORS];
byte fphiy[COLORS];
byte fphitx[COLORS];
byte fphity[COLORS];
byte ftt[COLORS];

void setup() {
  randomSeed(analogRead(0));
  matrix.begin();
  matrix.fillScreen(0);
  for (int i = 0; i < COLORS; i++) {
    fx[i] = random(1024);
    fy[i] = random(1024);
    ftx[i] = random(-6, 6);
    fty[i] = random(-6, 6);
    fphix[i] = random(6);
    fphiy[i] = random(6);
    fphitx[i] = random(6);
    fphity[i] = random(6);
    ftt[i] = random(4) + 1;
    
  }
}

int pass = 0;
float rad = 0.0;

void loop() {

  byte phix[COLORS];
  byte phiy[COLORS];
  int t[COLORS];
  
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {      
      for (int k = 0; k < COLORS; k++) {
        t[k] = icos2p(pass*ftt[k])/4;    
        phix[k] = (icos2p(fphix[k]*i + pass/fphitx[k]) + icos2p(fphiy[k]*j + pass/fphity[k]))/2;
      }
      //phix[0] = fphix[0];
      //phix[1] = fphix[1];
      //phix[2] = fphix[2];
      //phiy[0] = fphiy[0];
      //phiy[1] = fphiy[1];
      //phiy[2] = fphiy[2];
      unsigned long tempr = (icos2p(icos2p(t[0]/ftx[0] + fx[0]*i + phix[0]) * icos2p(t[1]/fty[0] + fy[0]*j + phiy[0])/64));
      unsigned long tempg = (icos2p(icos2p(t[1]/ftx[1] + fx[1]*i + phix[1]) * icos2p(t[2]/fty[1] + fy[1]*j + phiy[1])/64));
      unsigned long tempb = (icos2p(icos2p(t[2]/ftx[2] + fx[2]*i + phix[2]) * icos2p(t[0]/fty[2] + fy[2]*j + phiy[2])/64));
      
      //unsigned long temp = i*256;
      uint8_t tcr = ((uint8_t) (tempr))/3;
      uint8_t tcg = ((uint8_t) (tempg))/8;; //icos2p(pass*3 + 20 * i - 31 * j + 400)/8;
      uint8_t tcb = ((uint8_t) (tempb))/6;; //icos2p(-pass*4 + 73 * i + 42 * j + 300)/4;

      matrix.drawPixel(i, j, matrix.Color(tcr, tcg, tcb));
    }
  }
  matrix.show();
  pass++;
}

// integer cosine using 2 parabolas
// input any int angle (0-255 is one revolution)
// output int value from 0-255. icos2p(64) = 128 for reference
int icos2p(int angle) {  
  long x = abs(angle) % 256;
  if (x > 128) {
    x = 256 - x;
  }
  long y = 0;
  if (x < 64) {
    y = -x*x/32 + 255;
  } else {
    y = x*x/32 -8*x + 512;
  }
  return (int) y;
}
