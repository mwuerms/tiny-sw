/**
 * Martin Wuerms
 * 2015-10-18
 *
 */

/* - includes --------------------------------------------------------------- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "send.h"

/* - defines ---------------------------------------------------------------- */

/* - typedef ---------------------------------------------------------------- */

/* - variables -------------------------------------------------------------- */

/* - private functions ------------------------------------------------------ */

/* - public functions ------------------------------------------------------- */

/**
 * send a value serially from a pin, msb first
 * @param   value       to send
 * @param   pin_mask
 */
void send_SeialMSB(uint8_t value, uint8_t pin_mask) {
    uint8_t i;
    DDRB |= pin_mask;
    PORTB &= ~pin_mask;
    for(i = 0x80; i > 0; i = i >> 1) {
        if(value & i) {
            PORTB |=  pin_mask;
            _delay_ms(2);
        }
        PORTB &= ~pin_mask;
    }
    PORTB &= ~pin_mask;
}
