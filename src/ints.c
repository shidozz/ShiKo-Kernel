#include <stdint.h>
#include "../include/ints.h"
#include "../include/serial.h"
#include "../include/mmu.h"

volatile uint64_t interrupt_counter = 0;

// Structure pour enregistrer les gestionnaires d'interruption
struct interrupt_vector {
    uint64_t interrupt_number;
    void (*handler)();
};

// Table des vecteurs d'interruption
struct interrupt_vector interrupt_table[] = {
    {0, timer_interrupt_handler},
    {1, device_interrupt_handler},
    {14, page_fault_handler},    // Interruption 14 : Page Fault
    {0x80, exception_handler}, 
    // Ajoutez d'autres entrées pour chaque type d'interruption
};
void config_interrupts(){
    interrupt_table[0].interrupt_number = 0;
    interrupt_table[0].handler = timer_interrupt_handler;
    interrupt_table[1].interrupt_number = 1;
    interrupt_table[1].handler = device_interrupt_handler;
    interrupt_table[14].interrupt_number = 14;
    interrupt_table[14].handler = page_fault_handler;
    interrupt_table[0x80].interrupt_number = 0x80;
    interrupt_table[0x80].handler = exception_handler;
}

// Fonction pour vérifier les interruptions
uint64_t check_interrupt() {
    // Lecture du registre d'interruption pour détecter l'interruption active
    uint64_t interrupt_number;
    asm volatile("mrs %0, daif" : "=r"(interrupt_number));

    // Vérification si une interruption est active
    if ((interrupt_number & 0x2) == 0) {
        // Bit 1 (IRQ) est effacé, indiquant qu'une interruption est active
        // Extraire le numéro d'interruption à partir des bits spécifiques à votre matériel
        // Exemple : interrupt_number & 0xFF pour les 8 bits les moins significatifs
        return interrupt_number & 0xFF;  // Numéro d'interruption détecté
    }

    // Aucune interruption active
    return -1;
}

// Fonction pour activer les interruptions
void enable_interrupts() {
    asm volatile("msr daifclr, #2");
    config_interrupts();
}

// Fonction pour désactiver les interruptions
void disable_interrupts() {
    asm volatile("msr daifset, #2");
}

// Fonction pour gérer les interruptions
void handle_interrupt(uint64_t interrupt_number) {
    // Recherche du gestionnaire d'interruption correspondant dans la table
    int num_handlers = sizeof(interrupt_table) / sizeof(struct interrupt_vector);
    for (int i = 0; i < num_handlers; i++) {
        if (interrupt_table[i].interrupt_number == interrupt_number) {
            // Appel du gestionnaire d'interruption
            interrupt_table[i].handler();
            return;
        }
    }
}

// Gestionnaire d'interruption du minuteur
void timer_interrupt_handler() {
    // Code pour gérer l'interruption du minuteur
    interrupt_counter++;
    print("timer_interrupt\n");
}

// Gestionnaire d'interruption du périphérique
void device_interrupt_handler() {
    // Code pour gérer l'interruption du périphérique
    print("device_interrupt\n");
}

// Gestionnaire d'interruption de Page Fault
void page_fault_handler() {
    // Code pour gérer l'interruption de Page Fault
    print("page_fault_interrupt\n");
}
// Gestionnaire d'interruption d'exception
void exception_handler() {
    // Code pour gérer l'interruption d'exception
    print("exception_interrupt\n");
}


// Adresse de base du registre du timer
#define TIMER_BASE_ADDRESS 0x12345678

// Décalage des registres du timer
#define TIMER_CONTROL_OFFSET 0x00
#define TIMER_LOAD_OFFSET 0x04
#define TIMER_VALUE_OFFSET 0x08
#define TIMER_IRQ_OFFSET 0x0C

// Masques de bits pour le registre de contrôle du timer
#define TIMER_CONTROL_ENABLE (1 << 0)
#define TIMER_CONTROL_PERIODIC (1 << 1)

// Fonction pour initialiser la minuterie
void init_timer() {
    // Adresse de base du registre du timer
    volatile uint32_t* timer_base = (uint32_t*)TIMER_BASE_ADDRESS;

    // Configuration du registre de contrôle du timer
    timer_base[TIMER_CONTROL_OFFSET] = TIMER_CONTROL_ENABLE | TIMER_CONTROL_PERIODIC;

    // Configuration de la valeur de chargement du timer
    timer_base[TIMER_LOAD_OFFSET] = 1000;  // Valeur de chargement de la minuterie (par exemple, 1000)

    // Configuration de l'interruption du timer (facultatif)
    timer_base[TIMER_IRQ_OFFSET] = 1;  // Activer l'interruption du timer (1 pour activer, 0 pour désactiver)
}