# PMW3360-Seeeduino-Trackball-Mouse
Connecting PMW3360 sensor to a Seeeduino XIAO

### BOM:
- [Encoder Switch Mouse Encoder Scroll Wheel](https://www.amazon.com/dp/B07HCGLG3Z)
  - I pulled this out of an old mouse to get the mouse wheel too 
- [PMW3360 Motion Sensor](https://www.tindie.com/products/jkicklighter/pmw3360-motion-sensor/)
- [Seeed Studio XIAO SAMD21](https://www.seeedstudio.com/Seeeduino-XIAO-Arduino-Microcontroller-SAMD21-Cortex-M0+-p-4426.html)
- [2 10kOhm resistors](https://www.amazon.com/gp/product/B016NXK6QK)
- Wires
  - For cleanliness, I connected to the PMW3360 using some [flat ribbon cable](https://www.amazon.com/dp/B088M5KJ4F)

### Setup for PMW3360 <> Seeeduino Xiao

- VI -> 5V
- GD -> GND
- MO -> A10/D10/MOSI
- MI -> A9/D9/MISO
- SC -> A8/D8/SCK
- MT -> A7/D7/RX
- SS -> A5/D5/SCL

### Setup for the Rotary Encoder <> Seeeduino Xiao

- A -> 5V -> resistor -> A0/D0
- B -> 5V -> resistor -> A1/D1
- C -> GND

> NOTE: I see some diagrams online that also hook the A/B pins to ground through a 1µF capacitor, but I didn't need that (maybe that's I needed to do the debouncing logic though).

### Setup for the buttons

- GND -> Button -> A2/D2
- GND -> Button -> A3/D3
- GND -> Button -> A4/D4

### Wiring Diagram

![Wiring diagram](assets/Wiring.svg)
