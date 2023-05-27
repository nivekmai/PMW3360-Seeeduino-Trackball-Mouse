#include <Mouse.h>
#include <PMW3360.h>
#include <ezButton.h>

#define DEBOUNCE 50

ezButton buttonL(4);
ezButton buttonM(2);
ezButton buttonR(3);
PMW3360 sensor(6, 1600);

void setup() {
  Serial.begin(96000);
  Mouse.begin();
  sensor.setup();
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
