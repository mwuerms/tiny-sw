/**
 * Martin Wuerms
 * 2015-11-29
 *
 */

#ifndef _BUTTON_H_
#define _BUTTON_H_

/* - includes --------------------------------------------------------------- */
#include <stdint.h>

/* - defines ---------------------------------------------------------------- */

/* - typedef ---------------------------------------------------------------- */

/* - public functions ------------------------------------------------------- */

void button_Init(void);

#define cBUTTON_RETURN_UNPRESSED   (0)
#define cBUTTON_RETURN_PRESSED     (1)
#define cBUTTON_RETURN_RISING      (2)
#define cBUTTON_RETURN_FALLING     (3)
#define cBUTTON_RETURN_PRESSED_2s  (4)
uint8_t button_Get(uint8_t interval);

#endif /* _BUTTON_H_ */
