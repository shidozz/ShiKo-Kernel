#ifndef __PAGING_H__
#define __PAGING_H__
#include <stdint.h>

// Structure d'entrée de table des pages
typedef struct {
    uint64_t valid: 1;
    uint64_t address: 48;
    // Autres attributs de contrôle des pages (présents, permission, etc.)
    // ...
} PageTableEntry;

void map_pages();
void initialize_page_table();
void enable_pagination();
#endif
