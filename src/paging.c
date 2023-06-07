#include <stdint.h>
#include "../include/paging.h"
#include "../include/serial.h"

// Adresses virtuelles et physiques
#define VIRTUAL_BASE_ADDRESS 0x10000000
#define PHYSICAL_BASE_ADDRESS 0x20000000

// Taille de la page
#define PAGE_SIZE 4096

// Table des pages
#define PAGE_TABLE_ENTRIES 1024



// Table des pages
PageTableEntry page_table[PAGE_TABLE_ENTRIES] __attribute__((aligned(PAGE_SIZE)));

// Fonction d'initialisation de la table des pages
void initialize_page_table() {
    for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        // Initialiser les entrées de la table des pages
        page_table[i].valid = 0;
        page_table[i].address = 0;
    }
}

// Fonction de mapping des pages virtuelles sur les pages physiques
void map_pages() {
    for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        // Calculer l'adresse virtuelle et l'adresse physique correspondante
        uint64_t virtual_address = VIRTUAL_BASE_ADDRESS + i * PAGE_SIZE;
        uint64_t physical_address = PHYSICAL_BASE_ADDRESS + i * PAGE_SIZE;
        
        // Remplir l'entrée de la table des pages avec l'adresse physique correspondante
        page_table[i].valid = 1;
        page_table[i].address = physical_address;
    }
}

// Fonction d'activation de la pagination
void enable_pagination() {
    uint64_t page_table_address = (uint64_t)page_table;
    uint64_t sctlr;

    // Charger l'adresse de la table des pages dans le registre de contrôle (TTBR0_EL1)
    asm volatile("msr ttbr0_el1, %0" :: "r"(page_table_address));

    // Charger la valeur actuelle du registre de contrôle (SCTLR_EL1)
    asm volatile("mrs %0, sctlr_el1" : "=r"(sctlr));

    // Activer la pagination en modifiant les bits appropriés dans le registre de contrôle
    sctlr |= (1 << 0); // Bit 0 : MMU enable

    // Charger la nouvelle valeur du registre de contrôle
    asm volatile("msr sctlr_el1, %0" :: "r"(sctlr));

    // Effectuer un TLB flush pour purger les anciennes entrées du TLB
    asm volatile("tlbi vmalle1is");

    // Réinitialiser le cache d'instructions
    asm volatile("isb");
}
