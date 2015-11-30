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

/* - defines ---------------------------------------------------------------- */
volatile uint8_t gloabl_events;
#define fEB_GET_BUTTON      _BV(1)

/* - variables -------------------------------------------------------------- */


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
    gloabl_events = 0;
    ledAnimation_Init();

    wdtTimer_Init(cEV_TIMER_INTERVAL_0_125S);

    vbat = vbat_Get(cVREF_VCC);
    send_SeialMSB(vbat, _BV(0));
    sei();
}

/* - public functions ------------------------------------------------------- */

/**
 * main loop
 */
#include <util/delay.h>
int main (void)
{
    uint8_t local_events;
    init();

    // start
    local_events = fEV_UPDATE_LEDs;

    while(1) {
        if(local_events & fEV_UPDATE_LEDs) {
            ledAnimation_Update();
        }

        while(1) {
            cli();
            local_events = gloabl_events;
            gloabl_events = 0;
            if(local_events) {
                sei();
                break;
            }
            _EnterSleepMode(SLEEP_MODE_PWR_DOWN);   // 200 uA
        }
    }
    return(0);
}
