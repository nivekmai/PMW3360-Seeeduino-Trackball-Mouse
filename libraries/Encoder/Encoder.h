#ifndef Encoder_h
#define Encoder_h

#include <functional>

#include "Arduino.h"

class Encoder {
   public:
    Encoder(uint8_t clk, uint8_t dt, int debounce);
    void setup();

    void loop(std::function<void(int)> callback);

   private:
    uint8_t _clk;
    uint8_t _dt;
    int _debounce;
    int _currentStateCLK;
    int _lastStateCLK;
    int _debounceTs;
    int _debounceDir;
};

#endif