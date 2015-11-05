/**
 * Martin Wuerms
 * 2015-10-18
 *
 */

#ifndef _WDT_TIMER_H_
#define _WDT_TIMER_H_

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include <avr/wdt.h>

/* - defines ---------------------------------------------------------------- */

/* - typedef ---------------------------------------------------------------- */
#define cEV_TIMER_INTERVAL_16MS     0
#define cEV_TIMER_INTERVAL_32MS     1
#define cEV_TIMER_INTERVAL_64MS     2
#define cEV_TIMER_INTERVAL_0_125S   3
#define cEV_TIMER_INTERVAL_0_25S    4
#define cEV_TIMER_INTERVAL_0_5S     5
#define cEV_TIMER_INTERVAL_1S       6
#define cEV_TIMER_INTERVAL_2S       7

/* - public functions ------------------------------------------------------- */
void wdtTimer_Init(uint8_t interval);
void wdtTimer_StartTimeout(uint8_t timeout, uint8_t event_flag);

#endif /* _WDT_TIMER_H_ */
