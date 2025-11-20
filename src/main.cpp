#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "hardware.h"
#include "RingBuffer.h"
#include "PwmController.h"
#include "CmdInterface.h"
#include "BtnHandler.h"

static RingBuffer serialBuffer;
static PwmController pwmController(hw::setPwm);
static BtnHandler btnHandler(&pwmController);

//  Using lambda, but also can handle pointer to the class
static CmdInterface cmdInterface(hw::uartWrite,
    [](uint8_t value) {
        pwmController.setPercent(value);
    },
    []() {
        return pwmController.getPercent();
    }
);

//  Tasks
void serialTask(void *pv) {
    (void)pv;
    while(1) {
        uint8_t byte;
        while (hw::uartReadByte(byte)) {
            serialBuffer.push(byte);
        }

        while (serialBuffer.pop(byte)) {
            cmdInterface.processChar((byte));
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

//  Only for breathe mode
void ledTask(void *pv) {
    (void)pv;
    while(1) {
        pwmController.breatheTick();
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void mainTask(void *pv) {
    (void)pv;
    while(1) {
        bool btn_is_pressed = hw::isButtonPressed();
        uint32_t nowMs = xTaskGetTickCount() * portTICK_PERIOD_MS;
        btnHandler.tick(nowMs ,btn_is_pressed);
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void setup() {
    hw::init();
    xTaskCreate(ledTask, "ledTask",  4096, nullptr, 3, nullptr);
    xTaskCreate(serialTask, "uartTask", 4096, nullptr, 2, nullptr);
    xTaskCreate(mainTask, "mainTask", 4096, nullptr, 1, nullptr);
}

void loop() {
    vTaskDelay(pdMS_TO_TICKS(1000));  //  do nothing in main
}
