# Encoder

This code is based on code collected from multiple online sources when searching 
for "arduino encoder".

## Usage
A simple implementation:

```
#include <Encoder.h>

// clk (0) and dt (1) pins connected to the encoder through a 10k resistor
// debounce set to 100 (ms) to avoid noise from the encoder
Encoder encoder(0, 1, 100); 

void setup() {
    Serial.begin(9600); // start serial writer
    while(!Serial); // wait for serial to init
    encoder.setup(); // initialize the sensor
}

void loop() {
  encoder.loop([](int direction) {
    // Direction will be 1 or -1 based on the direction that the encoder was 
    // turned.
    // Callback will only be called when there is consistent scroll direction 
    // throughout the entire debounce period.
    Serial.println(direction);
  });
}
```

## Legal
See LICENSE.md in root