#include "mergesort.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int int_gt_comparator(const void *a, const void *b)
{
    return *(int *) a - *(int *) b;  // Сортировка массива по убыванию.
}

int char_gt_comparator(const void *a, const void *b)
{
    return *(char *) a - *(char *) b;  // Сортировка массива по убыванию.
}

int string_gt_comparator(const void *a, const void *b)
{
    return strcmp(*(char **) a, *(char **) b);  // Сортировка массива по убыванию.
}


int main(int argc, char *argv[])
{
    if (argc < 3)//no agrs -> nothing to do
    {
        return EXIT_SUCCESS;
    }
    char *type = argv[1];
    if (strcmp(type, "int") == 0)
    {
        int *arr = malloc(sizeof(int) * (argc - 2));
        assert(arr != NULL);
        size_t n = (size_t)(argc - 2);
        for (size_t i = 0; i < n; ++i)
        {
            arr[i] = atoi(argv[i + 2]);
        }

        my_mergesort(arr, n, sizeof(int), int_gt_comparator);

        for (size_t i = 0; i < n - 1; ++i)
        {
            printf("%d ", arr[i]);
        }
        printf("%d\n", arr[n - 1]);

        free(arr);
    } else if (strcmp(type, "char") == 0)
    {
        char *arr = malloc(sizeof(char) * (argc - 2));
        assert(arr != NULL);
        size_t n = (size_t)(argc - 2);
        for (size_t i = 0; i < n; ++i)
        {
            arr[i] = argv[i + 2][0];
        }

        my_mergesort(arr, n, sizeof(char), char_gt_comparator);

        for (size_t i = 0; i < n - 1; ++i)
        {
            printf("%c ", arr[i]);
        }
        printf("%c\n", arr[n - 1]);

        free(arr);
    } else if (strcmp(type, "str") == 0)
    {
        char **arr = &argv[2];
        size_t n = (size_t)(argc - 2);

        my_mergesort(arr, n, sizeof(char*), string_gt_comparator);

        for (size_t i = 0; i < n - 1; ++i)
        {
            printf("%s ", arr[i]);
        }
        printf("%s\n", arr[n - 1]);
    }

    return EXIT_SUCCESS;
}