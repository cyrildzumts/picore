#include "dma.h"
#include <malloc.h>
static dma_cb_t* root = NULL;
static dma_cb_list_t cb_list;

dma_cb_t *dma_get_root()
{
    if(!root){
        root = (dma_cb_t *)malloc(sizeof (dma_cb_t));
    }
    return root;
}

void dma_add_cb(dma_cb_t *cb)
{
    if(cb_list.initialized){
        cb_list.tail->next = cb;
        cb_list.tail = cb;
        cb_list.size += cb->length;
    }
}

void dma_remove(dma_cb_t *cb)
{
    (void)cb;
}

void dma_init(dma_cb_t *cb)
{
    if(cb_list.initialized != 1){
        cb_list.head = dma_get_root();
        dma_copy(cb, cb_list.head);
        cb_list.tail = cb_list.head;
        cb_list.initialized = 1;
        cb_list.size = cb_list.head->length;
    }

}

int dma_is_empty()
{
    return cb_list.size == 0;
}

int dma_size()
{
    return cb_list.size;
}

void dma_copy(dma_cb_t *src, dma_cb_t *dst)
{
    if(dst && src){
        dst->destination = src->destination;
        dst->source = src->source;
        dst->length = src->length;
        dst->stride = src->stride;
        dst->transfer_info = src->transfer_info;
        dst->next = src->next;
    }
}

void dma_start()
{

}
