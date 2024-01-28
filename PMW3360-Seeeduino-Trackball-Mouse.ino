#include <Mouse.h>
#include <Keyboard.h>
#include <PMW3360.h>
#include <ezButton.h>

// debounce time for buttons to avoid duplicate clicks
#define DEBOUNCE 50
// chunk the scrolls such that the ball needs to move this 
// distance before a scroll movement (one line) is sent 
#define SCROLL_CHUNK_Y 50


ezButton buttonL(4);
ezButton buttonM(2);
ezButton buttonR(3);
PMW3360 sensor(6, 1600);
bool middleDown = false;
int scrollcompY = 0;

void setup() {
  Mouse.begin();
  Keyboard.begin();
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
    // Rotated 90 deg with normal scrolling
    scrollcompY -= x;
    Mouse.move(0, 0, scrollcompY/SCROLL_CHUNK_Y);
    scrollcompY = scrollcompY % SCROLL_CHUNK_Y;
  } else {
    // Rotated 90 deg
    Mouse.move(y, x, 0);
  }
}
