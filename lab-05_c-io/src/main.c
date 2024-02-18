#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "clist.h"
#include "point_list.h"

const unsigned char FULL_BYTE_MASK = 0xFF;
const int SIZE_OF_ONE_BYTE = 8;
const int SIZE_OF_TWO_BYTES = 16;
const size_t SIZE_OF_ARRAY_WITH_BYTES = 6;
const int NUMBER_OF_COORDINATES_IN_ONE_POINT = 2;
const int MINIMUM_NUMBER_OF_ARGS = 2;

void print_point(intrusive_node *node, void *data)
{
    point *p = container_of(node, point, node);
    printf(data, p->x, p->y);
}

void save_to_text_file(intrusive_list *list, const char *filename)
{
    FILE *file = fopen(filename, "w");
    assert(file != NULL);

    apply(list, print_point, (void *) "%d %d\n");

    fclose(file);
}

void save_to_binary_file(intrusive_list *list, const char *filename)
{
    FILE *file = fopen(filename, "wb");
    assert(file != NULL);
    if (!list || !list->head)
    {
        return;
    }

    intrusive_node *current = list->head;

    while (current->next != NULL)//taking a pointer to the tail
    {
        current = current->next;
    }

    while (current != NULL)
    {
        point *p = container_of(current, point, node);
        unsigned char byte1 = (p->x) & FULL_BYTE_MASK;
        unsigned char byte2 = (p->x >> SIZE_OF_ONE_BYTE) & FULL_BYTE_MASK;
        unsigned char byte3 = (p->x >> SIZE_OF_TWO_BYTES) & FULL_BYTE_MASK;
        unsigned char byte4 = (p->y) & FULL_BYTE_MASK;
        unsigned char byte5 = (p->y >> SIZE_OF_ONE_BYTE) & FULL_BYTE_MASK;
        unsigned char byte6 = (p->y >> SIZE_OF_TWO_BYTES) & FULL_BYTE_MASK;
        unsigned char bytes[] = {byte1, byte2, byte3, byte4, byte5, byte6};
        assert(fwrite(bytes, sizeof(unsigned char),
                      SIZE_OF_ARRAY_WITH_BYTES, file) == SIZE_OF_ARRAY_WITH_BYTES);
        current = current->prev;
    }

    fclose(file);
}

void read_from_text_file(intrusive_list *list, const char *filename)
{
    FILE *file = fopen(filename, "r");
    assert(file != NULL);

    int x, y;
    while (fscanf(file, "%d %d", &x, &y) == NUMBER_OF_COORDINATES_IN_ONE_POINT)
    {
        point *p = malloc(sizeof(point));
        p->x = x;
        p->y = y;
        add_node_back(list, &p->node);
    }

    fclose(file);
}

void read_from_binary_file(intrusive_list *list, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    assert(file != NULL);

    unsigned char bytes[SIZE_OF_ARRAY_WITH_BYTES];
    while (fread(&bytes, sizeof(unsigned char),
                 SIZE_OF_ARRAY_WITH_BYTES, file) == SIZE_OF_ARRAY_WITH_BYTES)
    {
        point *p = malloc(sizeof(point));
        p->x = bytes[0] + (bytes[1] << SIZE_OF_ONE_BYTE) + (bytes[2] << SIZE_OF_TWO_BYTES);
        p->y = bytes[3] + (bytes[4] << SIZE_OF_ONE_BYTE) + (bytes[5] << SIZE_OF_TWO_BYTES);
        add_node_back(list, &p->node);
    }

    fclose(file);
}

void count_points(intrusive_node *node, void *data)
{
    (void) node;
    (*(int *) data)++;
}

int main(int argc, char *argv[])
{
//    intrusive_list list;
//    init_list(&list);
//    read_from_text_file(&list, "/Users/mi/Documents/GitHub/C/labs-isaenkov/lab-05_c-io/bin.txt");
//    save_to_binary_file(&list, "/Users/mi/Documents/GitHub/C/labs-isaenkov/lab-05_c-io/out.bin");
//    save_to_text_file(&list, "/Users/mi/Documents/GitHub/C/labs-isaenkov/lab-05_c-io/out.txt");

    if (argc < MINIMUM_NUMBER_OF_ARGS)
    {
        return 1;
    }

    intrusive_list list;
    init_list(&list);

    const char *file_type = argv[1];
    const char *infile = argv[2];

    if (strcmp(file_type, "loadtext") == 0)
    {
        read_from_text_file(&list, infile);
    } else if (strcmp(file_type, "loadbin") == 0)
    {
        read_from_binary_file(&list, infile);
    } else
    {
        return 1;
    }

    for (int i = 3; i < argc; ++i)
    {
        const char *action = argv[i];
        if (strcmp(action, "savetext") == 0)
        {
            assert(i + 1 < argc);
            save_to_text_file(&list, argv[i + 1]);
            i++;
        } else if (strcmp(action, "savebin") == 0)
        {
            assert(i + 1 < argc);
            save_to_binary_file(&list, argv[i + 1]);
            i++;
        } else if (strcmp(action, "print") == 0)
        {
            assert(i + 1 < argc);
            apply(&list, print_point, (void *) argv[i + 1]);
            printf("\n");
            i++;

        } else if (strcmp(action, "count") == 0)
        {
            int count = 0;
            apply(&list, count_points, &count);
            printf("%d\n", count);
        } else
        {
            return 1;
        }
    }

    remove_all_points(&list);

    return 0;
}
