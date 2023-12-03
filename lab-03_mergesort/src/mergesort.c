#include "mergesort.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void merge(void *array, size_t left, size_t mid, size_t right, size_t element_size,
           int (*comparator)(const void *, const void *))
{
    size_t left_arr_size = mid - left + 1;
    size_t right_arr_size = right - mid;

    void *left_arr = malloc(left_arr_size * element_size);
    assert(left_arr != NULL);
    void *right_arr = malloc(right_arr_size * element_size);
    assert(right_arr != NULL);

    memcpy(left_arr, (char *) array + left * element_size, left_arr_size * element_size);
    memcpy(right_arr, (char *) array + (mid + 1) * element_size, right_arr_size * element_size);

    size_t i = 0;
    size_t j = 0;
    size_t k = left;

    while (i < left_arr_size && j < right_arr_size)
    {
        if (comparator((char *) left_arr + i * element_size, (char *) right_arr + j * element_size) <= 0)
        {
            memcpy((char *) array + k * element_size, (char *) left_arr + i * element_size, element_size);
            ++i;
        } else
        {
            memcpy((char *) array + k * element_size, (char *) right_arr + j * element_size, element_size);
            ++j;
        }
        ++k;
    }

    while (i < left_arr_size)
    {
        memcpy((char *) array + k * element_size, (char *) left_arr + i * element_size, element_size);
        ++i;
        ++k;
    }

    while (j < right_arr_size)
    {
        memcpy((char *) array + k * element_size, (char *) right_arr + j * element_size, element_size);
        ++j;
        ++k;
    }

    free(left_arr);
    free(right_arr);
}

void mergesortFunc(void *array, size_t left, size_t right, size_t element_size,
                   int (*comparator)(const void *, const void *))
{
    if (left >= right)
    {
        return;
    }
    size_t mid = left + (right - left) / 2;

    mergesortFunc(array, left, mid, element_size, comparator);
    mergesortFunc(array, mid + 1, right, element_size, comparator);

    merge(array, left, mid, right, element_size, comparator);
}

void my_mergesort(
        void *array,
        size_t elements, size_t element_size,
        int (*comparator)(const void *, const void *)
)
{
    mergesortFunc(array, 0, elements - 1, element_size, comparator);
}
