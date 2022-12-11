#pragma once

#include <stdarg.h>

void sprintf(char *dst, const char *fmt, ...);
void vsprintf(char *dst, const char *fmt, va_list argp);
void sprint_signed(char *dst, int number, int base);
void sprint_unsigned(char *dst, unsigned int number, int base);
void left_padding(char *str, int pad_to_length, char pad_char);
void right_padding(char *str, int pad_to_length, char pad_char);
