#ifndef ORDERED_LIST_H
#define ORDERED_LIST_H
#include "types.h"

typedef bool (*ol_lessthan)(type_t a,type_t b);
typedef struct OrderedList_t {
    type_t* array;
    u32 length;
    u32 size;
    ol_lessthan less_than;
} OrderedList_t;

OrderedList_t ol_create(u32 size, ol_lessthan callback);
OrderedList_t ol_create_at(void* addr, u32 size, ol_lessthan* callback);

bool ol_default_lessthan(type_t a,type_t b); // works on addrs
void ol_destory(OrderedList_t* ol);
void ol_insert(OrderedList_t* ol,type_t item);
type_t ol_get(OrderedList_t* ol,u32 i);
void ol_remove(OrderedList_t* ol,u32 i);
#endif

// #define ORDERED_LIST_IMPLEMENTATION_C
#if defined(ORDERED_LIST_IMPLEMENTATION_C) || defined(ORDERED_LIST_STATIC_IMPLEMENTATION_C) 
#include "types.h"
#include "macros.h"
#include  "mem.h"


OrderedList_t ol_create(u32 size, ol_lessthan callback) {
    OrderedList_t list;
    list.array = kmalloc(size * sizeof(type_t),0,0);
    memset(list.array,0,size * sizeof(type_t));
    list.length = 0;
    list.size = size;
    list.less_than = callback;
    return list;
}
OrderedList_t ol_create_at(void* addr, u32 size, ol_lessthan* callback)  {
    OrderedList_t list;
    list.array =  (type_t*) addr; // make sure u dont override it
    // memset(list.array,0,size * sizeof(type_t));
    list.length = 0;
    list.size = size;
    list.less_than = callback;
    return list;    
}


bool ol_default_lessthan(type_t a,type_t b) {
    return a < b;    
} 
void ol_destory(OrderedList_t* ol) {

}
void ol_insert(OrderedList_t* ol,type_t item) {
    ASSERT(ol->less_than != 0);
    ASSERT(ol->length < ol->size);
    
    u32 i = 0;
    while (i < ol->length && ol->less_than(ol_get(ol,i),item)) {
        i++;
    }
    if(i >= ol->size) {
        PANIC("[ol error] out of range error");
    }
    else if(i == ol->length) {
        ol->array[i] = item;
        ol->length++;
    }
    else {
        for (s32 j = ol->length; j > i ; j--) {
            ol->array[j] = ol->array[j - 1];
        }
        ol->array[i] = item;
        ol->length++;
    }
}
type_t ol_get(OrderedList_t* ol,u32 i) {
    return ol->array[i];
}
void ol_remove(OrderedList_t* ol,u32 i) {
    // kfree(ol);
}

#endif