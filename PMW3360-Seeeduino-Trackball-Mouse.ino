#include <Mouse.h>
#include <Encoder.h>
#include <PMW3360.h>
#include <ezButton.h>

#define DEBOUNCE 50

Encoder encoder(0, 1, DEBOUNCE);

ezButton buttonL(4);
ezButton buttonM(2);
ezButton buttonR(3);
PMW3360 sensor(6, 1600);

void setup() {
  Serial.begin(9600);
  Mouse.begin();
  sensor.setup();
  encoder.setup();
  buttonL.setDebounceTime(DEBOUNCE);
  buttonM.setDebounceTime(DEBOUNCE);
  buttonR.setDebounceTime(DEBOUNCE);
}

void loop() {
  buttonL.loop();
  buttonM.loop();
  buttonR.loop();
  buttonLoop(buttonR, MOUSE_RIGHT);
  buttonLoop(buttonM, MOUSE_MIDDLE);
  buttonLoop(buttonL, MOUSE_LEFT);
  sensor.loop([](int x, int y) {
    Mouse.move(x, y, 0);
  });
  encoder.loop([](int scroll) {
    Mouse.move(0, 0, scroll);
  });
}

void buttonLoop(ezButton button, char mouseButton) {
  if (button.isPressed()) {
    Mouse.press(mouseButton);
  }
  if (button.isReleased()) {
    Mouse.release(mouseButton);
  }
}
