Code written specifically for **Arduino Uno REV3**.

## Hardware Configuration

[Arduino Uno REV3](https://www.amazon.com/Arduino-A000066-ARDUINO-UNO-R3/dp/B008GRTSV6)

[Dual-axis joystick](https://www.amazon.com/Joystick-Console-Dual-Axis-Compatible-Raspberry/dp/B0D83FD3M8)
```
GND => GND (Arduino)
+5V => 5V  ...
VRX => A0
VRY => A1
```

[Screen](https://www.amazon.com/128x160-Display-ST7735S-Replace-Arduino/dp/B0F1C8X7D8?crid=1UN5AV648102T)
```
GND => GND  (Arduino)
VDD => 3.3V ...
SCL => 13
SDA => 11
RST => 7
DC  => 8
CS  => 4
BLK => 3.3V
```

## Compilation and Uploading

```
avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -o out.elf <PROGRAM>.c
avr-objcopy -O ihex -R .eeprom out.elf out.hex
```

Once your Arduino is plugged into your computer, check what port it's connected to with `ls /dev/cu.usbmodem*` (Mac only), which you need for uploading:

```
avrdude -F -V -c arduino -p m328p -P /dev/cu.usbmodem<PORT> -b 115200 -U flash:w:main.hex
```

## Notes

**Goal:** affordable open source handheld entertainment console with focus on 2D (a playdate is $229 bro)

![nintendo switch lite](https://media.gamestop.com/i/gamestop/11095775/Nintendo-Switch-Lite-Console-Turquoise)

Handheld console hardware

Might take a while to reach this goal given our recent revelations about arduino lol but at least we can learn about hardware :D

We should treat this as a personal project instead of a public one for now (so less flexibility in what it can do)

The thing is that you don’t buy a switch for the hardware, you buy a switch for the compatibility with popular games, but if you don’t care about those games, you can really make something similar (lower spec obviously) for much cheaper and just make your own games

one game-console

### Hardware Specification

- Arduino
  - Machine-code compiles from C
  - No cartridges (for now…?) — all games are loaded into onboard memory (filesystem) (SMB1 was 40KB, this only has 32KB, erm)
  - Apparently arduino isn’t good for projects that require dynamically loading and executing code, which isn’t necessary if we aren’t reading programs off SD cards but hmmmm arduino seems a bit toooo limited…
- 4 tactile face buttons
- screen
  - DS is 3-inch diagonal (256×192px)
  - Nintendo Switch Lite is 5.5-inch diagonal (1280×720px) (hella overkill)
  - Refresh rate is kinda slow but this project is limited anyways sooo

### Software Resources

Custom barebones OS that can launch games (as child processes) from the filesystem (impossible with arduino) and provides a hardware-abstraction framework (via pipes!)

### Games I like :3

- Arcade games like super money ball
- 2D and 3D platformers, especially collectathons, like super mario 3 and galaxy
- Movement shooters (impossible with one stick)
- Farming sims like Stardew Valley and Animal Crossing
- Linear story like bebder game
- Racing games like mario kart
- Tetris
- Number-go-up games like Balatro
- Room-based RPG like the original LoZ to make rendering easier, generic save-the-fat-princess from the demon king

Make, like, pong or asteroids first, something self-contained and easy, to make you enjoy using your entertainment console before committing to a bigger project
