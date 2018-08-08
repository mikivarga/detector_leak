#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {FALSE, TRUE} Boolean;

typedef struct s_mem_block{
    void *ptr;
    size_t size;
} t_mem_block;

typedef struct s_node {
    t_mem_block block;
    struct s_node *next;
} t_node;

typedef t_node *t_list;

void lst_init(t_list *plst);
Boolean lst_add_memory_block(t_mem_block block, t_list *plst, void *(*pfun)(size_t size));
void lst_show_leaks(const t_list *plst);
Boolean lst_delete_memory_block(void *ptr, t_list *plst, void (*pfun)(void *ptr));
void lst_delete(t_list *plst, void (*pfun)(void *ptr));

#endif /*LIST_H*/
