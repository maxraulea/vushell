#include "metadata.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void removeFromLastFreed2(void* new, void** lastFreed){
    for(int i = 0; i < 5; i++){
        if(lastFreed[i] == NULL){
            continue;
        }
        if(lastFreed[i] == new){
            lastFreed[i] = NULL;
        }
    }
}

list AddToList(void **head, void *tail, void *node){

    // try to merge blocks if possible

    void **last, *nextData;
    list newList;

    newList.head = head;
    newList.tail = tail;

    nextData = head;
    last = nextData;
       
    while (nextData != NULL)
    {
        
                
        if(nextData > node){
            void **temp = node;
            if(nextData == head){

                *temp = head;
                newList.head = node;
                return newList;
            }
            // this isnt right
            *temp = *last;
            *last = node;
            return newList;
        }

// check if you can free data
        if(tail == nextData){
            void **temp = tail;
            *temp = node;
            newList.tail = node;
            return newList;    
        }

        last = nextData;        
        nextData = *last;
        
        
    }
    return newList;
}


void EditMetadata(obj_metadata *obj, size_t size, long int free){
    if(free){
        //obj->is_free = free;
    }
        obj->size = size;
    
    return;
}

list DeleteFromList(void **head, void *tail, void *node){
    printf(" WE WILL DELETE YOU %p \n", node);
    fflush(stdout);
    void **last, **nextData, **temp;
    list newList;

    nextData = head;
    last = nextData;
    temp = nextData;
    newList.head = head;
    newList.tail = tail;

    if(head == node && tail == node){
        newList.head = NULL;
        newList.tail = NULL;
        return newList;
    }
    else if(head == node){
        newList.head = *head;
        return newList;
    }
    else if(tail == node){
        
        while(*nextData != tail){
            last = nextData;
            nextData = *last;
        }
        newList.head = nextData;
        return newList;
    }

    while (nextData != NULL)
    {
        if (nextData == node){

            *temp = *nextData;
            return newList;
        }
        temp = last;
        last = nextData;

        if (nextData == tail){
            newList.tail = temp;
            return newList;
        }

        nextData = *last;
        
    }    
    return newList;
}

void* FindFreeBlock(size_t size, void** head, void *tail){

    obj_metadata* data; 
    void **last,* nextData;

    if(head == NULL){
        return NULL;
    }

    nextData = head;


    if(nextData == tail){
        data = nextData - sizeof(obj_metadata);
        printf("head: %p and tail: %p", head, tail);
        if(data->size >= size){
            if(data->size - size > 16){
                data->size = data->size - size - sizeof(obj_metadata);
                nextData += data->size;
                data = nextData;
                data->size = size - sizeof(obj_metadata);
                return nextData + sizeof(obj_metadata);
                
            }
            return nextData;
        }

        return NULL;
    }

    while (nextData != NULL)
    {


        last = nextData;

        if(nextData == head){
            printf("MEGAAAAAAA MANNNNN %p %p %p\n \n", *head,  nextData, tail);
            fflush(stdout);
        }
        data = *last - sizeof(obj_metadata);

        if(*last == tail){
            return NULL;
            
        }
        

        if(data->size >= size){

            if(data->size - size > 16){

                data->size = data->size - size - sizeof(obj_metadata);
                nextData = *last;

                nextData += data->size;  
                data = nextData;

                data->size = size - sizeof(obj_metadata);
                return nextData + sizeof(obj_metadata);
               
            }

            return *last;
        }

        nextData = *last;
        
    }
    return NULL;
}

void printfreelist(void **head, void* tail){
    void **next;
    next = head;

    while(*next != NULL){
        // if(next > *next){
        //      printf("\n\n YOOUUUUU FAAIILEEDD \n \n");
        //      return;
        //  }
        printf(" next %p --> ", next);
        if(next == tail){
            printf("\n");
            return;
        }
        next = *next;
    }
    printf("\n");
    return;
}

// check the next block for every block !!!!!!!!!!!!!!!!
// return the tail of the list
// if the tail merges it changes
void* MergeFreeList(void **head, void *tail, void** lastFreed){

    obj_metadata *data, *toMergeData;
    void **temp, *nextData, **last;


    //data = node - 1;
    //temp = node + 2;
    //printf("%p node and the *node %p  and the size %d \n", node + 1, *node, data->size);
/*
    if(node + data->size + sizeof(obj_metadata) == *node){

        toMergeData = *node - sizeof(obj_metadata);
        data->size += toMergeData->size;
        temp = *node;
        printf("%p node  *node %p  and the size %p \n", *node, *temp, node);
        *node = *temp;
        
    }
    if(node + data->size + sizeof(obj_metadata) == *node){

        toMergeData = *node - sizeof(obj_metadata);
        data->size += toMergeData->size;
        temp = *node;
        printf("%p node  *node %p  and the size %p \n", *node, *temp, node);
        *node = *temp;
        
    }
*/
    nextData = head;

    while (nextData != NULL)
    {
        last = nextData;
        data = nextData - sizeof(obj_metadata);

        if(nextData == tail){
            return tail;
        }

        if(nextData + data->size + sizeof(obj_metadata) == *last){
        // remove from last freed *last
        
        toMergeData = *last - sizeof(obj_metadata);
        data->size += toMergeData->size + sizeof(obj_metadata);
        temp = nextData;
        printf(" %p \n", nextData);
        printf("%p in the whilleeee loop %p  and the size %lu \n", *last, nextData, data->size);
        printf(" %p \n", nextData);
        *temp = *last;
        removeFromLastFreed2(*last, lastFreed);
        if(*last == tail){
            return nextData;
        }
         printf(" %p \n", *last);

         
        continue;
        
    }

        nextData = *last;
        
    }
    return tail;
}
