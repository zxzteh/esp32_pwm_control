#include "BtnHandler.h"

BtnHandler::BtnHandler(PwmController* pwm)
    : m_pwm(pwm) {}

void BtnHandler::tick(uint32_t nowMs, bool pressed) {
    updateDebounce(nowMs, pressed);
    if (!isStable(nowMs)) {
        return;
    }

    const bool stablePressed = wasPressed_;

    switch (state_) {
    case State::Idle:
        handleIdle(nowMs, stablePressed);
        break;

    case State::Pressed:
        handlePressed(nowMs, stablePressed);
        break;

    case State::ClickReleased:
        handleClickReleased(nowMs, stablePressed);
        break;

    case State::LongPress:
        handleLongPress(nowMs, stablePressed);
        break;
    }
}

void BtnHandler::handleIdle(uint32_t nowMs, bool pressed)
{
    if (pressed) {
        startNewPress(nowMs);
    }
}

void BtnHandler::handlePressed(uint32_t nowMs, bool pressed)
{
    if (!pressed) {
        //  Btn release case
        if (!isLongPress(nowMs)) {
            clickCount_++;
            lastReleaseMs_ = nowMs;
            state_ = State::ClickReleased;
        }
        return;
    }

    //  Btn still pressed
    if (isLongPress(nowMs)) {
        clickCount_ = 0;
        state_      = State::LongPress;
    }
}

void BtnHandler::handleClickReleased(uint32_t nowMs, bool pressed) {
    if (pressed) {
        //  New click in a row
        startNewPress(nowMs);
        return;
    }

    //  Double click time has passed
    if (isDoubleClickGapExpired(nowMs)) {
        finishClickSequence();
    }
}

void BtnHandler::handleLongPress(uint32_t nowMs, bool pressed) {
    (void) nowMs;
    m_pwm->setBreathe();
    if (!pressed) {
        m_pwm->resetBreathe();
        state_ = State::Idle;
    }
}

void BtnHandler::updateDebounce(uint32_t nowMs, bool pressed) {
    if (pressed == wasPressed_) {
        return;
    }

    wasPressed_   = pressed;
    lastChangeMs_ = nowMs;
}

bool BtnHandler::isStable(uint32_t nowMs) const {
    return (nowMs - lastChangeMs_) >= DEBOUNCE_MS;
}

bool BtnHandler::isLongPress(uint32_t nowMs) const {
    return (nowMs - pressStartMs_) >= LONG_PRESS_MS;
}

bool BtnHandler::isDoubleClickGapExpired(uint32_t nowMs) const {
    return (nowMs - lastReleaseMs_) >= DOUBLE_CLICK_GAP_MS;
}

void BtnHandler::startNewPress(uint32_t nowMs) {
    state_       = State::Pressed;
    pressStartMs_ = nowMs;
}

void BtnHandler::finishClickSequence() {
    if (clickCount_ == 1) {
        if (m_pwm) m_pwm->increaseByPercent(10);
    } else if (clickCount_ >= 2) {
        if (m_pwm) m_pwm->off();
    }

    clickCount_ = 0;
    state_      = State::Idle;
}
