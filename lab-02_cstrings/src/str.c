#include "str.h"

char *my_strcpy(char *dst, const char *src)
{
    while (*src)
    {
        *dst = *src;
        ++src;
        ++dst;
    }
    *dst = '\0';
    return dst;
}

char *my_strcat(char *dst, const char *src)
{
    char *dstBegin = dst;
    while (*dst)
    {
        ++dst;
    }
    while (*src)
    {
        *dst = *src;
        ++dst;
        ++src;
    }
    *dst = '\0';
    return dstBegin;
}

int my_strcmp(const char *string1, const char *string2)
{
    while (*string1 && *string2)
    {
        if (*string1 != *string2)
        {
            return *string1 - *string2;
        }
        ++string1;
        ++string2;
    }
    return *string1 - *string2;
}

int my_strlen(const char *string)
{
    int length = 0;
    while(*string)
    {
        ++string;
        ++length;
    }
    return length;
}