#ifndef PLEX_HEADER
#define PLEX_HEADER

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#ifdef __GNUC__
    #define PLEX_INLINE inline
    #define PLEX_STATIC static
#elif _MSC_VER
    #define PLEX_INLINE __inline
    #define PLEX_STATIC
#else
    #define PLEX_INLINE
    #define PLEX_STATIC static
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct mem_block {
    struct mem_block* next;
#ifdef PLEX_ENABLE_RESET
    unsigned int      nmemb;
#endif
    char              data[0];
};

union mem_item {
    union mem_item*   next;
    char              data[sizeof(char*)];
};

struct plex {
    struct mem_block* head;
    union mem_item*   free;
    size_t            item_size;
#ifdef PLEX_DEBUG
    size_t            count_alloc;
    size_t            count_free;
#endif
};

typedef struct plex plex_t;

PLEX_STATIC PLEX_INLINE void plex_init(struct plex* me, size_t item_size) 
{
    assert(me && item_size);
    me->head = 0;
    me->free = 0;
    me->item_size = item_size<sizeof(void*) ? sizeof(void*) : item_size;
#ifdef PLEX_DEBUG
    me->count_alloc = 0;
    me->count_free = 0;
#endif
}

PLEX_STATIC PLEX_INLINE int plex_expand(struct plex* me, unsigned int nmemb, void*(*malloc_f)(void*, size_t), void* ctx) 
{
    struct mem_block* block = 0;
    union mem_item* item = 0;
    unsigned int i = 0;

    assert(me && malloc_f);

    block = (struct mem_block*)malloc_f(ctx, sizeof(struct mem_block) + nmemb * me->item_size);
    if (!block)
        return -1;
#ifdef PLEX_ENABLE_RESET
    //remember nmemb
    block->nmemb = nmemb; 
#endif

    //join to head
    block->next = me->head;
    me->head = block;

    //join to free_list
    item = (union mem_item*)block->data;
    for (; i<nmemb; i++) {
        item->next = me->free;
        me->free = item; 
        item = (union mem_item*)(item->data + me->item_size);
    }
    return 0;
}

PLEX_STATIC PLEX_INLINE void plex_clear(struct plex* me, void(*free_f)(void*ctx, void*ptr), void* ctx) 
{
    assert(me && free_f);
    while (me->head) {
        struct mem_block* bak = me->head;
        me->head = bak->next;
        free_f(ctx, bak);
    }
    me->head = 0;
    me->free = 0;
#ifdef PLEX_DEBUG
    me->count_alloc = 0;
    me->count_free = 0;
#endif
}

PLEX_STATIC PLEX_INLINE void* plex_alloc(struct plex* me) 
{
    void* ret = 0;

    assert(me);
    if (!me->free)
        return 0;

    ret = me->free;
    me->free = me->free->next;
#ifdef PLEX_DEBUG
    me->count_alloc++;
#endif
    return ret;
}

PLEX_STATIC PLEX_INLINE void plex_free(struct plex* me, void* ptr) 
{
    assert(me && ptr);
    ((union mem_item*)ptr)->next = me->free;
    me->free = (union mem_item*)ptr;
#ifdef PLEX_DEBUG
    me->count_free++;
#endif
}

#ifdef PLEX_ENABLE_RESET

PLEX_STATIC PLEX_INLINE void plex_reset(struct plex* me)
{
    unsigned int i = 0;
    struct mem_block* block = me->head;

    me->head = 0;
    me->free = 0;
#ifdef PLEX_DEBUG
    me->count_alloc = 0;
    me->count_free = 0;
#endif

    //join to free_list
    while (block) {
        union mem_item* item = (union mem_item*)block->data;
        for (i=0; i<block->nmemb; i++) {
            item->next = me->free; 
            me->free = item; 
            item = (union mem_item*)(item->data + me->item_size);
        }
        block = block->next;
    }
}

PLEX_STATIC PLEX_INLINE  size_t plex_size(struct plex* me)
{
    size_t size = 0;
    struct mem_block* block = me->head;
    while (block) {
        size += (sizeof(struct mem_block) + block->nmemb * me->item_size);
        block = block->next;
    }
    return size;
}

PLEX_STATIC PLEX_INLINE size_t plex_item_count(struct plex* me)
{
    size_t count = 0;
    struct mem_block* block = me->head;
    while (block) {
        count += block->nmemb;
        block = block->next;
    }
    return count;
}
#endif

PLEX_STATIC PLEX_INLINE void* sys_malloc(void* ctx, size_t size)
{
    void* buf = malloc(size);
    memset(buf, 0, size); 
    return buf;
}

PLEX_STATIC PLEX_INLINE void sys_free(void* ctx, void* ptr)
{
    free(ptr);
}

#define ITEM_MALLOC(plex, size) (plex) ? plex_alloc(plex) : malloc(size); 

#define ITEM_FREE(plex, ptr) (plex) ? plex_free(plex, ptr) : free(ptr); 

#ifdef __cplusplus
}
#endif

#endif 

