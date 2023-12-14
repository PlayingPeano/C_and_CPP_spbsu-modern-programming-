#ifndef CLIST_H_
#define CLIST_H_

#include "stddef.h"

#define container_of(ptr, type, member) (type*)((char*)(ptr) - offsetof(type, member))
typedef struct intrusive_node intrusive_node;
typedef struct intrusive_list intrusive_list;

struct intrusive_node
{
    intrusive_node *next;
    intrusive_node *prev;
};

struct intrusive_list
{
    int size;
    intrusive_node *head;
};

void init_list(intrusive_list *listToInit);

void add_node(intrusive_list *listWhereToAdd, intrusive_node *nodeToAdd);

void remove_node(intrusive_list *listWhereDelete, intrusive_node *nodeToDelete);

int get_length(intrusive_list *listToGetSizeFrom);

#endif
