#include "hardware.h"

int main() {

	init_hardware();

	fill_rect(0, 0, 16, 32, 0xF884);

	// // disable SPI (necessary for pin 13 to correspond to the onboard LED)
	// SPCR = 0;

	// // loop
    // while (1) {

	// 	// set pin 5 high to turn led on
	// 	PULL_HIGH(PORTB, 5);
	// 	_delay_ms(100);

	// 	// set pin 5 low to turn led off
	// 	PULL_LOW(PORTB, 5);
	// 	_delay_ms(100);
    // }
}
