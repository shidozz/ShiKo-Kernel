#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include "./include/paging.h"
#include "./include/serial.h"
#include "./include/ints.h"

void switch_to_supervisor_mode() {
    // Code d'assemblage pour passer en mode superviseur
    asm volatile(
        "mov x0, #0\n"            // Code d'appel système (0 pour basculer en mode superviseur)
        "mov x1, #0\n"            // Arguments (peuvent être utilisés selon les besoins)
        "mov x2, #0\n"            // Arguments (peuvent être utilisés selon les besoins)
        "mov x3, #0\n"            
        "mov x4, #0\n"          // Arguments (peuvent être utilisés selon les besoins)
        "hvc #0\n"               
    );
}

void kmain(void) {
    print("Shidozz et Kokolor kernel!\n");
    initialize_page_table();
    print("PAGING TABLE Initialiser!\n");
    map_pages();
    print("MAP PAGE!\n");
    enable_pagination();
    print("PAGING Enabled!\n");
    
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
