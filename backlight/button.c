/**
 * Martin Wuerms
 * 2015-11-29
 *
 */

/* - includes --------------------------------------------------------------- */
#include <avr/interrupt.h>
#include <util/delay.h>
#include "arch.h"
#include "button.h"

/* - defines ---------------------------------------------------------------- */
#define cBUTTON_PIN _BV(2)  // button on PB2
#define button_GetValue()   (PINB  &   cBUTTON_PIN)

/* - typedef ---------------------------------------------------------------- */

/* - variables -------------------------------------------------------------- */
static volatile uint8_t button_event_flag;

/* - private functions------------------------------------------------------- */
/**
 * button ISR
 */
ISR(INT0_vect) {
    GIMSK &= ~_BV(INT0);
    GIFR  |=  _BV(INTF0);
    global_events |= button_event_flag;
}

/* - public functions ------------------------------------------------------- */

/**
 * enable INT0 to wake up
 */
void button_EnableIntWake(uint8_t event_flag) {
    button_event_flag = event_flag;
    button_PullUpEnable();
    GIMSK &= ~_BV(INT0);
    MCUCR &= ~(_BV(ISC01)|_BV(ISC00));  // INT0, low level
    GIFR  |=  _BV(INTF0);
    GIMSK |=  _BV(INT0);
}

/**
 * disable INT0
 */
void button_DisableIntWake(void) {
    GIMSK &= ~_BV(INT0);
    GIFR  |=  _BV(INTF0);
    button_OutLow();
}

/**
 * enable pull up
 */
void button_PullUpEnable(void) {
    MCUCR &= ~PUD;
    DDRB  &= ~cBUTTON_PIN;
    PORTB |=  cBUTTON_PIN;
    _delay_us(20);
}

/**
 * out, low
 */
void button_OutLow(void) {
    DDRB  |=  cBUTTON_PIN;
    PORTB &= ~cBUTTON_PIN;
}

/**
 * get the state of the button
 * @param   pressed_delay    call this func so many times in fixed interval to return cBUTTON_RETURN_PRESSED_LONG
 * @return  see cBUTTON_RETURN_... in button.h
 */
uint8_t button_Get(uint8_t pressed_delay) {
    static uint8_t value = cBUTTON_PIN, value_old = cBUTTON_PIN;    // unpressed
    static uint8_t timeout;
    button_PullUpEnable();

    value = button_GetValue();
    if((value == 0) && (value_old)) {
        // falling edge
        timeout = pressed_delay;// 2000ms/64ms + 1 = 32
        value_old = value;
        button_OutLow();
        return(cBUTTON_RETURN_FALLING);
    }

    if((value == 0) && (value_old == 0)) {
        // pressed
        if(timeout == 0) {
            timeout--;
            value_old = value;
            button_OutLow();
            return(cBUTTON_RETURN_PRESSED);
        }
        else {
            timeout--;
            if(timeout == 0) {
                value_old = value;
                button_OutLow();
                return(cBUTTON_RETURN_PRESSED_LONG);
            }
            value_old = value;
            button_OutLow();
            return(cBUTTON_RETURN_PRESSED);
        }
    }

    if((value) && (value_old == 0)) {
        // rising edge
        value_old = value;
        button_OutLow();
        return(cBUTTON_RETURN_RISING);
    }

    // if((value) && (value_old)) {
    // unpressed
    value_old = value;
    button_OutLow();
    return(cBUTTON_RETURN_UNPRESSED);
}
