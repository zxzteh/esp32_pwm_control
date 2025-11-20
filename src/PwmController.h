#pragma once

#include <stdint.h>

class PwmController {
public:
    using SetPwmFn = void(*)(uint8_t duty);

    PwmController(SetPwmFn setPwm);

    void increaseByPercent(uint8_t percent);
    void decreaseByPercent(uint8_t percent);

    void setRaw(uint8_t duty);
    void setPercent(uint8_t percent);

    void off();
    void fullOn();

    uint8_t getPercent() const { return currentPercent_; }
    uint8_t getRaw() const { return currentRawDuty_; }

    void setBreathe();
    void resetBreathe();
    void breatheTick();

private:
    SetPwmFn setPwm_ = nullptr;
    uint8_t currentRawDuty_;
    uint8_t currentPercent_;
    bool breatheOn_;
    bool breathingUp_;
};
