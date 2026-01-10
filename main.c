#include "hardware.h"

int main() {

	init_hardware();

	uint16_t prev_x = 0;
	uint16_t prev_y = 0;

	while (1) {

		// clear
		fill_rect(prev_x, prev_y, 10, 10, 0x0000);

		prev_x = get_x1024() >> 4;
		prev_y = get_y1024() >> 4;

		fill_rect(prev_x, prev_y, 10, 10, 0xF448);

		sleep();
	}
}
