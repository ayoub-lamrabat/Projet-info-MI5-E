#include "utils.h"
#include <stdlib.h>
#include <string.h>

void effacer_ecran(void) {
    printf("\033[2J\033[H");
}

void vider_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void lire_nom(char nom[], char message[]) {
    int valide = 0;
    while (valide == 0) {
        printf("%s", message);
        if (fgets(nom, MAX_NOM, stdin) == NULL) {
            printf("Erreur de saisie.\n");
            continue;
        }
        if (nom[strlen(nom) - 1] == '\n') {
            nom[strlen(nom) - 1] = '\0';
        }
        if (strlen(nom) > 0) {
            valide = 1;
        } else {
            printf("Erreur : le nom ne peut pas etre vide.\n");
        }
    }
}

int lire_entier(char message[], int min, int max) {
    int valeur;
    printf("%s", message);
    while (scanf("%d", &valeur) != 1 || valeur < min || valeur > max) {
        vider_buffer();
        printf("Valeur invalide, entrez un nombre entre %d et %d : ", min, max);
    }
    vider_buffer(); 
    return valeur;
}
