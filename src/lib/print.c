#include "lib/print.h"

#include "lib/string.h"
#include "lib/logging.h"

const static char *HEX_DIGITS = "0123456789abcdef";

static char *get_nonzero_start(const char *str);

char *get_nonzero_start(const char *str)
{
    while (*str == '0' && str[1] != '\0')
    {
        str++;
    }
    return (char *)str;
}

void sprint_unsigned(char *dst, uint64_t number, int base)
{
    if (base > 16)
    {
        dst[0] = '\0';
        return;
    }
    char tmp_str[20]; // max str length is 16 (64 bits = 8 bytes = 16 hexdigits)
    tmp_str[19] = '\0';

    for (int index = 18; index >= 0; index--)
    {
        tmp_str[index] = HEX_DIGITS[number % base];
        number /= base;
    }
    strcpy(dst, get_nonzero_start(tmp_str));
}

void sprint_signed(char *dst, int64_t number, int base)
{
    if (number < 0)
    {
        sprint_unsigned(dst + 1, -number, base);
        dst[0] = '-';
    }
    else
    {
        sprint_unsigned(dst, number, base);
    }
}

void right_padding(char *str, int pad_to_length, char pad_char)
{
    int length = strlen(str);
    if (length >= pad_to_length)
        return;
    int offset = pad_to_length - length;
    for (int i = 0; i < offset; i++)
    {
        str[length + i] = pad_char;
    }
    str[length + offset] = '\0';
}

void left_padding(char *str, int pad_to_length, char pad_char)
{
    int length = strlen(str);
    if (length >= pad_to_length)
        return;
    int offset = pad_to_length - length;
    for (int i = length + 1; i >= 0; i--)
    {
        str[i + offset] = str[i];
    }
    for (int i = 0; i < offset; i++)
    {
        str[i] = pad_char;
    }
}

void sprintf(char *dst, const char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    vsprintf(dst, fmt, argp);
    va_end(argp);
}

void vsprintf(char *dst, const char *fmt, va_list argp)
{
    int64_t signed_number;
    uint64_t unsigned_number;
    char c, *s;

    for (; *fmt != '\0'; fmt++)
    {
        if (*fmt == '%')
        {
            fmt++;
            switch (*fmt)
            {
            case '%':
                *dst = *fmt;
                dst++;
                break;
            case 'c':
                c = (char)va_arg(argp, int);
                *dst = c;
                dst++;
                break;
            case 's':
                s = va_arg(argp, char *);
                strcpy(dst, s);
                dst += strlen(s);
                break;
            case 'x':
                signed_number = va_arg(argp, int64_t);
                sprint_signed(dst, signed_number, 16);
                dst += strlen(dst);
                break;
            case 'd':
                signed_number = va_arg(argp, int64_t);
                sprint_signed(dst, signed_number, 10);
                dst += strlen(dst);
                break;
            case 'b':
                signed_number = va_arg(argp, int64_t);
                sprint_signed(dst, signed_number, 2);
                dst += strlen(dst);
                break;
            case 'o':
                signed_number = va_arg(argp, int64_t);
                sprint_signed(dst, signed_number, 8);
                dst += strlen(dst);
                break;
            case 'p':
                unsigned_number = va_arg(argp, uint64_t);
                sprint_unsigned(dst, unsigned_number, 16);
                left_padding(dst, 16, '0');
                dst += strlen(dst);
                break;
            default:
                ERROR("unknown format");
            }
        }
        else
        {
            *dst = *fmt;
            dst++;
        }
    }
    *dst = '\0';
}
