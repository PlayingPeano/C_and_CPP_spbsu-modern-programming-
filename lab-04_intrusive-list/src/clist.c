#include "clist.h"

void init_list(intrusive_list *listToInit)
{
    listToInit->size = 0;
    listToInit->head = NULL;
}

void add_node(intrusive_list *listWhereToAdd, intrusive_node *nodeToAdd)
{
    if (!listWhereToAdd || !nodeToAdd)
    {
        return;
    }

    nodeToAdd->next = listWhereToAdd->head;
    nodeToAdd->prev = NULL;

    if (listWhereToAdd->head)
    {
        listWhereToAdd->head->prev = nodeToAdd;
    }

    listWhereToAdd->head = nodeToAdd;
    ++listWhereToAdd->size;
}

void remove_node(intrusive_list *listWhereDelete, intrusive_node *nodeToDelete)
{
    if (!listWhereDelete || !nodeToDelete)
    {
        return;
    }
    if (nodeToDelete->prev)
    {
        nodeToDelete->prev->next = nodeToDelete->next;
    } else
    {
        listWhereDelete->head = nodeToDelete->next;
    }

    if (nodeToDelete->next)
    {
        nodeToDelete->next->prev = nodeToDelete->prev;
    }

    --listWhereDelete->size;
}

int get_length(intrusive_list *listToGetSizeFrom)
{
    return listToGetSizeFrom->size;
}
