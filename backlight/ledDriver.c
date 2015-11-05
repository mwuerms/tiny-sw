/**
 * Martin Wuerms
 * 2015-09-13
 */

/* - includes --------------------------------------------------------------- */
#include <avr/io.h>
#include <string.h>
#include "ledDriver.h"
#include "arch.h"

/* - typedefs --------------------------------------------------------------- */

/* - defines ---------------------------------------------------------------- */

/* - variables -------------------------------------------------------------- */

/* - private functions  ----------------------------------------------------- */

/* - public functions ------------------------------------------------------- */

/**
 * update the send buffer
 * === USING PORTB ===
 * @param   send    pointer to colors to send
 * @param   length  nb of colors to send
 * @param   led_pin pin on which the signal is sent out
 * @return  =1: OK
 *          =-1: error, could not update send buffer
 */
int8_t ledDriver_Set(rgb_color_t *send, uint8_t length, uint8_t led_pin) {
    uint8_t sr;
    lock_interrupt(sr);
    /**
     * use WS2812 routines from Adafruits NeoPixel library
     * source: https://github.com/adafruit/Adafruit_NeoPixel, methode show()
     * use PORTB, led_pin, 400kHz
     */
    volatile uint16_t
    i   = length*sizeof(rgb_color_t);//numBytes; // Loop counter
    volatile uint8_t
    *ptr = (uint8_t *)send, //pixels,   // Pointer to next byte
    b   = *ptr++,   // Current byte value
    hi,             // PORT w/output bit set high
    lo;             // PORT w/output bit set low
    volatile uint8_t    *port;

    // 400 kHz
    // Timing is more relaxed; unrolling the inner loop for each bit is
    // not necessary.  Still using the peculiar RJMPs as 2X NOPs, not out
    // of need but just to trim the code size down a little.
    // This 400-KHz-datastream-on-8-MHz-CPU code is not quite identical
    // to the 800-on-16 code later -- the hi/lo timing between WS2811 and
    // WS2812 is not simply a 2:1 scale!

    // 20 inst. clocks per bit: HHHHxxxxxxLLLLLLLLLL
    // ST instructions:         ^   ^     ^          (T=0,4,10)

    volatile uint8_t next, bit;

    DDRB  |= led_pin;
    PORTB &= led_pin;
    hi = PORTB |  led_pin;
    lo = PORTB & ~led_pin;
    next = lo;
    bit  = 8;
    port = &PORTB;

    asm volatile (
    "head20:"                  "\n\t" // Clk  Pseudocode    (T =  0)
     "st   %a[port], %[hi]"    "\n\t" // 2    PORT = hi     (T =  2)
     "sbrc %[byte] , 7"        "\n\t" // 1-2  if(b & 128)
      "mov  %[next], %[hi]"    "\n\t" // 0-1   next = hi    (T =  4)
     "st   %a[port], %[next]"  "\n\t" // 2    PORT = next   (T =  6)
     "mov  %[next] , %[lo]"    "\n\t" // 1    next = lo     (T =  7)
     "dec  %[bit]"             "\n\t" // 1    bit--         (T =  8)
     "breq nextbyte20"         "\n\t" // 1-2  if(bit == 0)
     "rol  %[byte]"            "\n\t" // 1    b <<= 1       (T = 10)
     "st   %a[port], %[lo]"    "\n\t" // 2    PORT = lo     (T = 12)
     "rjmp .+0"                "\n\t" // 2    nop nop       (T = 14)
     "rjmp .+0"                "\n\t" // 2    nop nop       (T = 16)
     "rjmp .+0"                "\n\t" // 2    nop nop       (T = 18)
     "rjmp head20"             "\n\t" // 2    -> head20 (next bit out)
    "nextbyte20:"              "\n\t" //                    (T = 10)
     "st   %a[port], %[lo]"    "\n\t" // 2    PORT = lo     (T = 12)
     "nop"                     "\n\t" // 1    nop           (T = 13)
     "ldi  %[bit]  , 8"        "\n\t" // 1    bit = 8       (T = 14)
     "ld   %[byte] , %a[ptr]+" "\n\t" // 2    b = *ptr++    (T = 16)
     "sbiw %[count], 1"        "\n\t" // 2    i--           (T = 18)
     "brne head20"             "\n"   // 2    if(i != 0) -> (next byte)
     : [port]  "+e" (port),
       [byte]  "+r" (b),
       [bit]   "+r" (bit),
       [next]  "+r" (next),
       [count] "+w" (i)
     : [hi]    "r" (hi),
       [lo]    "r" (lo),
       [ptr]   "e" (ptr));

    PORTB &= led_pin;
    restore_interrupt(sr);
    return(1);
}
