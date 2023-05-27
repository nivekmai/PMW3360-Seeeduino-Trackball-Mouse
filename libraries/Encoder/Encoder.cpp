#include "Encoder.h"

Encoder::Encoder(uint8_t clk, uint8_t dt, int debounce) {
    _clk = clk;
    _dt = dt;
    _debounce = debounce;
}

void Encoder::setup() {
    pinMode(_clk, INPUT);
    pinMode(_dt, INPUT);
    _lastStateCLK = digitalRead(_clk);
    _debounceTs = millis();
}

void Encoder::loop(std::function<void(int)> callback) {
    int direction;
    _currentStateCLK = digitalRead(_clk);
    if (_currentStateCLK != _lastStateCLK && _currentStateCLK == 1) {
        if (digitalRead(_dt) != _currentStateCLK) {
            direction = -1;
        } else {
            direction = 1;
        }
        int nextTs = millis();
        if ((nextTs - _debounceTs > _debounce) || direction == _debounceDir) {
            _debounceDir = direction;
            callback(direction);
            _debounceTs = nextTs;
        }
    }
    _lastStateCLK = _currentStateCLK;
}