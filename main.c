#include "hardware.h"

int main() {

	init_hardware();

	uint16_t prev_x = 0;
	uint16_t prev_y = 0;

	uint16_t new_x = 0;
	uint16_t new_y = 0;

	while (1) {

		new_x = get_x1024() >> 4;
		new_y = get_y1024() >> 4;

		if (new_x != prev_x || new_y != prev_y) {

			// clear
			fill_rect(prev_x, prev_y, 10, 10, 0x00, 0x00, 0x00);

			// draw square
			fill_rect(prev_x = new_x, prev_y = new_y, 10, 10, 0x0F, 0x0F, 0x0F);
		}
		
		sleep();
	}
}
