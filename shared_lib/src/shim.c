#define _GNU_SOURCE
#include <dlfcn.h>
#include <sys/wait.h>
#include "../inc/list.h"

void __attribute__ ((constructor)) init_malloc_free(void);
void __attribute__ ((destructor)) cleanup_malloc_free(void);
void __cyg_profile_func_enter (void *this_fn, void *call_site) __attribute__((no_instrument_function));
void __cyg_profile_func_exit  (void *this_fn, void *call_site) __attribute__((no_instrument_function));

void *malloc(size_t size);
void free(void *ptr);

void *(*original_malloc)(size_t size);
void (*original_free)(void *ptr);

static t_list lst_mem_blocks;
static t_mem_block block;

void init_malloc_free(void)
{
    original_malloc = dlsym(RTLD_NEXT, "malloc");
    original_free = dlsym(RTLD_NEXT, "free");
    lst_init(&lst_mem_blocks);

    return ;
}

void cleanup_malloc_free(void)
{
    lst_show_leaks(&lst_mem_blocks);
    lst_delete(&lst_mem_blocks, original_free);

    return ;
}

void *malloc(size_t size)
{
    void *alloc_block;
    Dl_info info;
    
    dladdr(malloc, &info);
    alloc_block = original_malloc(size);
    block.size = size;
    block.ptr = alloc_block;
    block.addr_shared_obj = info.dli_fbase;//correct add;
    block.path_shared_obj = info.dli_fname;
    block.addr_in_stack = __builtin_return_address(0);
    lst_add_memory_block(block, &lst_mem_blocks, original_malloc);
    //TODO: check if din't create new node!
    return alloc_block;
}

void free(void *ptr)
{
    lst_delete_memory_block(ptr, &lst_mem_blocks, original_free);
    //TODO: check if din't delete node!
    original_free(ptr);
    return ;
}
