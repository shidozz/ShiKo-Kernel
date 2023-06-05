#ifndef __MMU_H__
#define __MMU_H__
#include <stdint.h>
// Structure d'une entrée de table de pagination
typedef struct PageTableEntry {
    uint64_t valid    : 1;  // Bit de validité
    uint64_t table    : 1;  // Indicateur de table ou de page
    uint64_t pxn      : 1;  // Privilège d'exécution
    uint64_t xn       : 1;  // Privilège d'exécution (Noyau)
    uint64_t ap       : 2;  // Privilèges d'accès
    uint64_t sh       : 2;  // Partage
    uint64_t af       : 1;  // Accès autorisé
    uint64_t ng       : 1;  // Non-global
    uint64_t output   : 36; // Adresse physique ou adresse de la prochaine table
} PageTableEntry;
uint64_t translate_address(uint64_t virtual_address);
void mmu_init();
void page_fault_handler();
void* allocate_page();
uint8_t* example();
#endif
