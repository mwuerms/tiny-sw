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
#define cNB_LEDs            (5)
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
    {.red = 50, .green = 25, .blue = 25},
    {.red = 0, .green = 25, .blue = 50},
    {.red = 0, .green = 50, .blue = 25},
    {.red = 0, .green = 25, .blue = 25},
    {.red = 25, .green = 50, .blue = 0},
    {.red = 25, .green = 25, .blue = 50},
};
#define cNB_COLORS (sizeof(colors)/sizeof(rgb_color_t))

static rgb_color_t leds1[cNB_LEDs];
static uint8_t leds1_pin;
static rgb_color_t leds2[cNB_LEDs];
static uint8_t leds2_pin;

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
    memset(leds1, 0, sizeof(leds1));
    memset(leds2, 0, sizeof(leds2));

    leds1_pin = _BV(4);
    leds2_pin = _BV(3);
    ledDriver_Set(leds1, cNB_LEDs, leds1_pin);
    ledDriver_Set(leds2, cNB_LEDs, leds2_pin);
}

/**
 * switch to next animation
 */
void ledAnimation_Next(void) {
    uint8_t temp;
    temp = leds1_pin;
    leds1_pin = leds2_pin;
    leds2_pin = temp;
}

/**
 * process the leds
 */
void ledAnimation_Update(void) {
    static uint8_t led_i = 0;

    ledDriver_Set(leds1, cNB_LEDs, leds1_pin);
    ledDriver_Set(leds2, cNB_LEDs, leds2_pin);

    //static uint8_t led_dir = 0; // =0: down, =1: up
    _SetAllRedLEDs(leds2, cNB_LEDs, cBRIGHTNESS_1);

    if(led_i == 0) {
        led_i = cNB_LEDs-1;
    }
    else {
        led_i--;
    }
    leds2[led_i].red = cBRIGHTNESS_2;

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

}
