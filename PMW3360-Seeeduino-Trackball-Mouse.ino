#include <Mouse.h>
#include <Keyboard.h>
#include <Encoder.h>
#include <PMW3360.h>
#include <ezButton.h>

#define DEBOUNCE 50

Encoder encoder(0, 1, 100);

ezButton buttonL(4);
ezButton buttonM(2);
ezButton buttonR(3);
PMW3360 sensor(6, 1600);
bool middleDown = false;
int scrollcompY = 0;
int SCROLL_CHUNK_Y = 50;

void setup() {
  Serial.begin(9600);
  Mouse.begin();
  Keyboard.begin();
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
  middleButtonLoop();
  buttonLoop(buttonL, MOUSE_LEFT);
  sensor.loop([](int x, int y) {
    onTrackballMove(x, y);
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

void middleButtonLoop() {
  if (buttonM.isPressed()) {
    middleDown = true;
  }
  if (buttonM.isReleased()) {
    middleDown = false;
  }
}



void onTrackballMove(int x, int y) {
  if (middleDown) {
    scrollcompY -= x;
    Mouse.move(0, 0, scrollcompY/SCROLL_CHUNK_Y);
    scrollcompY = scrollcompY % SCROLL_CHUNK_Y;
  } else {
    Mouse.move(y, x, 0);
  }
}
