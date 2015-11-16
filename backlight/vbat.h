/**
 * Martin Wuerms
 * 2015-11-16
 *
 */

#ifndef _VBAT_GET_H_
#define _VBAT_GET_H_

/* - includes --------------------------------------------------------------- */
#include <stdint.h>

/* - defines ---------------------------------------------------------------- */

/* - typedef ---------------------------------------------------------------- */
#define cVREF_VCC   (0)
#define cVREF_AVREF (1)

/* - public functions ------------------------------------------------------- */
uint8_t vbat_Get(uint8_t vref);

#endif /* _VABT_GET_H_ */
