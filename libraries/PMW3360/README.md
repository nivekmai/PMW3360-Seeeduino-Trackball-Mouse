# PMW3360 library

This code is largely based on https://github.com/mrjohnk/PMW3360DM-T2QU, but has been extracted into a cleaner reusable library.

## Usage
A simple implementation:

```
#include <PMW3360.h>

PMW3360 sensor(6, 1600); // set the slave select pin and CPI

void setup() {
    Serial.begin(9600); // start serial writer
    while(!Serial); // wait for serial to init
    sensor.setup(); // initialize the sensor
}

void loop() {
  sensor.loop([](int x, int y) {
    // the callback will only be called when there's movement
    Serial.print(x);
    Serial.print("\t");
    Serial.print(y);
    Serial.println();
  });
}
```

## Legal
See LICENSE.md in root