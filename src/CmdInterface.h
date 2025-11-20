#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

class CmdInterface {
public:
    using WriteFn = void (*)(const char *data);
    using SetPwmFn = void (*)(uint8_t value);
    using GetPwmFn = uint8_t (*)(void);

    CmdInterface(WriteFn writeFn, SetPwmFn setPwmFn, GetPwmFn getPwmFn);

    void processChar(uint8_t ch);

private:
    static constexpr size_t LINE_BUF_SIZE = 64;
    char  lineBuf_[LINE_BUF_SIZE];
    size_t lineLen_ = 0;

    WriteFn write_cb;
    SetPwmFn setPwm_cb;
    GetPwmFn getPwm_cb;

    void send(const char* string);
    void processLine(char* line);

    static char* trimSpaces(char* s);
    static int strnicmpLocal(const char* a, const char* b, size_t n);
};

