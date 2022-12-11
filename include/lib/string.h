#pragma once

#include <stdbool.h>

void str_insert(char *string, int index, char c);
void str_remove(char *string, int index);
int strlen(const char *string);
void strcpy(char *dst, const char *src);
void strcat(char *dst, const char *src);
char strcmp(const char *str1, const char *str2);

void memcpy(void *dst, const void *src, int count);
void memset(void *dst, int value, int count);
void memswap(void *addr1, void *addr2, int count);
char memcmp(const void *addr1, const void *addr2, int count, bool little_endian);
