#pragma once

#include <Arduino.h>

namespace hw {

//  HW init
void init();

//  Btn
bool isButtonPressed();

//  Pwm
void setPwm(uint8_t percent);

//  Uart
size_t uartAvailable();
bool uartReadByte(uint8_t &byte);
void uartWrite(const char* string);

} // namespace hw

