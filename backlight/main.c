/**
 * Martin Wuerms
 * 2015-09-13
 *
 * attiny85 pin description
 * name     pin     functions (attiny85)
 * PB0      5       (MOSI/DI/SDA/AIN0/OC0A/OC1A/AREF/PCINT0)
 * PB1      6       (MISO/DO/AIN1/OC0B/OC1A/PCINT1)
 * PB2      7       (SCK/USCK/SCL/ADC1/T0/INT0/PCINT2)
 * PB3      2       (PCINT3/XTAL1/CLKI/OC1B/ADC3)
 * PB4      3       (PCINT4/XTAL2/CLKO/OC1B/ADC2)
 * PB5      1       (PCINT5/RESET/ADC0/dW)
 * VCC      8       2.7 ... 5.5 V
 * GND      4
 *
 * pin usage on pcb
 * name     signal
 * PB3      USB D+
 * PB4      USB D-
 * PB5      Reset
 */

/* - includes --------------------------------------------------------------- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <string.h>
#include "arch.h"
#include "wdtTimer.h"
#include "ledDriver.h"

/* - typedefs --------------------------------------------------------------- */

/* - defines ---------------------------------------------------------------- */
#define fEV_UPDATE_LEDs     _BV(0)

#define cNB_LEDs            (5)
#define cBRIGHTNESS_0       (0)
#define cBRIGHTNESS_1       (45)
#define cBRIGHTNESS_2       (90)
#define cBRIGHTNESS_STEP    (1)

/* - variables -------------------------------------------------------------- */
volatile uint8_t gloabl_events, local_events;
rgb_color_t leds[cNB_LEDs];
uint8_t led_state, led_timeout;

/* - private functions  ----------------------------------------------------- */
/**
 * enter the given sleep mode, see avr/sleep.h
 * @param   mode    sleep modes:
 * translate: own defines to arv/sleep.h:
 * cSLEEP_MODE_... | SLEEP_MODE_...
 * cSLEEP_MODE_ACTIVE | none, do not enter this function
 * cSLEEP_MODE_IDLE | SLEEP_MODE_IDLE
 *  none | SLEEP_MODE_ADC
 * cSLEEP_MODE_POWER_DOWN | SLEEP_MODE_PWR_DOWN
 *  none | SLEEP_MODE_PWR_SAVE
 *  none | SLEEP_MODE_STANDBY
 *  none | SLEEP_MODE_EXT_STANDBY
 */
static void _EnterSleepMode(uint8_t mode) {
    set_sleep_mode(mode);
    sleep_enable();
    sei();
    sleep_cpu();    // enter sleep mode, wait here
    sleep_disable();
    cli();
}

/**
 * set all red leds to a given value
 */
static void inline _SetAllRedLEDs(uint8_t value) {
    uint8_t i;
    for(i = 0; i < cNB_LEDs; i++) {
        leds[i].red = value;
    }
}

/**
 * process the leds
 */
static void leds_Update(void) {
    static uint8_t led_i = 0;
    //static uint8_t led_dir = 0; // =0: down, =1: up
    _SetAllRedLEDs(cBRIGHTNESS_1);

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
    leds[led_i].red = cBRIGHTNESS_2;
}
/**
 * initialize all components
 */
static void init(void) {
    cli();

    PRR = 0xFF;
    gloabl_events = 0;
    local_events = fEV_UPDATE_LEDs;
    led_state = 0;
    memset(leds, 0, sizeof(*leds));

    wdtTimer_Init(cEV_TIMER_INTERVAL_0_125S);

    sei();
}

/* - public functions ------------------------------------------------------- */

/**
 * main loop
 */
#include <util/delay.h>
int main (void)
{
    uint8_t i;
    init();

    // start
    local_events = fEV_UPDATE_LEDs;

    while(1) {
        if(local_events & fEV_UPDATE_LEDs) {
            leds_Update();
            //ledDriver_Set(leds, cNB_LEDs, _BV(3));
            ledDriver_Set(leds, cNB_LEDs, _BV(4));
            //wdtTimer_StartTimeout(led_timeout, fEV_UPDATE_LEDs);
            wdtTimer_StartTimeout(1, fEV_UPDATE_LEDs);
        }

        while(1) {
            cli();
            local_events = gloabl_events;
            gloabl_events = 0;
            if(local_events) {
                sei();
                break;
            }
            //_EnterSleepMode(SLEEP_MODE_IDLE);
            _EnterSleepMode(SLEEP_MODE_PWR_DOWN);   // 200 uA
        }
    }
    return(0);
}
