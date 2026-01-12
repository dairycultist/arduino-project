#include "hardware.h"
#include <avr/io.h>
#include <util/delay.h>

// DDR = Data Direction Register
#define SET_PIN_AS_OUT(ddr, pin) ((ddr) |= _BV(pin))

#define PULL_LOW(port, pin) ((port) &= ~_BV(pin))
#define PULL_HIGH(port, pin) ((port) |= _BV(pin))

/*
 * screen stuff
 */
static void SPI_init() {

	// initialize (hardware-specific SPI) pins as output
	SET_PIN_AS_OUT(DDRB, 2); // MUST BE OUTPUT (it's called the CS pin but that's for if this arduino is a... puppet, but we're using it as master)
	SET_PIN_AS_OUT(DDRB, 3); // SDA (MOSI)
	SET_PIN_AS_OUT(DDRB, 5); // SCL (SCK)

	// initialize SPI (with clock rate = fosc/4)
    // SPE = 1 (Enable SPI), MSTR = 1 (Master mode)
    SPCR = (1 << SPE) | (1 << MSTR);

	// SPCR |= (1 << SPR0); // SPR1=0, SPR0=1
  	// SPSR |= (1 << SPI2X); // Set SPI2X bit for doubling speed
}

static inline uint8_t SPI_transfer(uint8_t data) {

    // Start transmission
    SPDR = data;
    
    // Wait for transmission complete (SPIF bit in SPSR becomes 1)
    while (!(SPSR & (1 << SPIF)));
    
    // Return received data
    return SPDR; // Even if you only want to *send* data, reading the register clears the flag
}

static inline void ST7735S_start_command() {

	PULL_LOW(PORTB, 0); // DC low for commands
}

static inline void ST7735S_start_data() {

	PULL_HIGH(PORTB, 0); // DC high for data
}

static void ST7735S_init() {

	/*
	 * initialize relevant pins as output
	 */
	SET_PIN_AS_OUT(DDRD, 7); // RST
	SET_PIN_AS_OUT(DDRB, 0); // DC
	SET_PIN_AS_OUT(DDRD, 4); // CS

	/*
	 * select chip (pulling CS high deselects it)
	 */
	PULL_LOW(PORTD, 4);

	/*
	 * power-on sequence
	 */
	PULL_LOW(PORTD, 7); // hardware reset
	_delay_ms(100);
	PULL_HIGH(PORTD, 7);

	ST7735S_start_command();
	SPI_transfer(0x01); // Software Reset
	_delay_ms(120);

	SPI_transfer(0x11); // Sleep Out (turns screen on)
	_delay_ms(120);

	SPI_transfer(0x3A); // Color Mode
	ST7735S_start_data();
	SPI_transfer(0x03); // 12-bit color (RGB444)

	ST7735S_start_command();
	SPI_transfer(0x29); // Display On
}

/*
 * joystick stuff
 */
static void ADC_init() {

    // initialize Analog-to-Digital Converter (ADC)

    // REFS0   = 1 : Select AVcc as reference voltage (usually 5V or 3.3V, depending on board)
    // MUX0..3 = 0 : Select ADC0 pin (A0)
    ADMUX = _BV(REFS0);

    // ADEN     = 1     : enable the ADC
    // ADPS0..2 = 0b111 : set ADC prescaler to 128
    // this makes the ADC clock 16MHz/128 = 125kHz, which is within the recommended 50kHz-200kHz range for full 10-bit resolution
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

// function to read a value from a single ADC channel and return it as a 10-bit number (10-bit resolution = 1024 steps)
static uint16_t ADC_read(uint8_t channel) {

    // MUX0..3 = 4-bit number representing channel
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); 

    // start the conversion
    ADCSRA |= _BV(ADSC); 

    // wait for the conversion to complete
    while (ADCSRA & _BV(ADSC)); 

    // return the 10-bit result (ADCL and ADCH registers are combined)
    return ADC;
}

/*
 * header functions
 */
void init_hardware() {

	SPI_init();
	ST7735S_init();
	ADC_init();
}

void fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint8_t g, uint8_t b) {

	x++;
	y++;

	ST7735S_start_command();
	SPI_transfer(0x2A); 		// Column Address Set
	ST7735S_start_data();
	SPI_transfer(0x00); 		// start_x
	SPI_transfer(x & 0xFF);
	SPI_transfer(0x00); 		// end_x
	SPI_transfer((x + w) & 0xFF);

	ST7735S_start_command();
	SPI_transfer(0x2B); 		// Row Address Set
	ST7735S_start_data();
	SPI_transfer(0x00); 		// start_y
	SPI_transfer(y & 0xFF);
	SPI_transfer(0x00); 		// end_y
	SPI_transfer((y + h) & 0xFF);

	ST7735S_start_command();
	SPI_transfer(0x2C); 		// Memory Write

	// pixel color data
	ST7735S_start_data();

	for (int i = 0; i < (w + 1) * (h + 1) / 2; i++) {
		SPI_transfer(r << 4 | g);
		SPI_transfer(b << 4 | r);
		SPI_transfer(g << 4 | b);
	}
}

inline uint16_t get_x1024() {
	return ADC_read(0);
}

inline uint16_t get_y1024() {
	return ADC_read(1);
}

void sleep() {
	_delay_ms(50);
}







/*
 * onboard LED code which im saving in case I need it
 */
// // disable SPI (necessary for pin 13 to correspond to the onboard LED)
// SPCR = 0;

// // pin 5 of PORTB (pin 13) must be output btw
// // loop
// while (1) {

// 	// set pin 5 high to turn led on
// 	PULL_HIGH(PORTB, 5);
// 	_delay_ms(100);

// 	// set pin 5 low to turn led off
// 	PULL_LOW(PORTB, 5);
// 	_delay_ms(100);
// }