#include "hardware.h"

int main() {

	init_hardware();

	while (1) {
		fill_rect(0, 0, 128, 160, get_x1024() << 1);
		sleep();
	}
}
