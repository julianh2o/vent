# Red Team

## Building and Running Tests

```
cd tests
make clean; make all; ./main
```

## Building and installing to ESP32
* Windows: Install USB/UART bridge: https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers
* Install platform IO: https://platformio.org/
* Upload using the IDE or `platformio run -t upload`

TODO:
* Test LCD library: https://platformio.org/lib/show/571/Adafruit%20ILI9341
