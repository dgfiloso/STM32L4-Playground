# STM32L4 Room Monitor

Measure the temperature, relative humidity and atmospheric pressure of a room and display to a LCD screen.

## Consumption

Measured connecting the jumper JP5 with a multimeter.

### Without Low Power Implementation

120 uA to run the sensor measurement.
100 uA while waiting.

### With Low Power Implementation

90 uA to wake up and run the sensor measurement.
70 uA in sleep mode.
