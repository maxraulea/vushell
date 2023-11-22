#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include "metadata.h"

#define SBRK_ERROR (void*)-1
#define ALLIGNMENT sizeof(long)
#define LOCALITY 5
#define MEM_CHUNK 512

static void* lastFreed[LOCALITY];

void append(void* new);
void* findInLastFreed(size_t size);
void removeFromLastFreed(void* new);

static void* AllocateBlock(size_t size);

int decreaseBreak(void* ptr, void* head, void* tail);

void *beginChunk = NULL;;
size_t remainingBytes = 0;

void **freeList; // store all the free blocks pointer to freed region, in that region is the pointer to next freed regio
void *endFreeList;

void *mymalloc(size_t size)
{

    obj_metadata *freeBlock;
    void *allocatedMemory;
    size_t totalSize;
    if (size == 0){
        return NULL;
    }

    if (size % 8 == 0){
        totalSize = sizeof(obj_metadata) + size;
    }
    else{
        totalSize = sizeof(obj_metadata) + size + 8 - size % 8;
    }

     freeBlock = findInLastFreed(totalSize - 8);
           
     if(freeBlock){
         list newList = DeleteFromList(freeList, endFreeList, freeBlock);
         removeFromLastFreed(freeBlock);
         endFreeList = newList.tail;
         freeList = newList.head;
         return freeBlock;    
     }

    freeBlock = FindFreeBlock(totalSize, freeList, endFreeList);

    if(freeBlock){

        list newList = DeleteFromList(freeList, endFreeList, freeBlock);
        endFreeList = newList.tail;
        freeList = newList.head;
        return freeBlock;    
    }


    allocatedMemory = AllocateBlock(totalSize);
    if(allocatedMemory == NULL){
        return NULL;
    }

    EditMetadata(allocatedMemory, totalSize - sizeof(obj_metadata), 0);
    //printf("allocated pointer %p with size %lu \n",allocatedMemory + sizeof(obj_metadata), totalSize);
    //fflush(stdout);

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
    if(ptr == NULL){
        return;
    }
    if (!freeList){
        freeList = ptr;
        endFreeList = ptr;
        lastFreed[0] = ptr;
    }else{
        list l = AddToList(freeList, endFreeList, ptr);
        freeList = l.head;
        endFreeList = l.tail;
        append(ptr);
        endFreeList = MergeFreeList(freeList, endFreeList, lastFreed);
        
        printf("head: %p and tail: %p", freeList, endFreeList);

        if(!decreaseBreak(ptr, freeList, endFreeList)){
            list newList = DeleteFromList(freeList, endFreeList, ptr);    
            removeFromLastFreed(ptr);      
            endFreeList = newList.tail;
            freeList = newList.head;
            printf("YEEEEEETTTTTTTT \n\n");
            fflush(stdout);
        }
        
        printfreelist(freeList, endFreeList);
        printf("head: %p and tail: %p", freeList, endFreeList);
    }
    
    
}

void *myrealloc(void *ptr, size_t size)
{   
    void* new;
    if(ptr == NULL){       
        return mymalloc(size);
    }
    else if(size == 0){
        new = mymalloc(0);
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

static void* AllocateBlock(size_t totalSize){
    if(remainingBytes < totalSize){
        size_t size = totalSize;
        if(beginChunk == NULL){
            beginChunk = sbrk(MEM_CHUNK);
            remainingBytes += MEM_CHUNK;
        }

        while(remainingBytes < size){
            remainingBytes += MEM_CHUNK;
            if (sbrk(MEM_CHUNK) == SBRK_ERROR){
                return NULL;
            }
        }
        remainingBytes -= totalSize;
        beginChunk += totalSize;
        return beginChunk - totalSize;
    }
    else{
        remainingBytes -= totalSize;
        beginChunk += totalSize;
        return beginChunk - totalSize;
    }
    
}

void append(void* new){

    for(int i = LOCALITY - 1; i > 0; i--){
        lastFreed[i] = lastFreed[i - 1];
    }
    lastFreed[0] = new;
}

void* findInLastFreed(size_t size){
    obj_metadata* data;
    void* new;
    
    for(int i = 0; i < LOCALITY; i++){
        if(lastFreed[i] == NULL){
            continue;
        }
        
        data = lastFreed[i] - sizeof(obj_metadata);

        if(data->size >= size){
            if(data->size - size > 16){
                data->size = data->size - size - sizeof(obj_metadata);
                new = lastFreed[i] + data->size;
                data = new;
                data->size = size - sizeof(obj_metadata); 
                return new;
                
            }
            return lastFreed[i];
        }
    }
    return NULL;
}

void removeFromLastFreed(void* new){
    for(int i = 0; i < LOCALITY; i++){
        if(lastFreed[i] == NULL){
            continue;
        }
        if(lastFreed[i] == new){
            lastFreed[i] = NULL;
        }
    }
}

int decreaseBreak(void* ptr, void* head, void* tail){
    obj_metadata* data;
    void *currBrk = sbrk(0);

    if(head == tail){
        data = head - sizeof(obj_metadata);
        if(currBrk == head + data->size + remainingBytes){
            brk(head - sizeof(obj_metadata));
            remainingBytes = 0;
            return 0;
        }
    }
    else{
        data = ptr - sizeof(obj_metadata);
        printf("curr brk %p the ptr %p and my lookup %p \n", currBrk, ptr, ptr + data->size + remainingBytes);
        if(currBrk == ptr + data->size + remainingBytes){
            brk(ptr - sizeof(obj_metadata));
            remainingBytes = 0;
            return 0;
        }
    }

    return 1;
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
