# Red Team

## Building and Running Tests

```
cd tests
make clean; make all; ./main
```

## Building and installing to ESP32
* Windows: Install USB/UART bridge: https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers
* Install platform IO: https://platformio.org/
* Upload using the IDE or `pio run -t upload`
* View serial port with `pio device monitor`

Flow Sensor Datasheet: https://www.sensirion.com/en/flow-sensors/mass-flow-meters-for-high-precise-measurement-of-gases/proximal-flow-sensors-sfm3300-autoclavable-washable-or-single-use/
