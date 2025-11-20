#include "hardware.h"

namespace hw {

constexpr uint8_t LED_PIN    = 4;
constexpr uint8_t BUTTON_PIN = 45;

constexpr uint8_t  PWM_CHANNEL      = 0;
constexpr uint32_t PWM_FREQUENCY    = 5000;
constexpr uint8_t  PWM_RESOLUTION   = 8;

static volatile bool buttonPressed_ = false;

//  Buttons 

void IRAM_ATTR buttonIsr() {
    int level = digitalRead(BUTTON_PIN);
    buttonPressed_ = (level == LOW);
}

static void initButton() {
    pinMode(BUTTON_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonIsr, CHANGE);
}

bool isButtonPressed() {
    int level = digitalRead(BUTTON_PIN);
    return (level == HIGH);
}

//  PWM 

void setPwm(uint8_t value) {
    ledcWrite(PWM_CHANNEL, value);
}

static void initPwm() {
    ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(LED_PIN, PWM_CHANNEL);
    setPwm(0);
}

//  UART

static void initUart() {
    Serial.begin(115200);
}

bool uartReadByte(uint8_t &byte) {
    if (Serial.available()) {
        int v = Serial.read();
        if (v >= 0) {
            byte = static_cast<uint8_t>(v);
            return true;
        }
    }
    return false;
}

void uartWrite(const char* string) {
    if (string) {
        Serial.print(string);
    }
}

//  All init

void init() {
    initPwm();
    initButton();
    initUart();
}

} // namespace hw
