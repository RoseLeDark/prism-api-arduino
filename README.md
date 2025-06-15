# prism-api-arduino
This is the PRISM Host library for Arduino allowing you to communicate with a PRISM SIMD coprocessor over I²C.

Click here to learn more about the PRISM SIMD coprocessor.

The default I²C address of PRISM SIMD is 0x52 and 0x51 is broadcast for all devices.

## Connect the PRISM SIMD coprocessor
PRISM CoProcessor use P²Link - I²C with a low-level 8Bit-Parallelbus 

| *Arduino PIN* | *Name* | *Description* | *Color*
|:-------------:|--------|---------------|:------:
| D2 | PRISM_PIN_1LOW |  Low Bit 0 P²Link | Blue
| D3 | PRISM_PIN_2LOW |  Low Bit 1 P²Link | Blue
| D4 | PRISM_PIN_3LOW |  Low Bit 2 P²Link | Blue
| D5 | PRISM_PIN_4LOW |  Low Bit 3 P²Link | Blue
| D6 | PRISM_PIN_CLK_DEFAULT| Timing (Taktleitung) | White
| D7 | PRISM_PIN_NXT_DEFAULT | Entry-Flag (neuer Eintrag beginnt) | Yellow
| D8 | PRISM_PIN_7HIGH_DEFAULT | High Bit 0 P²Link | Green
| D9 | PRISM_PIN_8HIGH_DEFAULT | High Bit 1 P²Link | Green
| D10 | PRISM_PIN_9HIGH_DEFAULT | High Bit 2 P²Link | Green
| D11 | PRISM_PIN_10HIGH_DEFAULT | High Bit 3 P²Link | Green
| D18 | SDA | I2C: Serial data input / output | Orange
| D19 | SCL | I2C: Serial clock input | Gray
| V | 3.3V  | Power In PRISM | Red
| GND | GND  | Ground | Black

The recommended voltage is 3.3V.

### Board specific wiring
You will find pinout schematics for recommended board models below:

| *GPIO ESP32* | *Name* | Color
|:-------------:|--------|---------------
| 36 | PRISM_PIN_1LOW | Blue
| 39 | PRISM_PIN_2LOW | Blue
| 34 | PRISM_PIN_3LOW  | Blue
| 35 | PRISM_PIN_4LOW  | Blue
| 32 | PRISM_PIN_CLK_DEFAULT|  White
| 33 | PRISM_PIN_NXT_DEFAULT |  Yellow
| 25 | PRISM_PIN_7HIGH_DEFAULT |  Green
| 26 | PRISM_PIN_8HIGH_DEFAULT | Green
| 27 | PRISM_PIN_9HIGH_DEFAULT | Green
| 14 | PRISM_PIN_10HIGH_DEFAULT | Green
| 21 | I²C SDA | Orange
| 22 | I²C SCL |  Gray
| VIN | 3.3V  | Red
| GND | GND | Black

## Contributing

**Contributions are welcome!**

This Sensirion library uses
[`clang-format`](https://releases.llvm.org/download.html) to standardize the
formatting of all our `.cpp` and `.h` files. Make sure your contributions are
formatted accordingly:

The `-i` flag will apply the format changes to the files listed.

```bash
clang-format -i src/*.cpp inc/prism/*.h
```


## License

See [LICENSE](LICENSE).
