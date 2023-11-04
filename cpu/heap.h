

#ifndef HEAP_H_
#define HEAP_H_
#include "types.h"
#include "ordered_list.h"

#define KHEAP_START 0xA0000000
#define KHEAP_SIZE  0x100000
#define KHEAP_MIN_SIZE  0x80000
#define KHEAP_LIST_SIZE 0x20000
#define KHEAP_MAGIC 0xC0DEBABE


typedef struct Header_t {
    u32 magic;
    u32 size;
    bool is_hole;
}  Header_t;

typedef struct Footer_t {
    u32 magic;
    Header_t* header;
}  Footer_t;


typedef struct Heap_t {
    OrderedList_t idx_list;
    u32 start_addr;
    u32 end_addr;

    u32 max_addr;
    bool supervisor;
    bool readonly;
} Heap_t;

Heap_t* heap_create(u32 start,u32 end,u32 max);
void* alloc(Heap_t heap,u32 size,bool aligned);
void free(Heap_t heap, void* pointer);

#endif

#define HEAP_IMPLEMENTATION_C
#ifdef HEAP_IMPLEMENTATION_C

#include "types.h"
#include "macros.h"
#include "paging.h"

#define ORDERED_LIST_STATIC_IMPLEMENTATION_C
#include "ordered_list.h"

// i dont take the page align arg
//TODO: take page alignt may be usefull when u do paging 
static s32 find_smallest_hole(Heap_t* heap,u32 size) {
    u32 i = 0;
    while(i < heap->idx_list.length) {
        Header_t* header = (Header_t*) ol_get(&heap->idx_list,i);
        if(header->size >= size && header->is_hole) {
            break;
        }
        i++;
    }

    if(i == heap->idx_list.length) {
        PANIC("No hole");
        return -1;
    }

    return i;
}


static bool header_t_lessthan(type_t* a,type_t* b) {
    return ((Header_t*)a)->size < ((Header_t*)b)->size; 
}

Heap_t* heap_create(u32 start,u32 end,u32 max) {
    Heap_t* heap = (Heap_t*) kmalloc(sizeof(Heap_t),0,0); 
    heap->idx_list = ol_create_at((void*)start,KHEAP_LIST_SIZE,(ol_lessthan*)(&header_t_lessthan));


    start += sizeof(type_t) * KHEAP_LIST_SIZE;
    
    heap->start_addr = start;
    heap->end_addr = end;
    heap->max_addr = max;
    heap->supervisor = true;
    heap->readonly = false;

    Header_t* hole = (Header_t*) start;
    hole->size = end - start;
    hole->magic = KHEAP_MAGIC;
    hole->is_hole = true;
    ol_insert(&heap->idx_list,hole);

    return heap;
}

//TODO: need paging to do this stuff

// static void expand(Heap_t* heap,u32 size) {
    // ASSERT(size > heap->end_addr - heap->start_addr);
    // 
    // if(size & 0xFFFFF000 != 0) {
        // size &= 0xFFFFF000;
        // size += 0x1000;
    // }
// 
    // ASSERT(heap->start_addr + size <= heap->max_addr);
    // u32 old_size = heap->end_addr - heap->start_addr;
// 
    // while (old_size < size) {
        // frame_alloc(page_get(heap->start_addr + old_size,kernel_dir))
    // }
// }

// static u32int contract(u32int new_size, heap_t *heap)
// {
//    Sanity check.
//    ASSERT(new_size < heap->end_address-heap->start_address);
//    Get the nearest following page boundary.
//    if (new_size&0x1000)
//    {
    //    new_size &= 0x1000;
    //    new_size += 0x1000;
//    }
//    Don't contract too far!
//    if (new_size < HEAP_MIN_SIZE)
    //    new_size = HEAP_MIN_SIZE;
//    u32int old_size = heap->end_address-heap->start_address;
//    u32int i = old_size - 0x1000;
//    while (new_size < i)
//    {
    //    free_frame(page_get(heap->start_address+i, 0, kernel_directory));
    //    i -= 0x1000;
//    }
//    heap->end_address = heap->start_address + new_size;
//    return new_size;
// }

void* heap_alloc(Heap_t* heap,u32 size) {
    u32 new_size =  size + sizeof(Header_t) + sizeof(Footer_t);  // calc the size with the header and footer
    s32 i = find_smallest_hole(heap,new_size);
    if(i == -1) { // didnt find a enoght hole
        PANIC("Shit-tone of err handling");
    }
    Header_t* found_header = (Header_t*) ol_get(&heap->idx_list,i);
    u32 header_pos = (u32) found_header;
    u32 hole_size = found_header->size;
    if(hole_size - new_size < sizeof(Header_t) + sizeof(Footer_t)) {
        size += hole_size - new_size;
        new_size = hole_size;
    }

    ol_remove(&heap->idx_list,i);


    Header_t *block_header  = (Header_t *)header_pos;
    block_header->magic = KHEAP_MAGIC;
    block_header->is_hole = false;
    block_header->size = new_size;

    Footer_t* block_footer = (Footer_t *)(header_pos + sizeof(Header_t) + size);
    block_footer->magic = KHEAP_MAGIC;
    block_footer->header = block_header;


    if(hole_size - new_size > 0) {
        Header_t* hole_header = (Header_t*) (header_pos + sizeof(Header_t) + size + sizeof(Footer_t));
        hole_header->magic = KHEAP_MAGIC;
        hole_header->is_hole = true;
        hole_header->size = hole_size - new_size;
    

        Footer_t* hole_footer = (Footer_t*) (hole_header + hole_header->size + sizeof(Footer_t));
        hole_footer->magic = KHEAP_MAGIC;
        hole_footer->header = hole_header;

        ol_insert(&heap->idx_list,(type_t)hole_header);
    }

    return (void*) block_header + sizeof(Header_t);
}

void* heap_free(Heap_t* heap,void* pointer) { 
    if(pointer == 0) return 0;

    Header_t* header = (Header_t*) (pointer - sizeof(Header_t));
    Footer_t* footer = (Footer_t*) (((u32)header) + header->size - sizeof(Footer_t));

    ASSERT(header->magic == KHEAP_MAGIC);
    ASSERT(footer->magic == KHEAP_MAGIC);

    header->is_hole = true;
    bool did_unify = false;

    Footer_t* left_footer = (Footer_t*) header - sizeof(Footer_t);
    if(left_footer->magic == KHEAP_MAGIC && left_footer->header->is_hole) {

        u32 size = header->size;
        
        header = left_footer->header;
        footer->header = left_footer->header;

        header->size += size;
        did_unify = true;
    }

    Header_t* right_header = (Header_t*) footer - sizeof(Footer_t);
    if(right_header->magic == KHEAP_MAGIC && right_header->is_hole) {
        header->size += right_header->size;
        
        footer = (Footer_t*) (right_header + right_header->size - sizeof(Footer_t));  

        u32 i = 0;
        while(i < heap->idx_list.length && ol_get(&heap->idx_list,i) != right_header ) {
            i++;
        }

        ASSERT(i < heap->idx_list.length);
        ol_remove(&heap->idx_list,i);

        did_unify = true;
    }

    if(!did_unify) {
        ol_insert(&heap->idx_list,header);
    }

    return 0;
}

#undef ORDERED_LIST_STATIC_IMPLEMENTATION_C
#endif