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

/* - typedef ---------------------------------------------------------------- */

/* - public functions ------------------------------------------------------- */
void ledAnimation_Init(void);
void ledAnimation_ShowOff(void);
void ledAnimation_ShowGreen(void);
void ledAnimation_ShowRed(void);
void ledAnimation_Next(void);
void ledAnimation_Update(void);

#endif /* _LED_ANIMATION_H_ */
