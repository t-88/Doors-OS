#pragma once
#include "shared.h"


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
static const char* mmap_entry_type_to_str();





// #define MMAP_IMPLEMENTATION
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
}

void mmap_print_map() {
    for (u8 i = 0; i < MMAP_ENTRIES_COUNT; i++) {
        PRINT("baseAddress: ");PRINT_HEX(mmap_entries[i].baseAddress);
        PRINT("      length     : ");PRINT_HEX(mmap_entries[i].length);
        PRINT("      type       : ");PRINT(mmap_entry_type_to_str(mmap_entries[i].type));
        PRINTLN();
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
        break;
    }
}

#endif


