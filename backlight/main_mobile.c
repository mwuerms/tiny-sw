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
 * pin usage on project "backlight"
 * name     signal
 * PB0      AVREF to measure VBAT
 * PB1      VCC-enable for LEDs, high: disable, low: enable, using P-FET to switch power
 * PB2      button, internal pullup, INT0 for wakeup
 * PB3      serial signal for LEDs1
 * PB4      serial signal for LEDs2
 * PB5      Reset
 *
 * pin usage on pcb
 * name     signal
 * PB2      button, internal pullup, INT0 for wakeup
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
#include "vbat.h"
#include "send.h"
#include "button.h"
#include "ledAnimation.h"

/* - typedefs --------------------------------------------------------------- */
//typedef void *(* state_handler_t)(void);

/* - defines ---------------------------------------------------------------- */

/* - variables -------------------------------------------------------------- */
volatile uint8_t global_events;
#define fEV_TICK_TIMER     _BV(0)
#define fEV_BUTTON_WAKE    _BV(1)

//static state_handler_t state_func;
static uint8_t state;
#define cST_BOOT        (0)
#define cST_ANIMATE     (1)
#define cST_OFF         (2)

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
 * initialize all components
 */
static void init(void) {
    uint8_t vbat;
    cli();

    PRR = 0xFF;
    global_events = 0;
    state = cST_BOOT;
    ledAnimation_Init();

    /*vbat = vbat_Get(cVREF_VCC);
    send_SeialMSB(vbat, _BV(0));*/
    sei();
}

/* - public functions ------------------------------------------------------- */

/**
 * main loop
 */
#include <util/delay.h>
int main (void)
{
    uint8_t local_events = 0;
    uint8_t button_state = 0;
    init();

    // start
    wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_0_125S, fEV_TICK_TIMER);
    state = cST_ANIMATE;

    while(1) {

        if(state == cST_ANIMATE) {
            if(local_events & fEV_TICK_TIMER) {
                ledAnimation_Update();

                /*button_state = button_Get(16);
                if(button_state == cBUTTON_RETURN_PRESSED_LONG) {
                    ledAnimation_Init();
                    state = cST_OFF;
                }
                if(button_state == cBUTTON_RETURN_FALLING) {
                    ledAnimation_Next();
                }*/
                wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_0_125S, fEV_TICK_TIMER);
            }
        }
        else { // only other possible state: if(state == cST_OFF)
            if(local_events & fEV_BUTTON_WAKE) {
                button_DisableIntWake();
                button_state = button_Get(16);
                if(button_state == cBUTTON_RETURN_UNPRESSED) {
                    button_EnableIntWake(fEV_BUTTON_WAKE);
                }
                else {
                    wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_0_125S, fEV_TICK_TIMER);
                }
            }
            if(local_events & fEV_TICK_TIMER) {
                button_state = button_Get(16);
                if(button_state == cBUTTON_RETURN_UNPRESSED) {
                    button_EnableIntWake(fEV_BUTTON_WAKE);
                }
                else if(button_state == cBUTTON_RETURN_PRESSED_LONG) {
                    ledAnimation_Init();
                    wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_0_125S, fEV_TICK_TIMER);
                    state = cST_ANIMATE;
                }
                else {
                    wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_0_125S, fEV_TICK_TIMER);
                }
            }
        }

        while(1) {
            cli();
            local_events = global_events;
            global_events = 0;
            if(local_events) {
                sei();
                break;
            }
            _EnterSleepMode(SLEEP_MODE_PWR_DOWN);   // 200 uA
        }
    }
    return(0);
}
