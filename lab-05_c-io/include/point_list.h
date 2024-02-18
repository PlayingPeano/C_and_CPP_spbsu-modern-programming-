#ifndef POINT_LIST_H_
#define POINT_LIST_H_

#include "clist.h"

typedef struct point point;
struct point
{
    int x, y;
    intrusive_node node;
};

void add_point(intrusive_list *list, int x, int y);

void remove_point(intrusive_list *list, int x, int y);

void show_all_points(intrusive_list *list);

void remove_all_points(intrusive_list *list);

#endif