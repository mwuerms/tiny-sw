/**
 * Martin Wuerms
 * 2015-11-20
 *
 */

#ifndef _FADE_H_
#define _FADE_H_

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include "ledDriver.h"

/* - defines ---------------------------------------------------------------- */

/* - typedef ---------------------------------------------------------------- */
typedef union {
	uint16_t	u16;
	struct {
		uint8_t l;
		uint8_t h;
	} u8;
} uint8_16_t;

/*typedef struct {
	uint8_t	red;
	uint8_t	green;
	uint8_t blue;
} color_t;*/

typedef struct {
	struct {
		int16_t	red;
		int16_t	green;
		int16_t blue;
	} delta;
	struct {
		uint8_16_t red;
		uint8_16_t green;
		uint8_16_t blue;
	} calc_color;
	rgb_color_t	stop;
	uint8_t step;
} fade_color_t;	// fading color context

/* - public functions ------------------------------------------------------- */
void fade_Start(rgb_color_t *start, rgb_color_t *stop, fade_color_t *fade, uint8_t steps);
uint8_t fade_Next(fade_color_t *fade);
void fade_GetCurrentColor(fade_color_t *fade, rgb_color_t *current);

#endif /* _FADE_H_ */
