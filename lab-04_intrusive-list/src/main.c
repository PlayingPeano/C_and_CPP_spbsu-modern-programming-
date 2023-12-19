#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "clist.h"

typedef struct point point;
struct point
{
    int x, y;
    intrusive_node node;
};

void add_point(intrusive_list *list, int x, int y)
{
    point *new_point = (point *) malloc(sizeof(point));
    assert(new_point != NULL);
    new_point->x = x;
    new_point->y = y;
    add_node(list, &new_point->node);
}

void remove_point(intrusive_list *list, int x, int y)
{ // removes all (x, y) pairs
    intrusive_node *current = list->head;

    while (current)
    {
        point *current_point = container_of(current, point, node);
        intrusive_node *next = current->next;

        if (current_point->x == x && current_point->y == y)
        {
            remove_node(list, current);
            free(current_point);
        }
        current = next;
    }
}

void show_all_points(intrusive_list *list)
{
    intrusive_node *current = list->head;

    while (current)
    {
        point *current_point = container_of(current, point, node);

        printf("(%d %d)", current_point->x, current_point->y);
        if (current->next)
        {
            printf("%c", ' ');
        }
        current = current->next;
    }
    printf("%c", '\n');
}

void remove_all_points(intrusive_list *list)
{
    intrusive_node *current = list->head;

    while (current)
    {
        point *current_point = container_of(current, point, node);
        intrusive_node *next = current->next;

        remove_node(list, current);
        free(current_point);

        current = next;
    }
}

int main()
{

    intrusive_list l;
    init_list(&l);

//    add_point(&l, 10, 10);
//    add_point(&l, 20, 20);
//
//    show_all_points(&l);

    char input[240];
    while (1)
    {
        scanf("%239s", input);
        if (strcmp(input, "add") == 0)
        {
            int x, y;
            scanf("%d %d", &x, &y);
            add_point(&l, x, y);
        } else if (strcmp(input, "rm") == 0)
        {
            int x, y;
            scanf("%d %d", &x, &y);
            remove_point(&l, x, y);
        } else if (strcmp(input, "print") == 0)
        {
            show_all_points(&l);
        } else if (strcmp(input, "rma") == 0)
        {
            remove_all_points(&l);
        } else if (strcmp(input, "len") == 0)
        {
            printf("%d\n", get_length(&l));
        } else if (strcmp(input, "exit") == 0)
        {
            break;
        } else
        {
            printf("Unknown command\n");
        }
    }

    remove_all_points(&l);
    return 0;
}
