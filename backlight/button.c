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
#define cBUTTON_PIN _BV(2)  // button on PB2
#define button_In()         (DDRB  &= ~cBUTTON_PIN)
#define button_Pullup()     do { \
                                PORTB |=  cBUTTON_PIN; \
                                MCUCR &= ~PUD; \
                            } while (0)
#define button_GetValue()   (PINB  &   cBUTTON_PIN)

/* - typedef ---------------------------------------------------------------- */

/* - public functions ------------------------------------------------------- */

/**
 * initialize
 */
void button_Init(void) {
    button_In();
    button_Pullup();
}
/**
 * get the state of the button
 * @param   interval    timing interval
 * @return  see cBUTTON_RETURN_... in button.h
 */
#define cBUTTON_RETURN_UNPRESSED   (0)
#define cBUTTON_RETURN_PRESSED     (1)
#define cBUTTON_RETURN_RISING      (2)
#define cBUTTON_RETURN_FALLING     (3)
#define cBUTTON_RETURN_PRESSED_2s  (4)
uint8_t button_Get(uint8_t interval) {
    static uint8_t value = 1, value_old = 1;    // unpressed
    static uint8_t timeout;

    value = button_GetValue();
    if((value == 0) && (value_old == 1)) {
        // falling edge
        timeout = 32;// 2000ms/64ms + 1;
        value_old = value;
        return(cBUTTON_RETURN_FALLING);
    }

    if((value == 0) && (value_old == 0)) {
        // pressed
        if(timeout == 0) {
            timeout--;
            value_old = value;
            return(cBUTTON_RETURN_FALLING);
        }
        else {
            timeout--;
            if(timeout == 0) {
                value_old = value;
                return(cBUTTON_RETURN_PRESSED_2s);
            }
            value_old = value;
            return(cBUTTON_RETURN_FALLING);
        }
    }

    if((value == 1) && (value_old == 0)) {
        // rising edge
        value_old = value;
        return(cBUTTON_RETURN_RISING);
    }

    // if((value == 1) && (value_old == 1)) {
    // unpressed
    value_old = value;
    return(cBUTTON_RETURN_UNPRESSED);
}

#endif /* _BUTTON_H_ */
