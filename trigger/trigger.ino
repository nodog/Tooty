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
int ftt[COLORS];

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
    ftt[i] = random(32) - 16;
    if (ftt[i] >= 0) {
      ftt[i] += 1;
    }
    
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
        //t[k] = fnosc(pass*ftt[k])/4;    
        t[k] = pass*ftt[k]/16;    
        phix[k] = (fnosc(fphix[k]*i + pass/fphitx[k]) + fnosc(fphiy[k]*j + pass/fphity[k]))/2;
      }
      //phix[0] = fphix[0];
      //phix[1] = fphix[1];
      //phix[2] = fphix[2];
      //phiy[0] = fphiy[0];
      //phiy[1] = fphiy[1];
      //phiy[2] = fphiy[2];
      //unsigned long tempr = (fnosc(fnosc(t[0]/ftx[0] + fx[0]*i + phix[0]) * fnosc(t[1]/fty[0] + fy[0]*j + phiy[0])/64));
      //unsigned long tempg = (fnosc(fnosc(t[1]/ftx[1] + fx[1]*i + phix[1]) * fnosc(t[2]/fty[1] + fy[1]*j + phiy[1])/64));
      //unsigned long tempb = (fnosc(fnosc(t[2]/ftx[2] + fx[2]*i + phix[2]) * fnosc(t[0]/fty[2] + fy[2]*j + phiy[2])/64));
      //unsigned long tempr = ((fnosc(t[0]/ftx[0] + fx[0]*i + phix[0]) * fnosc(t[1]/fty[0] + fy[0]*j + phiy[0])/64));
      //unsigned long tempg = ((fnosc(t[1]/ftx[1] + fx[1]*i + phix[1]) * fnosc(t[2]/fty[1] + fy[1]*j + phiy[1])/64));
      //unsigned long tempb = ((fnosc(t[2]/ftx[2] + fx[2]*i + phix[2]) * fnosc(t[0]/fty[2] + fy[2]*j + phiy[2])/64));
      unsigned long tempr = ((fnosc(t[0]*ftx[0] + fx[0]*i) * fnosc(t[1]*fty[0] + fy[0]*j)));
      unsigned long tempg = ((fnosc(t[1]*ftx[1] + fx[1]*i) * fnosc(t[2]*fty[1] + fy[1]*j)));
      unsigned long tempb = ((fnosc(t[2]*ftx[2] + fx[2]*i) * fnosc(t[0]*fty[2] + fy[2]*j)));
      
      //unsigned long temp = i*256;
      uint8_t tcr = ((uint8_t) (tempr/256))/2;
      uint8_t tcg = ((uint8_t) (tempg/256))/3; //fnosc(pass*3 + 20 * i - 31 * j + 400)/8;
      uint8_t tcb = ((uint8_t) (tempb/256))/3; //fnosc(-pass*4 + 73 * i + 42 * j + 300)/4;

      matrix.drawPixel(i, j, matrix.Color(tcr, tcg, tcb));
    }
  }
  matrix.show();
  pass++;
}

// integer cosine using 2 parabolas
// input any int angle (0-255 is one revolution)
// output int value from 0-255. icos2p(64) = 128 for reference
int fnosc(int angle) {
  int x = abs(angle) % 256;
  if (x > 127) {
    x = 255 - x;
  }
  // here's the fn
  int y = 255 - 2*x;
  return y;
}

