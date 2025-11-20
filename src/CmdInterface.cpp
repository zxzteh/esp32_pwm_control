#include "CmdInterface.h"

CmdInterface::CmdInterface(WriteFn writeFn, SetPwmFn setPwmFn, GetPwmFn getPwmFn)
    : write_cb(writeFn),
      setPwm_cb(setPwmFn),
      getPwm_cb(getPwmFn),
      lineLen_(0)
{}

void CmdInterface::send(const char* string) {
    if (write_cb) {
        write_cb(string);
    }
}

//  Cmp two strings, ignore case with len limit
int CmdInterface::strnicmpLocal(const char* str_a, const char* str_b, size_t n) {
    for (size_t i = 0; i < n; i++) {
        char char_a = static_cast<char>(toupper(static_cast<unsigned char>(str_a[i])));
        char char_b = static_cast<char>(toupper(static_cast<unsigned char>(str_b[i])));
        if (char_a != char_b) {
            return static_cast<int>(static_cast<unsigned char>(char_a) - static_cast<unsigned char>(char_b));
        }
        if (char_a == '\0') {
            return 0;
        }
    }
    return 0;
}


//  Ignore spaces around valid command
char* CmdInterface::trimSpaces(char* p_string) {
    //  From left
    while (*p_string && isspace(static_cast<unsigned char>(*p_string))) {
        p_string++;
    }
    
    //  From Right 
    char* end = p_string + strlen(p_string);
    while (end > p_string && isspace(static_cast<unsigned char>(*(end - 1)))) {
        end--;
    }
    *end = '\0';
    return p_string;
}

void CmdInterface::processLine(char* line) {
    char *p_char = trimSpaces(line);
    
    //  Empty line case
    if (*p_char == '\0') {
        send("empty");
        return;
    }

    //  SET PWM X
    if (strnicmpLocal(p_char, "SET", 3) == 0 && isspace(static_cast<unsigned char>(p_char[3]))) {
        p_char += 3;
        p_char = trimSpaces(p_char);

        if (strnicmpLocal(p_char, "PWM", 3) == 0 && isspace(static_cast<unsigned char>(p_char[3]))) {
            p_char += 3;
            p_char = trimSpaces(p_char);

            if (!isdigit(static_cast<unsigned char>(*p_char))) {
                return;
            }

            //  Collect number
            int value = 0;
            while (isdigit(static_cast<unsigned char>(*p_char))) {
                value = value * 10 + (*p_char - '0');
                p_char++;
            }

            p_char = trimSpaces(p_char);

            //  Garbage after cmd
            if (*p_char != '\0') {
                return;
            }

            //  Invalid argument
            if (value < 0 || value > 100) {
                return;
            }

            //  All ok, set pwm
            if (setPwm_cb) {
                setPwm_cb(static_cast<uint8_t>(value));
            }
            send("OK\r\n");
            return;
        }
        return;
    }

    //  GET PWM
    if (strnicmpLocal(p_char, "GET", 3) == 0 && isspace(static_cast<unsigned char>(p_char[3]))) {
        p_char += 3;
        p_char = trimSpaces(p_char);

        if (strnicmpLocal(p_char, "PWM", 3) == 0) {
            p_char += 3;
            p_char = trimSpaces(p_char);

            //  We expecting null, not arguments
            if (*p_char != '\0') {
                return;
            }

            if (getPwm_cb) {
                uint8_t val = getPwm_cb();
                char buf[16];
                int n = snprintf(buf, sizeof(buf), "%u\r\n", static_cast<unsigned>(val));
                if (n > 0) {
                    send(buf);
                }
            }
            return;
        }
        return;
    }
}

void CmdInterface::processChar(uint8_t ch) {

    //  End of the line case 
    if (ch == '\r' || ch == '\n') {
        if (lineLen_ > 0) {
            lineBuf_[lineLen_] = '\0';
            processLine(lineBuf_);
            lineLen_ = 0;
        }
        return;
    }

    //  Add to line
    if (lineLen_ < (LINE_BUF_SIZE - 1)) {
        lineBuf_[lineLen_] = static_cast<char>(ch);
        lineLen_++;
    }
}
