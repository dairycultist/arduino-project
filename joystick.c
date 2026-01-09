// #include <avr/io.h>
// #define __DELAY_BACKWARD_COMPATIBLE__
// #include <util/delay.h>

// // function to read a value from a single ADC channel and return it as a 10-bit number (10-bit resolution = 1024 steps)
// uint16_t readADC(uint8_t channel) {

//     // MUX0..3 = 4-bit number representing channel
//     ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); 

//     // start the conversion
//     ADCSRA |= _BV(ADSC); 

//     // wait for the conversion to complete
//     while (ADCSRA & _BV(ADSC)); 

//     // return the 10-bit result (ADCL and ADCH registers are combined)
//     return ADC;
// }

// int main(void) {

// 	// set pin 5 of PORTB for output
// 	DDRB |= _BV(DDB5); // Using DDRB (Data Direction Register B)

//     // initialize Analog-to-Digital Converter (ADC)

//     // REFS0   = 1 : Select AVcc as reference voltage (usually 5V or 3.3V, depending on board)
//     // MUX0..3 = 0 : Select ADC0 pin (A0)
//     ADMUX = _BV(REFS0);

//     // ADEN     = 1     : enable the ADC
//     // ADPS0..2 = 0b111 : set ADC prescaler to 128
//     // this makes the ADC clock 16MHz/128 = 125kHz, which is within the recommended 50kHz-200kHz range for full 10-bit resolution
//     ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);

// 	// loop
//     while (1) {

// 		// Read voltage from A0
//         float vrx = (float) readADC(0) / 1024.0;

// 		// set pin 5 high to turn led on
// 		PORTB |= _BV(PORTB5);
// 		_delay_ms(vrx * 500 + 100);

// 		// set pin 5 low to turn led off
// 		PORTB &= ~_BV(PORTB5);
// 		_delay_ms(vrx * 500 + 100);
//     }
// }
