#include "mergesort.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int int_gt_comparator(const void *a, const void *b)
{
    return *(int *) a - *(int *) b;
}

int char_gt_comparator(const void *a, const void *b)
{
    return *(char *) a - *(char *) b;
}

int string_gt_comparator(const void *a, const void *b)
{
    return strcmp(*(char **) a, *(char **) b);
}


int main(int argc, char *argv[])
{
    if (argc < 3)//no agrs -> nothing to do
    {
        printf("\n");
        return EXIT_SUCCESS;
    }

    char *type = argv[1];
    size_t n = (size_t)(argc - 2);
    size_t sizeOfType;
    int (*comparator)(const void *, const void *);
    void *arr;

    if (strcmp(type, "int") == 0)
    {
        arr = (int *) malloc(sizeof(int) * (argc - 2));
        assert(arr != NULL);
        for (size_t i = 0; i < n; ++i)
        {
            *((int *) arr + i) = atoi(argv[i + 2]);
        }
        comparator = int_gt_comparator;
        sizeOfType = sizeof(int);

    } else if (strcmp(type, "char") == 0)
    {
        arr = (char *) malloc(sizeof(char) * (argc - 2));
        assert(arr != NULL);
        for (size_t i = 0; i < n; ++i)
        {
            *((char *) arr + i) = argv[i + 2][0];
        }
        comparator = char_gt_comparator;
        sizeOfType = sizeof(char);

    } else if (strcmp(type, "str") == 0)
    {
        comparator = string_gt_comparator;
        sizeOfType = sizeof(char *);
        arr = &argv[2];
    } else
    {
        return EXIT_SUCCESS;
    }

    if (my_mergesort(arr, n, sizeOfType, comparator))
    {
        printf("Error: memory allocation failed.\n");
        free(arr);
        return 1;
    }

    for (size_t i = 0; i < n - 1; ++i)
    {
        if (strcmp(type, "int") == 0)
        {
            printf("%d ", *((int *) arr + i));
        } else if (strcmp(type, "char") == 0)
        {
            printf("%c ", *((char *) arr + i));
        } else if (strcmp(type, "str") == 0)
        {
            printf("%s ", *((char **) arr + i));
        }
    }

    if (strcmp(type, "int") == 0)
    {
        printf("%d", *((int *) arr + n - 1));
    } else if (strcmp(type, "char") == 0)
    {
        printf("%c", *((char *) arr + n - 1));
    } else if (strcmp(type, "str") == 0)
    {
        printf("%s", *((char **) arr + n - 1));
    }

    printf("\n");
    if (strcmp(type, "str") != 0)
    { free(arr); }
    return EXIT_SUCCESS;
}