#include "shared.h"
#include "print.h"

#ifndef MMAP_H_
#define MMAP_H_ 


typedef struct MmapEntry_t {
	u64 baseAddress;	
	u64 length;		    
	u32 type;		    
	u32 acpi_null;	    
} MmapEntry_t;

typedef enum EntryType {
    EntryType_Available = 1,
    EntryType_Reserved,
    EntryType_ACPI,
    EntryType_ACPI_NVS,
} EntryType;


#define MMAP_ENTRY (u8*)0x1200
#define MMAP_ENTRIES_COUNT 7
MmapEntry_t mmap_entries[MMAP_ENTRIES_COUNT];


void mmap_load_map();
static  void mmap_compress_map();
static const char* mmap_entry_type_to_str();

#endif



#define MMAP_IMPLEMENTATION
#ifdef MMAP_IMPLEMENTATION
void mmap_load_map() {
    u8* mmap_entry = MMAP_ENTRY;

    for(int i = 0; i < MMAP_ENTRIES_COUNT; i++) {
        mmap_entries[i].baseAddress = *((u64*)(mmap_entry));
        mmap_entries[i].length =      *((u64*)(mmap_entry + 8));
        mmap_entries[i].type =        *((u32*)(mmap_entry + 8 + 8)); 
        mmap_entries[i].acpi_null =   *((u32*)(mmap_entry + 8 + 8 + 4));

        mmap_entry += 8 + 8 + 4 + 4;
    }

    mmap_compress_map();
}
static void mmap_compress_map() {
    // TODO: make sort faster 
    // NOTE: faster sort? does it even worth it for number of elements less then 10
    // applying bubble sort to sort the mmap_entries by start addr 

    for (u8 i = 0; i < MMAP_ENTRIES_COUNT; i++) {
        for (u8 j = 0 ; j < MMAP_ENTRIES_COUNT - 1; j++) {
            if(mmap_entries[j].baseAddress > mmap_entries[j+1].baseAddress) {
                MmapEntry_t temp_entry = mmap_entries[j];
                mmap_entries[j] = mmap_entries[j+1]; 
                mmap_entries[j+1] = temp_entry; 
                printf("\n");
            }
        }
    }
}

void mmap_print_map() {
    for (u8 i = 0; i < MMAP_ENTRIES_COUNT; i++) {
        printf("baseAddress: %10lx, length %9lx, type: %s\n",
                            mmap_entries[i].baseAddress,
                            mmap_entries[i].length,
                            mmap_entry_type_to_str(mmap_entries[i].type)
                        );
    }
}


static const char* mmap_entry_type_to_str(u32 type) {
    switch (type)
    {
        case EntryType_Available:
            return "Available";
        case EntryType_Reserved:
            return "Reserved";
        case EntryType_ACPI:
            return "ACPI";
        case EntryType_ACPI_NVS:
            return "ACPI_NVS";
        default:
            return "Reserved";
            return;
    }
}

#endif


