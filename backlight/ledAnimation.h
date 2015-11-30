/**
 * Martin Wuerms
 * 2015-11-30
 *
 */

#ifndef _LED_ANIMATION_H_
#define _LED_ANIMATION_H_

/* - includes --------------------------------------------------------------- */
#include <stdint.h>

/* - defines ---------------------------------------------------------------- */
#define fEV_UPDATE_LEDs     _BV(0)

/* - typedef ---------------------------------------------------------------- */

/* - public functions ------------------------------------------------------- */
void ledAnimation_Init(void);
void ledAnimation_Update(void);

#endif /* _LED_ANIMATION_H_ */
