#include <PMW3360.h>
#include <Mouse.h>

PMW3360 sensor(5, 1600);

void setup() {
  Serial.begin(96000);
  Mouse.begin();
  sensor.setup();
}

void loop() {
  sensor.loop([](int x, int y){
    Mouse.move(x,y,0);
  });
}
