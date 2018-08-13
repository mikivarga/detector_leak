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

static char *find_func_name(void **ptr, char *pfile)
{
    int n;
    char *stack;
    char addr_fun[BUF_SIZE];

    snprintf(addr_fun, BUF_SIZE, "%0x", *(unsigned int *)ptr);
    while (*pfile) {
        n = memcmp(addr_fun, pfile + 10, 6);
        if (n > 0) {
            stack = pfile;
            pfile = strchr(pfile, '\n');
            pfile++;
        } else if (n < 0) {
            return stack;
        }
    }
    return NULL;
}

static char *open_file(struct stat *sb)
{
    int fd;
    char *fptr;

    fd = open(getenv("FILE"), O_RDONLY);
    if (fd == -1) {
        HANDLE_ERROR("open");
    }
    if (fstat(fd, sb) == -1) {
        HANDLE_ERROR("fstat");
    }
    fptr = mmap(0, (*sb).st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (fptr == MAP_FAILED) {
        HANDLE_ERROR("mmap");
    }
    if (close(fd) == -1) {
        HANDLE_ERROR("close");
    }
    return fptr;
}

static void show_addr_call_func(char *fptr, t_node *pnode)
{
    char buf[BUF_SIZE];
    char *pfunc_name;

    fprintf(stderr, "LEAK\t%zu bytes", pnode->block.size);
    fprintf(stderr, "\tat %p: malloc (in %s)\n",
    pnode->block.addr_shared_obj, pnode->block.path_shared_obj);
    pfunc_name = find_func_name(&(pnode->block.addr_in_stack),fptr);
    memset(buf, '\0', BUF_SIZE);
    strncpy(buf, pfunc_name + 19,
    strchr(pfunc_name + 19, '\n') - (pfunc_name + 19));
    fprintf(stderr, "\t\t\tby %p: %s (in %s)\n\n",
    pnode->block.addr_in_stack, buf, getenv("PRG_NAME"));
}

void lst_show_leaks(const t_list *plst)
{
    int leak_cnt, leak_size;
    char *fptr;
    t_node *pnode;
    struct stat sb;

    pnode = *plst;
    leak_cnt = leak_size = 0;
    fptr = open_file(&sb);
    fprintf(stderr, "\n\tHEAP SUMMARY:\n");
    while(pnode != NULL) {
        leak_cnt++;
        leak_size += pnode->block.size;
        show_addr_call_func(fptr, pnode);
        pnode = pnode->next;
    }
    fprintf(stderr, "\tLEAK SUMMARY:\n");
    fprintf(stderr, "TOTAL LOST:\t%d bytes in %d blocks\n", leak_size, leak_cnt);
    if (munmap(fptr, sb.st_size) == -1) {
        HANDLE_ERROR("munmap");
    }
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