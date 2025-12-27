/*
avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -o screen.elf screen.c
avr-objcopy -O ihex -R .eeprom screen.elf screen.hex

ls /dev/cu.usbmodem*

avrdude -F -V -c arduino -p m328p -P /dev/cu.usbmodem14401 -b 115200 -U flash:w:screen.hex
 */

#include <avr/io.h>
#include <util/delay.h>

// DDR = Data Direction Register
#define SET_PIN_AS_OUT(ddr, pin) ((ddr) |= _BV(pin))

#define PULL_LOW(port, pin) ((port) &= ~_BV(pin))
#define PULL_HIGH(port, pin) ((port) |= _BV(pin))

void SPI_init() {

	// initialize (Arduino Uno-specific for SPI) pins as output
	SET_PIN_AS_OUT(DDRB, 2); // MUST BE OUTPUT (it's called the CS pin but that's for if this arduino is a... puppet, but we're using it as master)
	SET_PIN_AS_OUT(DDRB, 3); // SDA (MOSI)
	SET_PIN_AS_OUT(DDRB, 5); // SCL (SCK)

	// initialize SPI (with clock rate = fosc/4)
    // SPE = 1 (Enable SPI), MSTR = 1 (Master mode)
    SPCR = (1 << SPE) | (1 << MSTR);
}

uint8_t SPI_transfer(uint8_t data) {

    // Start transmission
    SPDR = data;
    
    // Wait for transmission complete (SPIF bit in SPSR becomes 1)
    while (!(SPSR & (1 << SPIF)));
    
    // Return received data
    return SPDR; // Even if you only want to send data, reading the register clears the flag
}

void ST7735S_send_command(uint8_t command) {

	PULL_LOW(PORTB, 0); // DC low for commands
	PULL_LOW(PORTD, 4);
	SPI_transfer(command);
	PULL_HIGH(PORTD, 4);
}

void ST7735S_send_data(uint8_t data) {

	PULL_HIGH(PORTB, 0); // DC high for data
	PULL_LOW(PORTD, 4);
	SPI_transfer(data);
	PULL_HIGH(PORTD, 4);
}

void ST7735S_init() {

	/*
	 * initialize relevant pins as output
	 */
	SET_PIN_AS_OUT(DDRD, 7); // RST
	SET_PIN_AS_OUT(DDRB, 0); // DC
	SET_PIN_AS_OUT(DDRD, 4); // CS

	/*
	 * power-on sequence
	 */
	PULL_LOW(PORTD, 7); // hardware reset
	_delay_ms(100);
	PULL_HIGH(PORTD, 7);

	ST7735S_send_command(0x01); // Software Reset
	_delay_ms(120);

	ST7735S_send_command(0x11); // Sleep Out (turns screen on)
	_delay_ms(120);

	ST7735S_send_command(0x3A); // Color Mode
	ST7735S_send_data(0x05); // 16-bit color (RGB565)

	ST7735S_send_command(0x29); // Display On
}

int main(void) {

	SPI_init();
	ST7735S_init();

	/*
	 * draw something
	 */
	ST7735S_send_command(0x2A); // Column Address Set
	ST7735S_send_data(0x00); // start_x
	ST7735S_send_data(0x00);
	ST7735S_send_data(0x00); // end_x
	ST7735S_send_data(0x08);

	ST7735S_send_command(0x2B); // Row Address Set
	ST7735S_send_data(0x00); // start_y
	ST7735S_send_data(0x00);
	ST7735S_send_data(0x00); // end_y
	ST7735S_send_data(0x20);

	ST7735S_send_command(0x2C); // Memory Write

	for (int i = 0; i < 256; i++) {
		ST7735S_send_data(0xF8); // pixel color data!
		ST7735S_send_data(0x84);
	}

	// disable SPI (necessary for pin 13 to correspond to the onboard LED)
	SPCR = 0;

	// loop
    while (1) {

		// set pin 5 high to turn led on
		PULL_HIGH(PORTB, 5);
		_delay_ms(100);

		// set pin 5 low to turn led off
		PULL_LOW(PORTB, 5);
		_delay_ms(100);
    }
}
