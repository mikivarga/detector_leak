#include "../inc/list.h"

static void copy_to_node(t_mem_block block, t_node *pnode);

void lst_init(t_list *plst)
{
    *plst = NULL;
}

Boolean lst_add_memory_block(t_mem_block block, t_list *plst, void *(*pfun)(size_t size))
{
    t_node *pnew;
    t_node *scan;

    scan = *plst;
    pnew = (t_node *)(*pfun)(sizeof(t_node));
    if (pnew == NULL) {
        return FALSE;
    }
    copy_to_node(block, pnew);
    pnew->next = NULL;
    if (scan == NULL) {
        *plst = pnew;
    } else {
        while (scan->next != NULL) {
            scan = scan->next;
        }
        scan->next = pnew;
    }
    return TRUE;
}

void lst_show_leaks(const t_list *plst)
{
    int leak_cnt, leak_size;
    t_node *pnode;

    pnode = *plst;
    leak_cnt = leak_size = 0;
    while(pnode != NULL) {
        leak_cnt++;
        leak_size += pnode->block.size;
        fprintf(stderr, "LEAK\t%zu\n", pnode->block.size);
        pnode = pnode->next;
    }
    fprintf(stderr, "TOTAL\t%d\t%d\n", leak_cnt, leak_size);
}

Boolean lst_delete_memory_block(void *ptr, t_list *plst, void (*pfun)(void *ptr))
{
    t_node *pnode, *psave;

    pnode = *plst;
    psave = NULL;
    while (pnode != NULL) {
        if (pnode->block.ptr == ptr) {
            if (psave == NULL) {
                *plst = pnode->next;
            }
            else {
                psave->next = pnode->next;
            }
            (*pfun)(pnode);
            return TRUE;
        }
        psave = pnode;
        pnode = pnode->next;
    }
    return FALSE;
}

void lst_delete(t_list *plst, void (*pfun)(void *ptr))
{
    t_node *psave;

    while (*plst != NULL) {
        psave = (*plst)->next;
        (*pfun)(*plst);
        *plst = psave;
    }
}

static void copy_to_node(t_mem_block block, t_node *pnode)
{
    pnode->block = block;
}