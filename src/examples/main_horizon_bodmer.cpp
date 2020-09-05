#include <Arduino.h>

#include <horizon.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip

unsigned long redrawTime = 0;

extern TFT_eSPI tft;

// #########################################################################
// Setup, runs once on boot up
// #########################################################################

void setup(void) {
  Serial.begin(115200);

  initHorizon();

  tft.setCursor(0,0); 
  tft.setTextSize(1);
  tft.println("HORIZON DEMO");
  tft.println("Based on Bodmer code");
  tft.println("Enjoy");
  delay(2000);

  initHorizon();

  // Test roll and pitch
  testRoll();
  testPitch();
}


// #########################################################################
// Main loop, keeps looping around
// #########################################################################

void loop() {

  // Refresh the display at regular intervals
  if (millis() > redrawTime) {
    redrawTime = millis() + REDRAW_DELAY;

    // Roll is in degrees in range +/-180
    // int roll = random(361) - 180;
    int roll = random(91) - 45;

    // Pitch is in y coord (pixel) steps, 20 steps = 10 degrees on drawn scale
    // Maximum pitch shouls be in range +/- 80 with HOR = 172
    int pitch = random (161) - 80;

    updateHorizon(roll, pitch);
  }
}
