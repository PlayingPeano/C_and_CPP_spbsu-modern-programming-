#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "str.h"

void test_my_strcpy()
{
    char src[] = "abcd";
    char dst[5];
    my_strcpy(dst, src);

    assert(strcmp(src, dst) == 0);
}

void test_my_strcat()
{

    char src[] = "mam";
    char dst[7] = "pap";
    char res[] = "papmam";
    my_strcat(dst, src);

    assert(strcmp(res, dst) == 0);
}

void test_my_strcmp()
{
    {
        char string1[] = "mam";
        char string2[7] = "mam";
        int k = my_strcmp(string1, string2);
        assert(k == 0);
    }
    {
        char string1[] = "mam";
        char string2[7] = "pap";
        int k = my_strcmp(string1, string2);
        assert(k < 0);
    }
}

void test_my_strlen()
{
    {
        char string[] = "mam";
        int k = my_strlen(string);

        assert(k == 3);
    }
    {
        char string[7] = "pap";
        int k = my_strlen(string);

        assert(k == 3);
    }
}