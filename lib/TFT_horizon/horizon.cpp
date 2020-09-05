
#include "horizon.h"

#include <SPI.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip


int last_roll = 0; // the whole horizon graphic
int last_pitch = 0;

float fps = 0.0;

extern TFT_eSPI tft = TFT_eSPI(2 * XC, 2 * YC);


// #########################################################################
// Draw the horizon with a new roll (angle in range -180 to +180)
// #########################################################################


void drawHorizon(int roll, int pitch)
{
  // Calculate coordinates for line start
  float sx = cos(roll * DEG2RAD);
  float sy = sin(roll * DEG2RAD);

  int16_t x0 = sx * HOR;
  int16_t y0 = sy * HOR;
  int16_t xd = 0;
  int16_t yd = 1;
  int16_t xdn  = 0;
  int16_t ydn = 0;

  if (roll > 45 && roll <  135) {
    xd = -1;
    yd =  0;
  }
  if (roll >=  135)             {
    xd =  0;
    yd = -1;
  }
  if (roll < -45 && roll > -135) {
    xd =  1;
    yd =  0;
  }
  if (roll <= -135)             {
    xd =  0;
    yd = -1;
  }

  if ((roll != last_roll) && ((abs(roll) > 35)  || (pitch != last_pitch)))
  {
    xdn = 4 * xd;
    ydn = 4 * yd;
    tft.drawLine(XC - x0 - xdn, YC - y0 - ydn - pitch, XC + x0 - xdn, YC + y0 - ydn - pitch, SKY_BLUE);
    tft.drawLine(XC - x0 + xdn, YC - y0 + ydn - pitch, XC + x0 + xdn, YC + y0 + ydn - pitch, BROWN);
    xdn = 3 * xd;
    ydn = 3 * yd;
    tft.drawLine(XC - x0 - xdn, YC - y0 - ydn - pitch, XC + x0 - xdn, YC + y0 - ydn - pitch, SKY_BLUE);
    tft.drawLine(XC - x0 + xdn, YC - y0 + ydn - pitch, XC + x0 + xdn, YC + y0 + ydn - pitch, BROWN);
  }
  xdn = 2 * xd;
  ydn = 2 * yd;
  tft.drawLine(XC - x0 - xdn, YC - y0 - ydn - pitch, XC + x0 - xdn, YC + y0 - ydn - pitch, SKY_BLUE);
  tft.drawLine(XC - x0 + xdn, YC - y0 + ydn - pitch, XC + x0 + xdn, YC + y0 + ydn - pitch, BROWN);

  tft.drawLine(XC - x0 - xd, YC - y0 - yd - pitch, XC + x0 - xd, YC + y0 - yd - pitch, SKY_BLUE);
  tft.drawLine(XC - x0 + xd, YC - y0 + yd - pitch, XC + x0 + xd, YC + y0 + yd - pitch, BROWN);

  tft.drawLine(XC - x0, YC - y0 - pitch,   XC + x0, YC + y0 - pitch,   TFT_WHITE);

  last_roll = roll;
  last_pitch = pitch;

}

void drawHorizonQB(int roll, int pitch)
{
  long tic = millis();
  // center given by pitch: 0 is center, 90 is full screen
  // int hcy = YC +  YC * last_pitch / 90;
  // int offset = XC * tan(last_roll * DEG2RAD);
  // tft.drawLine(XC * 0, hcy - offset, XC *2, hcy + offset, TFT_BLACK);

  // tft.fillScreen(TFT_BLACK);

  // new drawing
  int hcy = YC +  YC * pitch / 90;
  int offset = XC * tan(roll * DEG2RAD);

  tft.fillRect(XC * 0, 0, XC * 2, hcy - abs(offset), SKY_BLUE);
  tft.fillRect(XC * 0, hcy + abs(offset), XC * 2, 2 * YC - hcy - abs(offset), BROWN);

  if (roll < 0){
    tft.fillTriangle(XC * 0, hcy - offset, XC * 2, hcy + offset, XC * 0, hcy + offset, SKY_BLUE);
    tft.fillTriangle(XC * 0, hcy - offset, XC * 2, hcy + offset, XC * 2, hcy - offset, BROWN);
  } else {
    tft.fillTriangle(XC * 0, hcy - offset, XC * 2, hcy + offset, XC * 0, hcy + offset, BROWN);
    tft.fillTriangle(XC * 0, hcy - offset, XC * 2, hcy + offset, XC * 2, hcy - offset, SKY_BLUE);
  }

  tft.drawLine(XC * 0, hcy - offset, XC * 2, hcy + offset, TFT_WHITE);

  long toc = millis();

  fps = 1000.0f / (toc - tic);

  Serial.printf("PI %d, RO %d\n", pitch, roll);

  last_roll = roll;
  last_pitch = pitch;
}

// #########################################################################
// Draw the information
// #########################################################################

void drawInfo(void)
{
  // Update things near middle of screen first (most likely to get obscured)

  // Level wings graphic
  tft.fillRect(XC - 1, YC - 1, 3, 3, TFT_RED);
  tft.drawFastHLine(XC - 30,   YC, 24, TFT_RED);
  tft.drawFastHLine(XC + 30 - 24, YC, 24, TFT_RED);
  tft.drawFastVLine(XC - 30 + 24, YC, 3, TFT_RED);
  tft.drawFastVLine(XC + 30 - 24, YC, 3, TFT_RED);

  // Pitch scale
  tft.drawFastHLine(XC - 12,   YC - YC * 20 / 90, 24, TFT_WHITE);
  tft.drawFastHLine(XC -  6,   YC - YC * 15 / 90, 12, TFT_WHITE);
  tft.drawFastHLine(XC - 12,   YC - YC * 10 / 90, 24, TFT_WHITE);
  tft.drawFastHLine(XC -  6,   YC - YC * 5 / 90, 12, TFT_WHITE);

  tft.drawFastHLine(XC -  6,   YC + YC * 5 / 90, 12, TFT_WHITE);
  tft.drawFastHLine(XC - 12,   YC + YC * 10 / 90, 24, TFT_WHITE);
  tft.drawFastHLine(XC -  6,   YC + YC * 15 / 90, 12, TFT_WHITE);
  tft.drawFastHLine(XC - 12,   YC + YC * 20 / 90, 24, TFT_WHITE);

  // Pitch scale values
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(XC - 12 - 13, YC - YC * 10 / 90 - 3);
  tft.print("10");
  tft.setCursor(XC + 12 + 1, YC - YC * 10 / 90 - 3);
  tft.print("10");
  tft.setCursor(XC - 12 - 13, YC + YC * 10 / 90 - 3);
  tft.print("10");
  tft.setCursor(XC + 12 + 1, YC + YC * 10 / 90 - 3);
  tft.print("10");

  tft.setCursor(XC - 12 - 13, YC - YC * 20 / 90 - 3);
  tft.print("20");
  tft.setCursor(XC + 12 + 1, YC - YC * 20 / 90 - 3);
  tft.print("20");
  tft.setCursor(XC - 12 - 13, YC + YC * 20 / 90 - 3);
  tft.print("20");
  tft.setCursor(XC + 12 + 1, YC + YC * 20 / 90 - 3);
  tft.print("20");

  // Display justified roll value near bottom of screen
  tft.setTextColor(TFT_YELLOW, BROWN); // Text with background
  tft.setTextDatum(MC_DATUM);            // Centre middle justified
  tft.setTextPadding(24);                // Padding width to wipe previous number
  tft.drawNumber(last_pitch, XC - 10, YC * 2 - 20, 1);
  tft.drawNumber(last_roll, XC + 10, YC * 2 - 20, 1);
  tft.drawNumber((long)fps, XC + 30, YC * 2 - 20, 1);

  // Draw fixed text
  tft.setTextColor(TFT_YELLOW);
  tft.setTextDatum(TC_DATUM);            // Centre middle justified
  tft.drawString("SPD  LNAV WNAV PTH", XC, 1, 1);
  tft.drawString("Bodmer's AHI", XC, YC * 2 -10, 1);
}

// #########################################################################
// Update the horizon with a new roll (angle in range -180 to +180)
// #########################################################################

void updateHorizon(int roll, int pitch)
{
  int delta_pitch = 0;
//   int pitch_error = 0;
  int delta_roll  = 0;
  while ((last_pitch != pitch) || (last_roll != roll))
  {
    delta_pitch = 0;
    delta_roll  = 0;

    if (last_pitch < pitch) {
      delta_pitch = 1;
    //   pitch_error = pitch - last_pitch;
    }

    if (last_pitch > pitch) {
      delta_pitch = -1;
    //   pitch_error = last_pitch - pitch;
    }

    if (last_roll < roll) delta_roll  = 1;
    if (last_roll > roll) delta_roll  = -1;

    // if (delta_roll == 0) {
    //   if (pitch_error > 1) delta_pitch *= 2;
    // }

    drawHorizon(last_roll + delta_roll, last_pitch + delta_pitch);
    drawInfo();
  }
}


// #########################################################################
// init
// #########################################################################

void initHorizon(void){

  tft.begin();
  tft.setRotation(2);
  tft.invertDisplay(false);

  tft.fillRect(0,  0, 2*XC, YC, SKY_BLUE);
  tft.fillRect(0, YC, 2*XC, YC, BROWN);

  // Draw the horizon graphic
  drawHorizon(0, 0);
  drawInfo();
}


// #########################################################################
//  for testing only
// #########################################################################

// Variables for test only
int test_roll = 0;
int delta = 0;


int rollGenerator(int maxroll)
{
  // Synthesize a smooth +/- 50 degree roll value for testing
  delta++; if (delta >= 360) test_roll = 0;
  test_roll = (maxroll + 1) * sin((delta) * DEG2RAD);

  // Clip value so we hold roll near peak
  if (test_roll >  maxroll) test_roll =  maxroll;
  if (test_roll < -maxroll) test_roll = -maxroll;

  return test_roll;
}

// #########################################################################
// Function to generate roll angles for testing only
// #########################################################################

void testRoll(void)
{
  tft.setTextColor(TFT_YELLOW, SKY_BLUE);
  tft.setTextDatum(TC_DATUM);            // Centre middle justified
  tft.drawString("Roll test", 64, 10, 1);

  for (int a = 0; a < 360; a++) {
    delay(REDRAW_DELAY / 2);
    updateHorizon(rollGenerator(180), 0);
  }
  tft.setTextColor(TFT_YELLOW, SKY_BLUE);
  tft.setTextDatum(TC_DATUM);            // Centre middle justified
  tft.drawString("         ", 64, 10, 1);
}

// #########################################################################
// Function to generate pitch angles for testing only
// #########################################################################

void testPitch(void)
{
  tft.setTextColor(TFT_YELLOW, SKY_BLUE);
  tft.setTextDatum(TC_DATUM);            // Centre middle justified
  tft.drawString("Pitch test", 64, 10, 1);

  for (int p = 0; p > -80; p--) {
    delay(REDRAW_DELAY / 2);
    updateHorizon(0, p);
  }

  for (int p = -80; p < 80; p++) {
    delay(REDRAW_DELAY / 2);
    updateHorizon(0, p);
  }


  for (int p = 80; p > 0; p--) {
    delay(REDRAW_DELAY / 2);
    updateHorizon(0, p);
  }

  tft.setTextColor(TFT_YELLOW, SKY_BLUE);
  tft.setTextDatum(TC_DATUM);            // Centre middle justified
  tft.drawString("          ", 64, 10, 1);
}
