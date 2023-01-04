#pragma once

#include <stdarg.h>
#include <stdint.h>

void sprintf(char *dst, const char *fmt, ...);
void vsprintf(char *dst, const char *fmt, va_list argp);
void sprint_signed(char *dst, int64_t number, int base);
void sprint_unsigned(char *dst, uint64_t number, int base);
void left_padding(char *str, int pad_to_length, char pad_char);
void right_padding(char *str, int pad_to_length, char pad_char);
