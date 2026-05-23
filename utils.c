#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "structures.h"

// Efface le terminal proprement
void effacer_ecran(void) {
    printf("\033[2J\033[H");
}

// Vide la mémoire tampon du clavier
void vider_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Sécurise la saisie de texte
void lire_nom(char nom[], char message[]) {
    int valide = 0;
    while (valide == 0) {
        printf("%s", message);
        if (fgets(nom, MAX_NOM, stdin) == NULL) {
            printf("  Erreur critique lors de la lecture.\n");
            continue;
        }
        if (nom[strlen(nom) - 1] == '\n') {
            nom[strlen(nom) - 1] = '\0';
        }
        if (strlen(nom) > 0) {
            valide = 1;
        } else {
            printf(ROUGE "  Erreur : Le nom ne peut pas être vide !\n" RESET);
        }
    }
}

// Sécurise la saisie de nombres entiers
int lire_entier(char message[], int min, int max) {
    int valeur;
    printf("%s", message);
    while (scanf("%d", &valeur) != 1 || valeur < min || valeur > max) {
        vider_buffer();
        printf(ROUGE "  Saisie invalide ! Veuillez entrer un nombre entre %d et %d : " RESET, min, max);
    }
    vider_buffer();
    return valeur;
}
