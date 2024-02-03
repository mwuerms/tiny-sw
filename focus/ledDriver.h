/**
 * Martin Wuerms
 * 2015-09-13
 */

#ifndef _LED_DRIVER_H_
#define _LED_DRIVER_H_

/* - includes --------------------------------------------------------------- */
#include <stdint.h>

/* - typedefs --------------------------------------------------------------- */
typedef struct {
    uint8_t green, red, blue;
} rgb_color_t;

/* - defines ---------------------------------------------------------------- */

/* - variables -------------------------------------------------------------- */

/* - public functions ------------------------------------------------------- */

int8_t ledDriver_Set(rgb_color_t *send, uint8_t length, uint8_t led_pin);

#endif /* _LED_DRIVER_H_ */
