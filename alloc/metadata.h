#ifndef metadata_h
#define metadara_h

#include <unistd.h>

// free list can leave for making a freelist
//store pointer in the free region
typedef struct obj_metadata{
//int is_free : 1;
size_t size;
// : 63;
} obj_metadata;

typedef struct{
    void **head;
    void *tail;
} list;

list AddToList(void **head, void *tail, void *node);

list DeleteFromList(void **head, void *tail, void *node);

void EditMetadata(obj_metadata *obj, size_t size, long int free);

void* MergeFreeList(void **head, void *tail, void** lastFreed);

void printfreelist(void **head, void* tail);

void* FindFreeBlock(size_t size, void** heap, void *tail);

#endif