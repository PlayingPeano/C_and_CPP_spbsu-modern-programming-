#ifndef LAB_03_MERGESORT_MERGESORT_H
#define LAB_03_MERGESORT_MERGESORT_H
#include <stdlib.h>

int my_mergesort(
        void *array,
        size_t elements, size_t element_size,
        int (*comparator)(const void *, const void *)
);

#endif //LAB_03_MERGESORT_MERGESORT_H
