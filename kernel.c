#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include "./include/mmu.h"
#include "./include/serial.h"
#include "./include/ints.h"



void kmain(void) {
    print("Shidozz et Kokolor kernel!\n");
    mmu_init();
    print("mmu initialiser!\n");
    // Configuration des vecteurs d'interruption et enregistrement des gestionnaires
    // Activation des interruptions
    enable_interrupts();
    while(1) process_command();
    /*
        // Boucle principale du kernel
        while (1) {
            // Code du kernel
            // Vérification et gestion des interruptions
            uint64_t interrupt_number = check_interrupt();
            if (interrupt_number != -1) {
                handle_interrupt(interrupt_number);
            }
        }
    */
}