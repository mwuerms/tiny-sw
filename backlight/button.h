/**
 * Martin Wuerms
 * 2015-11-29
 *
 */

#ifndef _BUTTON_H_
#define _BUTTON_H_

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include <avr/io.h>

/* - defines ---------------------------------------------------------------- */
#define fEV_GET_BUTTON      _BV(1)

/* - typedef ---------------------------------------------------------------- */

/* - public functions ------------------------------------------------------- */

void button_Init(void);

#define cBUTTON_RETURN_UNPRESSED        (0)
#define cBUTTON_RETURN_PRESSED          (1)
#define cBUTTON_RETURN_RISING           (2)
#define cBUTTON_RETURN_FALLING          (3)
#define cBUTTON_RETURN_PRESSED_LONG     (4)
uint8_t button_Get(uint8_t pressed_delay);

#endif /* _BUTTON_H_ */
