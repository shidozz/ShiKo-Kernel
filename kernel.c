#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include "./include/paging.h"
#include "./include/serial.h"
#include "./include/ints.h"



void kmain(void) {
    print("Shidozz et Kokolor kernel!\n");
    initialize_page_table();
    print("PAGING TABLE Initialiser!\n");
    map_pages();
    print("MAP PAGE!\n");
    enable_pagination();
    print("PAGING Enabled!\n");
    
    // Configuration des vecteurs d'interruption et enregistrement des gestionnaires
    // Activation des interruptions
    enable_interrupts();
    // Boucle principale du kernel
    while (1) {
        // Code du kernel
        process_command();
        // Vérification et gestion des interruptions
        uint64_t interrupt_number = check_interrupt();
        if (interrupt_number != -1) {
            handle_interrupt(interrupt_number);
        }
    }
}
