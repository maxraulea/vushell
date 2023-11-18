#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include "metadata.h"

#define SBRK_ERROR (void*)-1
#define ALLIGNMENT sizeof(long)

static void* AllocateBlock(size_t size);

void *memObjects;

void **freeList; // store all the free blocks pointer to freed region, in that region is the pointer to next freed regio
void *endFreeList;

void *mymalloc(size_t size)
{

    obj_metadata *freeBlock;
    void *allocatedMemory;

    freeBlock = FindFreeBlock(size, freeList, endFreeList);

    if(freeBlock){
        list newList = DeleteFromList(freeList, endFreeList, freeBlock);
        endFreeList = newList.tail;
        freeList = newList.head;
        return freeBlock;    
    }


    allocatedMemory = AllocateBlock(size);

    EditMetadata(allocatedMemory, size, 0);

    return allocatedMemory + sizeof(obj_metadata);
}

void *mycalloc(size_t nmemb, size_t size)
{
    void* mem = mymalloc(nmemb * size);
    memset(mem, 0, nmemb * size);
    return mem;
}

void myfree(void *ptr)
{   
    //obj_metadata *temp = freeList;
    if (!freeList){
        freeList = ptr;
    }else{
        AddToList(freeList, endFreeList, ptr);
    }
    endFreeList = ptr;
}

void *myrealloc(void *ptr, size_t size)
{   
    void* new = mymalloc(size);
    if(ptr == NULL){
        return new;
    }
    obj_metadata* meta = ptr - sizeof(obj_metadata);
    memcpy(new, ptr, meta->size);
    free(ptr);
    return new;
}

static void* AllocateBlock(size_t size){

    size_t totalSize;
    void *newMem;

    if (size % 8 == 0){
        totalSize = sizeof(obj_metadata) + size;
    }
    else{
        totalSize = sizeof(obj_metadata) + size + 8 - size % 8;
    }
    
    newMem = sbrk(totalSize);
    if (newMem == SBRK_ERROR){
        return NULL;
    }

    return newMem;
}

/*
 * Enable the code below to enable system allocator support for your allocator.
 * Doing so will make debugging much harder (e.g., using printf may result in
 * infinite loops).
 */
#if 0
void *malloc(size_t size) { return mymalloc(size); }
void *calloc(size_t nmemb, size_t size) { return mycalloc(nmemb, size); }
void *realloc(void *ptr, size_t size) { return myrealloc(ptr, size); }
void free(void *ptr) { myfree(ptr); }
#endif
