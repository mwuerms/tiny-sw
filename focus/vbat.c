/**
 * Martin Wuerms
 * 2015-11-16
 *
 */

/* - includes --------------------------------------------------------------- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "vbat.h"

/* - defines ---------------------------------------------------------------- */

/* - typedef ---------------------------------------------------------------- */

/* - variables -------------------------------------------------------------- */

/* - private functions ------------------------------------------------------ */

/* - public functions ------------------------------------------------------- */

/**
 * measure vbat
 * source: http://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
 * @param   vref    =0: VCC, =1: AVREF
 * @return  vbat in 100 mV, ex: 37 * 100 mV = 3.7 V
 */
uint8_t vbat_Get(uint8_t vref) {
    // Read 1.1V reference against AVcc
    // set the reference to Vcc and the measurement to the internal 1.1V reference
    uint8_t result;
    PRR &= ~_BV(PRADC);
    if(vref == cVREF_VCC) {
        ADMUX = _BV(ADLAR)|_BV(MUX3)|_BV(MUX2);  // VREF = VCC, result adjust left, measure bandgab (1.1V)
    }
    else {
        ADMUX = _BV(REFS0)|_BV(ADLAR)|_BV(MUX3)|_BV(MUX2);  // VREF = AVREF, result adjust left, measure bandgab (1.1V)
    }

    _delay_ms(2); // Wait for Vref to settle
    ADCSRA = _BV(ADEN)|_BV(ADSC); // Start conversion
    while (ADCSRA & ADSC); // measuring
    ADCSRA = 0;
    PRR |=  _BV(PRADC);

    result = ADCL; // must read ADCL first - it then locks ADCH
    result = ADCH; // unlocks both, but use ADCH only
    return(result);
    result = 2816 / result; // Calculate Vcc (in mV); 2816 = 1.1*256*10, result in 100 mV
    return(result); // vbat in  100 mV
}
