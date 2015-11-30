/**
 * Martin Wuerms
 * 2015-11-30
 *
 * color definitions
 * rrggbb name
 * ff0000 red
 * ff0066 piggy pink
 * ff00ff pink
 * 9900ff dark pink
 * 0000ff blue
 * 0099ff light blue
 * ff3300 orange 1
 * ffff00 yellow
 * cc9900 dark yellow
 * 00ff00 green
 * 669900 dark green
 * 66ff33 poison green
 * 00ffff turkis
 */

/* - includes --------------------------------------------------------------- */
#include <avr/io.h>
#include <string.h>
#include "arch.h"
#include "wdtTimer.h"
#include "ledAnimation.h"
#include "ledDriver.h"
#include "fade.h"

/* - defines ---------------------------------------------------------------- */
#define cNB_LEDs            (6)
#define cBRIGHTNESS_0       (0)
#define cBRIGHTNESS_1       (45)
#define cBRIGHTNESS_2       (90)
#define cBRIGHTNESS_STEP    (1)

#define mSETCOLOR(red, green, blue, r, g, b)    do { red = r; green = g; blue = b; } while(0)

/* - typedef ---------------------------------------------------------------- */

/* - variables -------------------------------------------------------------- */
static const rgb_color_t colors[] = {
    {.red = 50, .green = 0, .blue = 0},
    {.red = 50, .green = 0, .blue = 50},
    {.red = 0, .green = 0, .blue = 50},
    {.red = 0, .green = 50, .blue = 50},
    {.red = 0, .green = 50, .blue = 0},
    {.red = 50, .green = 50, .blue = 0},
    {.red = 50, .green = 25, .blue = 0},
    {.red = 50, .green = 25, .blue = 50},
    {.red = 0, .green = 25, .blue = 50}};
#define cNB_COLORS (sizeof(colors)/sizeof(rgb_color_t))

static rgb_color_t leds1[cNB_LEDs];
static rgb_color_t leds2[cNB_LEDs];

static uint8_t led_state;//, led_timeout;

/* - private functions ------------------------------------------------------ */

/**
 * set all red leds to a given value
 */
static void inline _SetAllRedLEDs(rgb_color_t *l, uint8_t nb, uint8_t value) {
    uint8_t i;
    for(i = 0; i < nb; i++) {
        l[i].red = value;
    }
}

static inline uint8_t _inc(uint8_t value, uint8_t max) {
    if(value < max)
        return(value+1);
    return(0);
}

/* - public functions ------------------------------------------------------- */

/**
 * initialize
 */
void ledAnimation_Init(void) {
    led_state = 0;
    memset(leds1, 0, sizeof(*leds1));
    memset(leds2, 0, sizeof(*leds2));

    ledDriver_Set(leds1, cNB_LEDs, _BV(4));
    ledDriver_Set(leds2, cNB_LEDs, _BV(3));
}

/**
 * process the leds
 */
void ledAnimation_Update(void) {
    static uint8_t led_i = 0;

    ledDriver_Set(leds1, cNB_LEDs, _BV(4));
    ledDriver_Set(leds2, cNB_LEDs, _BV(3));
    //wdtTimer_StartTimeout(led_timeout, fEV_UPDATE_LEDs);
    wdtTimer_StartTimeout(0, fEV_UPDATE_LEDs);

    //static uint8_t led_dir = 0; // =0: down, =1: up
    _SetAllRedLEDs(leds2, cNB_LEDs, cBRIGHTNESS_1);

    if(led_i == 0) {
        led_i = cNB_LEDs-1;
    }
    else {
        led_i--;
    }

    /*
        if(led_i > cNB_LEDs) {
            led_i = cNB_LEDs;
            led_dir = 0;
        }
        if(led_dir == 0) {
            led_i--;
            if(led_i == 0) {
                led_dir = 1;
            }
        }
        else {
            led_i++;
            if(led_i >= (cNB_LEDs-1)) {
                led_dir = 0;
            }
        }
    */
    leds2[led_i].red = cBRIGHTNESS_2;

    /*
     * ff0000 red
     * ff0066 piggy pink
     * ff00ff pink
     * 9900ff dark pink
     * 0000ff blue
     * 0099ff light blue
     * ff3300 orange 1
     * ffff00 yellow
     * cc9900 dark yellow
     * 00ff00 green
     * 669900 dark green
     * 66ff33 poison green
     * 00ffff turkis
     */
     /*static uint8_t ci = cNB_LEDs;
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 0xff, 0x00, 0x00);
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 0xff, 0x00, 0x66);
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 0xff, 0x00, 0xff);
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 0x99, 0x00, 0x66);
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 0x00, 0x00, 0xff);
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 0x00, 0x99, 0xff);
     ci = _inc(ci, (cNB_LEDs-1));
     ci = _inc(ci, (cNB_LEDs-1));*/
/*
     static uint8_t ci = cNB_LEDs;
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 50, 0, 0);
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 50, 50, 0);
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 0, 50, 0);
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 0, 50, 50);
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 0, 0, 50);
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 50, 0, 0);
     ci = _inc(ci, (cNB_LEDs-1));*/

     /*static uint8_t ci = cNB_LEDs;
     static color_t col = {.red = 50, .green = 0, .blue = 0};
     color col1;
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, col1.red, col1.green. col1.blue);
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 50, 50, 0);
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 0, 50, 0);
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 0, 50, 50);
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 0, 0, 50);
     ci = _inc(ci, (cNB_LEDs-1));
     mSETCOLOR(ledsc[ci].red, ledsc[ci].green, ledsc[ci].blue, 50, 0, 0);
     ci = _inc(ci, (cNB_LEDs-1));
     ci = _inc(ci, (cNB_LEDs-1));*/


     static rgb_color_t c0, c1;
     static fade_color_t fc;
     rgb_color_t cur;
     static uint8_t fade_state = 0;
     static uint8_t color_count;
     uint8_t i;
     if(fade_state == 0) {
         fade_state = 1;
         color_count = 0;
         mSETCOLOR(c0.red, c0.green, c0.blue, colors[color_count].red, colors[color_count].green, colors[color_count].blue);
         color_count = _inc(color_count, (cNB_COLORS-1));
         mSETCOLOR(c1.red, c1.green, c1.blue, colors[color_count].red, colors[color_count].green, colors[color_count].blue);
         color_count = _inc(color_count, (cNB_COLORS-1));

         fade_Start(&c0, &c1, &fc, 10);

         // wrong direction for(i = 0; i < cNB_LEDs; i++) {
         for(i = cNB_LEDs-1; i > 0; i--) {
             fade_GetCurrentColor(&fc, &cur);
             mSETCOLOR(leds1[i].red, leds1[i].green, leds1[i].blue, cur.red, cur.green, cur.blue);
             fade_Next(&fc);
         }
         fade_GetCurrentColor(&fc, &cur);
         mSETCOLOR(leds1[i].red, leds1[i].green, leds1[i].blue, cur.red, cur.green, cur.blue);
     }
     else {
         // copy values from [i-1] to [i], from cNB_LEDs-1 ... 1, afterwards set [0] to new value
         for(i = cNB_LEDs-1; i > 0; i--) {
             mSETCOLOR(leds1[i].red, leds1[i].green, leds1[i].blue, leds1[i-1].red, leds1[i-1].green, leds1[i-1].blue);

         }
         if(fade_Next(&fc) == 0) {
             // set new colors
             mSETCOLOR(c0.red, c0.green, c0.blue, c1.red, c1.green, c1.blue);
             mSETCOLOR(c1.red, c1.green, c1.blue, colors[color_count].red, colors[color_count].green, colors[color_count].blue);
             color_count = _inc(color_count, (cNB_COLORS-1));
             fade_Start(&c0, &c1, &fc, 10);
         }
         fade_GetCurrentColor(&fc, &cur);
         mSETCOLOR(leds1[0].red, leds1[0].green, leds1[0].blue, cur.red, cur.green, cur.blue);

     }

     /*mSETCOLOR(ledsc[0].red, ledsc[0].green, ledsc[0].blue, 0,0,15);
     mSETCOLOR(ledsc[1].red, ledsc[1].green, ledsc[1].blue, 10,0,15);
     mSETCOLOR(ledsc[2].red, ledsc[2].green, ledsc[2].blue, 20,0,15);
     mSETCOLOR(ledsc[3].red, ledsc[3].green, ledsc[3].blue, 30,0,15);
     mSETCOLOR(ledsc[4].red, ledsc[4].green, ledsc[4].blue, 40,0,15);
     mSETCOLOR(ledsc[5].red, ledsc[5].green, ledsc[5].blue, 50,0,15);*/

}
