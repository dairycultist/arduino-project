Code written specifically for **Arduino Uno REV3**.

## Compilation and Uploading

```
avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -o out.elf <PROGRAM>.c
avr-objcopy -O ihex -R .eeprom out.elf out.hex
```

Once your Arduino is plugged into your computer, check what port it's connected to with `ls /dev/cu.usbmodem*` (Mac only), which you need for uploading:

```
avrdude -F -V -c arduino -p m328p -P /dev/cu.usbmodem<PORT> -b 115200 -U flash:w:main.hex
```
