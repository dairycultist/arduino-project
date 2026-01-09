@PHONY: upload

upload: ./*.c
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -o out.elf $^
	avr-objcopy -O ihex -R .eeprom out.elf out.hex
	avrdude -F -V -c arduino -p m328p -P $(shell ls /dev/cu.usbmodem*) -b 115200 -U flash:w:out.hex