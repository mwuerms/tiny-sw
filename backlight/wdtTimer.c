/**
 * Martin Wuerms
 * 2015-10-18
 *
 */

/* - includes --------------------------------------------------------------- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "arch.h"
#include "wdtTimer.h"

/* - defines ---------------------------------------------------------------- */

/* - typedef ---------------------------------------------------------------- */

/* - variables -------------------------------------------------------------- */
static volatile uint8_t wdt_interval, wdt_timeout, wdt_event_flag;

/* - private functions ------------------------------------------------------ */

/**
 * stop wdtTimer
 */
static inline void wdtTimer_Stop(void) {
    WDTCR = (1 << WDCE);
    WDTCR = 0;
}

/**
 * watchdog ISR
 */
ISR(WDT_vect) {
    if(wdt_timeout > 0) {
        wdt_timeout--;
    }
    else {
        wdtTimer_Stop();
        gloabl_events |= wdt_event_flag;
    }
}

/* - public functions ------------------------------------------------------- */

/**
 * initialize the event timer
 */
void wdtTimer_Init(uint8_t interval) {
    wdt_interval = interval & 0x07;
}

/**
 * start the wdtTimer
 * @param   timeout     overall timeout = interval * timeout
 */
void wdtTimer_StartTimeout(uint8_t timeout, uint8_t event_flag) {
    uint8_t sr;
    lock_interrupt(sr);
    wdt_timeout = timeout;
    wdt_event_flag = event_flag;
    WDTCR = (1 << WDCE) | (1 << WDIF);
    WDTCR = (wdt_interval);
    WDTCR |= (1 << WDIE);
    restore_interrupt(sr);
}
