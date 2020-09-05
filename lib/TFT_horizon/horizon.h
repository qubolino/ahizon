
#ifndef Horizon_h
#define Horizon_h


#define XC 120  // x coord of centre of horizon
#define YC 160  // y coord of centre of horizon


#define REDRAW_DELAY 16 // minimum delay in milliseconds between display updates

#define HOR 2 * YC    // Horizon vector line length

#define BROWN      0x5140 //0x5960
#define SKY_BLUE   0x02B5 //0x0318 //0x039B //0x34BF
#define DARK_RED   0x8000
#define DARK_GREY  0x39C7

#define DEG2RAD 0.0174532925


void initHorizon(void);

void drawHorizon(int roll, int pitch);

void drawInfo(void);

void updateHorizon(int roll, int pitch);



void testRoll();
void testPitch();

#endif