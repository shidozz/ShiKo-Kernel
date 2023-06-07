#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include "../include/paging.h"
#include "../include/serial.h"
#include "../include/string.h"
// Déclaration des registres UART
#define UART_BASE 0x09000000
#define UART_DR   (UART_BASE + 0x00)
#define UART_FR   (UART_BASE + 0x18)


// Fonction pour écrire un caractère dans le terminal
void putchar(char c) {
    while (*(volatile unsigned int*)UART_FR & (1 << 5)) {}
    *(volatile unsigned int*)UART_DR = c;
}

// Fonction pour écrire une chaine caractères dans le terminal
void print(const char *s) {
    while (*s) {
        if (*s == '\r' || *s == '\n') {
            putchar('\r');
            putchar('\n');
            break;
        }else{
            putchar(*s++);
        }
    }
}

// Fonction pour lire un caractère depuis le terminal
char getc() {
    while (*(volatile unsigned int*)UART_FR & (1 << 4)) {}
    return *(volatile unsigned int*)UART_DR;
}

// Fonction pour lire une ligne depuis le terminal
void gets(char *buffer, int size) {
    char c;
    int count = 0;
    int cursor = 0; // Position du curseur dans le tampon
    
    while (1) {
        c = getc();
        
        if (c == '\r' || c == '\n') {
            putchar('\r');
            putchar('\n');
            break;
        }
        else if (c == '\b') { // Suppression du caractère précédent
            if (count > 0 && cursor > 0) {
                // Décalage des caractères à gauche pour supprimer le caractère à la position du curseur
                for (int i = cursor - 1; i < count; i++) {
                    buffer[i] = buffer[i + 1];
                }
                count--;
                cursor--;
                
                // Mise à jour de l'affichage après la suppression
                putchar('\b');
                print(" "); // Effacer l'affichage du caractère supprimé
                putchar('\b');
                
                // Réaffichage des caractères à droite de la position du curseur
                for (int i = cursor; i < count; i++) {
                    putchar(buffer[i]);
                }
                
                // Retour du curseur à la position correcte
                for (int i = count; i > cursor; i--) {
                    putchar('\b');
                }
            }
        }
        else if (c == 27) { // Gestion des touches de contrôle (par exemple, les flèches)
            // Votre code de gestion des touches de contrôle ici
            
            // Exemple de gestion de la touche "Flèche gauche" (code de contrôle ANSI : \x1b[D)
            if (getc() == '[' && getc() == 'D') {
                if (cursor > 0) {
                    putchar('\b');
                    cursor--;
                }
            }
            
            // Autres touches de contrôle à gérer selon vos besoins
            
        }
        else if (count < size - 1) { // Ajout d'un nouveau caractère
            // Décalage des caractères à droite pour insérer le nouveau caractère à la position du curseur
            for (int i = count; i > cursor; i--) {
                buffer[i] = buffer[i - 1];
            }
            
            buffer[cursor] = c;
            count++;
            cursor++;
            
            // Mise à jour de l'affichage après l'insertion
            for (int i = cursor - 1; i < count; i++) {
                putchar(buffer[i]);
            }
            
            // Retour du curseur à la position correcte
            for (int i = count; i > cursor; i--) {
                putchar('\b');
            }
        }
    }
    
    buffer[count] = '\0';
}
// Fonction pour afficher le prompt et traiter les commandes
void process_command() {
    char command[100];
    print("ShiKO Kernel > ");
    gets(command, sizeof(command));
    if (strcmp(command, "hello") == 0) {
        print("comment tu vas ?");
    }else if (strcmp(command, "help") == 0 || strcmp(command, "?") == 0 ){
        print("Available commands:\r\n");
        print(" - hello: Display a greeting message\r\n");
        print(" - help: Display available commands\r\n");
    }else{
        print("Unknown ");
        print(command);
        print(" command. Type 'help' for available commands.");
    }
    print("\r\n");
}
