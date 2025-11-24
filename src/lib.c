#include <stdint.h>
#include <stdarg.h>
#include "vmem.h"

char dec_to_hex_mapping[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

static void putchar(char c) {
    static uint8_t *vidptr = (uint8_t*)0xb8000;
    static uint32_t pos = 0;
    if (c == '\n') {
        pos = ((pos / 160) + 1) * 160;
    } else {
        vidptr[pos++] = c;
        vidptr[pos++] = 0x07;
    }
}

void 
decimalToHex(uint32_t dec){
    char hex[12]; //static buffer
    int idx = 0;
    while(dec > 0)
    {
        hex[idx++] = dec_to_hex_mapping[dec % 16];
        dec /= 16;
    }
    putchar('0');
    putchar('x');
    while (idx--) {
        putchar(hex[idx]);
    }
    return;
}

void 
decimalToBinary(uint32_t dec){
    char bin[48]; //static buffer
    int idx = 0;
    while(dec > 0)
    {
        bin[idx++] = (dec % 2)? '1' : '0';
        dec /= 2;
    }
    putchar('0');
    putchar('b');
    while (idx--) {
        putchar(bin[idx]);
    }
    return;
}

void printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    for (const char *p = fmt; *p; ++p) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }
        ++p;  // skip '%'

        switch (*p) {
        case 'd': {
            int val = va_arg(ap, int);
            // handle zero explicitly
            if (val == 0) {
                putchar('0');
                break;
            }
            // handle negative
            if (val < 0) {
                putchar('-');
                val = -val;
            }
            // buffer for digits (max 10 digits for 32-bit)
            char buf[12];
            int idx = 0;
            while (val > 0) {
                buf[idx++] = '0' + (val % 10);
                val /= 10;
            }
            // emit in reverse
            while (idx--) {
                putchar(buf[idx]);
            }
            break;
        }
        case 'c': {
            int ch = va_arg(ap, int);
            putchar((char)ch);
            break;
        }
        case 'x': {
            int dec = va_arg(ap, int);
            decimalToHex((uint32_t)dec);
            break;
        }
        case 'b': {
            int dec = va_arg(ap, int);
            decimalToBinary((uint32_t)dec);
            break;
        }
        case '%':
            putchar('%');
            break;
        default:
            //just print it literally
            putchar('%');
            putchar(*p);
            break;
        }
    }
    va_end(ap);
}
