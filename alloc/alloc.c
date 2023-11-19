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


/// malloc 0???????
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
    if(allocatedMemory == NULL){
        return NULL;
    }

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
    printf("\nfree the pointer %p \n", ptr);
    void **temp = endFreeList;
    if (!freeList){
        freeList = ptr;
    }else{
        *temp = ptr;
    }
    endFreeList = ptr;
}

void *myrealloc(void *ptr, size_t size)
{   
    void* new;
    if(ptr == NULL){       
        return mymalloc(size);
    }
    else if(size == 0){
        new = mymalloc(8);
    }else{
        new = mymalloc(size);
    }
   // printf(" \n jo  %p ", ptr);
    obj_metadata* meta = new - sizeof(obj_metadata);
    //printf("gfgfgfg");
    //fflush(stdout);
    //printf("size %d ", meta->size);
    memcpy(new, ptr, meta->size);
   // printf("%#x bytes 1 %#x bytes 2 ", , );
    
    char* l = ptr;
   // printf(" \n ja %p size of old one %d and the value at %d \n", ptr , meta->size, *l);
   // printf(" \ncharacter 1 %d and character 2 %d\n", l[0], l[1]);
    
   char* u = new;
   printf("%#x bytes 1 %#x bytes 2 ", l[0] , u[0]);
   printf("this is the pointer %p ", new);
   fflush(stdout);
   // printf(" \n ja %p size of old one %d and the value at %d \n", new , meta->size, *l);
   // printf(" \ncharacter 1 %d and character 2 %d\n", l[0], l[1]);

    //myfree(ptr);
    return new;
}

static void* AllocateBlock(size_t size){

    size_t totalSize;
    void *newMem;
    if (size == 0){
        return NULL;
    }
    else if (size % 8 == 0){
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
