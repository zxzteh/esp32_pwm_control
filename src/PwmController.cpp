#include "PwmController.h"

PwmController::PwmController(SetPwmFn setPwm)
    : setPwm_(setPwm),
      currentRawDuty_(0),
      currentPercent_(0),
      breatheOn_(false),
      breathingUp_(true)
{}

void PwmController::setRaw(uint8_t duty) {
    if (setPwm_) {
        setPwm_(duty);
    }
    currentRawDuty_ = duty;
}

void PwmController::setPercent(uint8_t percent) {
    if (percent > 100) {
        percent = 100;
    } 

    float scaled = (percent / 100.0f) * static_cast<float>(255);
    uint8_t duty = static_cast<uint8_t>(scaled + 0.5f);
    setRaw(duty);
    currentPercent_ = percent;
}

void PwmController::increaseByPercent(uint8_t percent) {
    uint8_t newValue = currentPercent_ + percent;
    if (newValue > 100) {
        newValue = 100;
    }
    setPercent(newValue);
}

void PwmController::decreaseByPercent(uint8_t percent) {
    uint8_t newValue = (currentPercent_ > percent) ? currentPercent_ - percent : 0;
    setPercent(newValue);
}

void PwmController::off() {
    setPercent(0);
}

void PwmController::fullOn() {
    setPercent(100);
}

void PwmController::resetBreathe() {
    breatheOn_ = false;
}

void PwmController::setBreathe() {
    breatheOn_ = true;
}

void PwmController::breatheTick() {
    if (!breatheOn_) return;
        

    if (currentPercent_ >= 100.0f) {
        breathingUp_ = false;
    } else if (currentPercent_ <= 0.0f) {
        breathingUp_ = true;
    }

    if (breathingUp_) {
        increaseByPercent(1.0f);
    } else {
        decreaseByPercent(1.0f);
    }
}

