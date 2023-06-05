#include <stdint.h>
#include "../include/mmu.h"
#include "../include/serial.h"
// Définition des tables de pagination
#define NUM_TABLE_ENTRIES 512
#define PAGE_SIZE 4096

// Adresse de la prochaine page disponible dans la mémoire physique
uint64_t next_physical_page = 0x400000; // Exemple, à ajuster selon votre système


// Déclaration de la variable pour la table de niveau supérieur
PageTableEntry **page_table_l1;

// Initialisation de la MMU
void mmu_init() {
    // Allocation de la table de niveau supérieur
    page_table_l1 = (PageTableEntry**)allocate_page();

    // Initialisation des entrées de table à zéro
    for (int i = 0; i < NUM_TABLE_ENTRIES; i++) {
        page_table_l1[i] = 0;
    }
}


// Traduction d'une adresse virtuelle en adresse physique
uint64_t translate_address(uint64_t virtual_address) {
    // Obtention de l'index de la table de niveau supérieur
    uint64_t l1_index = (virtual_address >> 30) & 0x1FF;

    // Obtention de l'index de la table de niveau inférieur
    uint64_t l2_index = (virtual_address >> 21) & 0x1FF;

    // Obtention de l'index de la page
    uint64_t page_index = (virtual_address >> 12) & 0x1FF;

    // Vérification de la validité de l'entrée de table de niveau supérieur
    if (!page_table_l1[l1_index]->valid) {
        // Gestion de la page manquante
        handle_page_fault();
    }

    // Obtention de l'entrée de table de niveau inférieur correspondante
    PageTableEntry **page_table_l2 = (PageTableEntry **)page_table_l1[l1_index]->output;

    // Vérification de la validité de l'entrée de table de niveau inférieur
    if (!page_table_l2[l2_index]->valid) {
        // Gestion de la page manquante
        handle_page_fault();
    }

    // Obtention de l'adresse physique correspondante
    uint64_t physical_address = (page_table_l2[l2_index]->output << 12) + (page_index << 12);

    return physical_address;
}

// Gestion de l'erreur de page manquante (page fault)
void handle_page_fault() {
    // Code pour gérer la page manquante
}

// Exemple d'utilisation de la MMU
void example() {
    uint64_t virtual_address = 0x12345678;
    uint64_t physical_address = translate_address(virtual_address);
    uint8_t* data = (uint8_t*)physical_address;

}


// Fonction d'allocation de page
void* allocate_page() {
    void* page_address = (void*)next_physical_page;
    next_physical_page += PAGE_SIZE;
    return page_address;
}