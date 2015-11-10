/**
 * Martin Wuerms
 * 2015-09-13
 *
 * attiny85 pin description
 * name     pin     functions (attiny85)
 * PB0      5       (MOSI/DI/SDA/AIN0/OC0A/OC1A/AREF/PCINT0)
 * PB1      6       (MISO/DO/AIN1/OC0B/OC1A/PCINT1)
 * PB2      7       (SCK/USCK/SCL/ADC1/T0/INT0/PCINT2)
 * PB3      2       (PCINT3/XTAL1/CLKI/OC1B/ADC3)
 * PB4      3       (PCINT4/XTAL2/CLKO/OC1B/ADC2)
 * PB5      1       (PCINT5/RESET/ADC0/dW)
 * VCC      8       2.7 ... 5.5 V
 * GND      4
 *
 * pin usage on pcb
 * name     signal
 * PB2      button
 * PB3      USB D+
 * PB4      USB D-
 * PB5      Reset
 */

/* - includes --------------------------------------------------------------- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

/* - typedefs --------------------------------------------------------------- */

/* - defines ---------------------------------------------------------------- */

/* - variables -------------------------------------------------------------- */
static volatile wakeup;

/* - private functions  ----------------------------------------------------- */
/**
 * enter the given sleep mode, see avr/sleep.h
 * @param   mode    sleep modes:
 * translate: own defines to arv/sleep.h:
 * cSLEEP_MODE_... | SLEEP_MODE_...
 * cSLEEP_MODE_ACTIVE | none, do not enter this function
 * cSLEEP_MODE_IDLE | SLEEP_MODE_IDLE
 *  none | SLEEP_MODE_ADC
 * cSLEEP_MODE_POWER_DOWN | SLEEP_MODE_PWR_DOWN
 *  none | SLEEP_MODE_PWR_SAVE
 *  none | SLEEP_MODE_STANDBY
 *  none | SLEEP_MODE_EXT_STANDBY
 */
static void _EnterSleepMode(uint8_t mode) {
    set_sleep_mode(mode);
    sleep_enable();
    sei();
    sleep_cpu();    // enter sleep mode, wait here
    sleep_disable();
    cli();
}

/**
 * setup the INT0
 * @param   mode    value for ICS01, ICS00 in MCUCR
 *                  =0: low level Interrupt
 *                  =1: any logical change
 *                  =2: falling edge
 *                  =3: rising edge
 */
static void _SetupINT0(uint8_t mode) {
    mode  &=  (_BV(ISC01)|_BV(ISC00));
    GIMSK &= ~_BV(INT0);
    MCUCR &= ~(_BV(ISC01)|_BV(ISC00));
    MCUCR |=  mode;
    GIFR  |=  _BV(INTF0);
    GIMSK |=  _BV(INT0);
}

/**
 * ISR for button, low level Interrupt
 */
ISR(INT0_vect) {
    GIFR  |=  _BV(INTF0);
    /*
    // determine which type has triggered
    if((MCUCR & (_BV(ISC01)|_BV(ISC00))) == 0x00) {
        // low level interrupt has triggered --> enable rising edge
        _SetupINT0(3);
    }
    else {
        // any other type: only rising edge was setup
        // rising edge has triggered --> enable low level
        _SetupINT0(0);
        // reenter sleep mode
        wakeup = 1;
    }*/
}

/**
 * initialize all components
 */
static void init(void) {
    wakeup = 0;
    PRR = 0xFF;
    // button: PB2, input, pull up, level interrupt (low)
    DDRB  &= ~_BV(2);
    PORTB |=  _BV(2);
    MCUCR &= ~_BV(PUD);

    DDRB |= _BV(0);
    PORTB &= ~_BV(0);
    PORTB |=  _BV(0);
    PORTB &= ~_BV(0);

    // level interrupt, must be hold for at least 4 clock cycles
    _SetupINT0(0);
    sei();
}

/* - public functions ------------------------------------------------------- */

/**
 * main loop
 */
int main (void)
{
    uint8_t in1, in0;
    in0 = 0;
    in1 = 0;
    init();
    /*while(1) {
        in0 = PINB & _BV(2);
        if((in0 == _BV(2) ) && (in1 == 0)) {
            // falling edge
            PORTB &= ~_BV(0);
        }
        if((in0 == 0) && (in1 == _BV(2) )) {
            // rising edge
            PORTB |=  _BV(0);
        }
        in1 = in0;
    }*/
    /*
    while(1) {
        PRR = 0xFF;
        // programmer not plugged in
        // active 5.8 mA
        //_EnterSleepMode(SLEEP_MODE_IDLE); // 2.7 mA
        //_EnterSleepMode(SLEEP_MODE_ADC); // 2.3 mA
        //_EnterSleepMode(SLEEP_MODE_PWR_DOWN); // 0.000 mA, cannot measure
        //_EnterSleepMode(SLEEP_MODE_PWR_SAVE); // 4.3 mA, does not work, do not use
    }

    while(1) {
        // PRR not set
        // programmer AVR JTAG ICE mkII plugged in
        // active 6.3 mA
        //_EnterSleepMode(SLEEP_MODE_IDLE); // 4.7 mA
        //_EnterSleepMode(SLEEP_MODE_ADC); // 3.6 mA
        //_EnterSleepMode(SLEEP_MODE_PWR_DOWN); // 1.4 mA
        //_EnterSleepMode(SLEEP_MODE_PWR_SAVE); // 6.5 mA, does not work, do not use
    }*/

    while(1) {
        PORTB |=  _BV(0);
        PORTB &= ~_BV(0);
        _EnterSleepMode(SLEEP_MODE_IDLE);
        while((PINB & _BV(2)) == 0);

        PORTB |=  _BV(0);
        PORTB &= ~_BV(0);
        PORTB |=  _BV(0);
        PORTB &= ~_BV(0);
        _EnterSleepMode(SLEEP_MODE_ADC);
        while((PINB & _BV(2)) == 0);

        PORTB |=  _BV(0);
        PORTB &= ~_BV(0);
        PORTB |=  _BV(0);
        PORTB &= ~_BV(0);
        PORTB |=  _BV(0);
        PORTB &= ~_BV(0);
        _EnterSleepMode(SLEEP_MODE_PWR_DOWN);   // wake from INT0, low level interrupt only
        while((PINB & _BV(2)) == 0);
    }
    return(0);
}
