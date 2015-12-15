/**
 * Martin Wuerms
 * 2015-08-10
 */

/* - this file contains architecture specific definitions etc ... ----------- */

#ifndef _ARCH_H_
#define _ARCH_H_

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include <avr/interrupt.h>

/* - define ----------------------------------------------------------------- */
// save status register + disable global interrupt
#define lock_interrupt(x)   do { \
                                sr = SREG; \
                                cli(); \
                            } while (0)
// restore status register again
#define restore_interrupt(x)    do { \
                                    SREG = sr; \
                                } while(0)
/*
                            do { \
                            x = SR; \
                            DINT; \
                            } while(0)
*/

/* - typedef ---------------------------------------------------------------- */

/* - variables -------------------------------------------------------------- */
extern volatile uint8_t global_events;

/* - public functions ------------------------------------------------------- */


#endif /* _ARCH_H_ */
