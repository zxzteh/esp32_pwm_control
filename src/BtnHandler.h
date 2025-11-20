#pragma once

#include <stdint.h>
#include "PwmController.h"

class BtnHandler {
public:
    explicit BtnHandler(PwmController* pwm);
    void tick(uint32_t nowMs, bool pressed);

private:
    static constexpr uint32_t DEBOUNCE_MS = 20;
    static constexpr uint32_t LONG_PRESS_MS = 800;
    static constexpr uint32_t DOUBLE_CLICK_GAP_MS = 200;

    PwmController* m_pwm;

    enum class State {
        Idle,
        Debouncing,
        Pressed,
        ClickReleased,
        LongPress
    };

    State state_ = State::Idle;

    bool wasPressed_ = false;  //  at last tick button was pressed or not

    uint32_t lastChangeMs_ = 0;  //  when btn change press or not state (before debounce) 
    uint32_t pressStartMs_ = 0;  //  when btn really pressed after debounce
    uint32_t lastReleaseMs_ = 0;  //  when btn released

    uint8_t  clickCount_ = 0;

    //  Helpers
    void updateDebounce(uint32_t nowMs, bool pressed);
    bool isStable(uint32_t nowMs) const;
    bool isLongPress(uint32_t nowMs) const;
    bool isDoubleClickGapExpired(uint32_t nowMs) const;
    void startNewPress(uint32_t nowMs);
    void finishClickSequence();

    //  state handlers
    void handleIdle(uint32_t nowMs, bool pressed);
    void handlePressed(uint32_t nowMs, bool pressed);
    void handleClickReleased(uint32_t nowMs, bool pressed);
    void handleLongPress(uint32_t nowMs, bool pressed);
};