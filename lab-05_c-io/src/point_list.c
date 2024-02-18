#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "clist.h"
#include "point_list.h"

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