#include "lib/string.h"
#include <stdint.h>

void str_insert(char *string, int index, char c)
{
    for (int i = strlen(string) + 1; i > index; i--)
    {
        string[i] = string[i - 1];
    }
    string[index] = c;
}

void str_remove(char *string, int index)
{
    for (int i = index; i <= strlen(string); i++)
    {
        string[i] = string[i + 1];
    }
}

int strlen(const char *string)
{
    int len = 0;
    while (string[len])
        len++;
    return len;
}

void strcpy(char *dst, const char *src)
{
    int length = strlen(src);
    for (int i = 0; i <= length; i++)
    {
        dst[i] = src[i];
    }
}

char strcmp(const char *str1, const char *str2)
{
    for (; *str1 != '\0' && *str2 != '\0'; str1++, str2++)
    {
        if ((uint8_t)(*str1) < (uint8_t)(*str2))
            return -1;
        if ((uint8_t)(*str1) > (uint8_t)(*str2))
            return 1;
    }
    if (*str1 == '\0' && *str2 != '\0')
        return -1;
    if (*str1 != '\0' && *str2 == '\0')
        return 1;
    return 0;
}

void strcat(char *dst, const char *src)
{
    strcpy(dst + strlen(dst), src);
}

void memcpy(void *dst, const void *src, int count)
{
    for (int i = 0; i < count; i++)
    {
        ((char *)dst)[i] = ((char *)src)[i];
    }
}

void memset(void *dst, int value, int count)
{
    for (int i = 0; i < count; i++)
    {
        ((char *)dst)[i] = value;
    }
}

void memswap(void *addr1, void *addr2, int count)
{
    for (int i = 0; i < count; i++)
    {
        char tmp = ((char *)addr1)[i];
        ((char *)addr1)[i] = ((char *)addr2)[i];
        ((char *)addr2)[i] = tmp;
    }
}

char memcmp(const void *addr1, const void *addr2, int count, bool little_endian)
{
    if (little_endian)
    {
        for (int i = count - 1; i >= 0; i--)
        {
            if (((uint8_t *)addr1)[i] < ((uint8_t *)addr2)[i])
                return -1;
            if (((uint8_t *)addr1)[i] > ((uint8_t *)addr2)[i])
                return 1;
        }
    }
    else
    {
        for (int i = 0; i < count; i++)
        {
            if (((uint8_t *)addr1)[i] < ((uint8_t *)addr2)[i])
                return -1;
            if (((uint8_t *)addr1)[i] > ((uint8_t *)addr2)[i])
                return 1;
        }
    }
    return 0;
}
