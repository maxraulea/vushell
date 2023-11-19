#include "metadata.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void AddToList(void **head, void *tail, void *node){

    void **last,* nextData;
    if(*head == NULL){
        return;
    }

    nextData = head;
    
    while (nextData != NULL)
    {   
        last = nextData;
        //printf("%p and the other %p and nog 1 %p\n", nextData, tail, node);
        if(last == tail){
            break;
            
        }
        nextData = *last;
        
    }

    *last = node;
    return;
}


void EditMetadata(obj_metadata *obj, size_t size, long int free){
    if(free){
        //obj->is_free = free;
    }
    if(size){
        obj->size = size;
    }  
    return;
}

list DeleteFromList(void **head, void *tail, void *node){

    void **last, **nextData, **temp;
    list newList;

    nextData = head;
    last = nextData;
    temp = nextData;
    newList.head = head;
    newList.tail = tail;

    //printf(" \n \n jaaaaaaaaaa \n \n ");

    if(head == node){
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
            //printf(" \n \nnode\n \n ");
            printf(" \n this pointer gets deletded %p and this is the node %p and check %p\n \n ", tail, node, *nextData);
            printf(" \n this pointer gets deletded %p and this is the node %p and check %p\n ", temp, *temp, last);
            *temp = *nextData;
            return newList;
        }
        temp = last;
        last = nextData;
        //printf("%p and the other %p and nog 1 %p\n", nextData, tail, node);

        nextData = *last;
        
    }    
    return newList;
}

void* FindFreeBlock(size_t size, void** head, void *tail){

    //void** last = head;
    //void* nextData = *last;

    obj_metadata* data; 
    void **last,* nextData;
    size_t maxSize;

    if(head == NULL){
        return NULL;
    }
        nextData = head;
       
    while (nextData != NULL)
    {

        last = nextData;
        data = *last - sizeof(obj_metadata);

        if(last == tail){
            return NULL;
            
        }
        maxSize = data->size;

        if(maxSize > 20000){
            printf("\n \n \n  why the hell are you soooo big  \n \n \n");
            printf(" here come all the pointers  the tail: %p  the head: %p nextdata: %p data: %p last*: %p and the size %#x and the data", tail, head, nextData, data, *last, data);
        }
        // + 8 - data->size % 8;

        if(maxSize >= size){
            printf(" \nthis is the size %lu \n", maxSize);
            printf(" \n %p and \n", *last);
            return *last;
        }


        nextData = *last;
        
    }
    return NULL;
}
