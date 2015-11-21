
/**
 * fade
 */

#include "fade.h"
//#include <stdio.h>

/**
 * c0 - c1 (stop - start)
 */
static int16_t fade_CalcDelta(uint8_t col0, uint8_t col1) {
		uint8_16_t c0, c1;
		//printf("_delta():\ncol0: %d, col1: %d\n", col0, col1);
		c0.u8.h = col0;
		c0.u8.l = 0;
		c0.u16 >>= 1;
		c1.u8.h = col1;
		c1.u8.l = 0;
		c1.u16 >>= 1;
		//printf("c0.u16: %d, c1.u16: %d\n", c0.u16, c1.u16);
		return((int16_t)(c0.u16 - c1.u16));
}

static void fade_SetCalcColor(fade_color_t *fade, rgb_color_t *color) {
	fade->calc_color.red.u8.h = color->red;
	fade->calc_color.red.u16 >>= 1;
	fade->calc_color.green.u8.h = color->green;
	fade->calc_color.green.u16 >>= 1;
	fade->calc_color.blue.u8.h = color->blue;
	fade->calc_color.blue.u16 >>= 1;
}

/**
 * start fading between two colors
 * @param	start	color to start from
 * @param	stop	color at the end
 * @param	fade	pointer to fading context
 * @param	steps	number of steps to fade between stop and start
 */
void fade_Start(rgb_color_t *start, rgb_color_t *stop, fade_color_t *fade, uint8_t steps) {
	//printf("\nfade_Start()\n");
	if(steps < 1) {
		return;
	}
	// set calc_color and stop values
	fade_SetCalcColor(fade, start);
	//printf("calc color red: %d, green: %d, blue: %d\n", fade->calc_color.red.u16, fade->calc_color.green.u16, fade->calc_color.blue.u16);
	fade->stop.red = stop->red;
	fade->stop.green = stop->green;
	fade->stop.blue = stop->blue;
	//printf("stop    red: %d, green: %d, blue: %d\n", fade->stop.red, fade->stop.green, fade->stop.blue);
	fade->step = steps-1;
	//printf("step: %d\n", fade->step);

	// calc deltas with more accuracy
	fade->delta.red = fade_CalcDelta(stop->red, start->red);
	//printf("fade delta red (pre): %d\n", fade->delta.red);
	fade->delta.red = fade->delta.red / steps;
	//printf("fade delta red: %d\n", fade->delta.red);

	fade->delta.green = fade_CalcDelta(stop->green, start->green);
	//printf("fade delta green (pre): %d\n", fade->delta.green);
	fade->delta.green = fade->delta.green / steps;
	//printf("fade delta green: %d\n", fade->delta.green);

	fade->delta.blue = fade_CalcDelta(stop->blue, start->blue);
	//printf("fade delta blue (pre): %d\n", fade->delta.blue);
	fade->delta.blue = fade->delta.blue / steps;
	//printf("fade delta blue: %d\n", fade->delta.blue);
}

/**
 * calculate the next color in fading between stop and start
 * @param	fade	pointer to fading context
 * @return	=0: fading is done, =1: fading in progress
 */
uint8_t fade_Next(fade_color_t *fade) {
	uint8_16_t c;
	//printf("\nfade_Next(), step: %d\n", fade->step);
	if(fade->step == 0) {
		fade->step = 0;
		fade_SetCalcColor(fade, &fade->stop);
		//printf("calc color red: %d, green: %d, blue: %d\n", fade->calc_color.red.u16, fade->calc_color.green.u16, fade->calc_color.blue.u16);
		return(0);
	}
	fade->step--;
	// calc new color value
	fade->calc_color.red.u16 += fade->delta.red;
	fade->calc_color.green.u16 += fade->delta.green;
	fade->calc_color.blue.u16 += fade->delta.blue;

	//printf("calc color red: %d, green: %d, blue: %d\n", fade->calc_color.red.u16, fade->calc_color.green.u16, fade->calc_color.blue.u16);
	return(1);
}

/**
 * get current color from fading color
 * @param	fade			get calculating color from here
 * @param	current		to get current color
 */
void fade_GetCurrentColor(fade_color_t *fade, rgb_color_t *current) {
	uint8_16_t c;
	//printf("fade_GetCurrentColor()\n");
	c.u16 = fade->calc_color.red.u16;
	//printf("c.u16: %d\n", c.u16);
	c.u16 <<= 1;
	//printf("c.u16: %d, c.u8.h: %d, c.u8.l: %d\n", c.u16, c.u8.h, c.u8.l);
	current->red = c.u8.h;
	//printf("current: %d\n", current->red);

	c.u16 = fade->calc_color.green.u16;
 	c.u16 <<= 1;
	current->green = c.u8.h;
	c.u16 = fade->calc_color.blue.u16;
 	c.u16 <<= 1;
	current->blue = c.u8.h;
}
