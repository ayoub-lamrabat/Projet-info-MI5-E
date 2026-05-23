#include <stdio.h>
#include <string.h>
#include "utils.h"

/*
 * effacer_ecran
 * Efface tout le contenu visible du terminal et place le curseur
 * en haut à gauche. Utilisé avant chaque affichage du plateau
 * pour éviter que les anciens affichages s'accumulent.
 */
void effacer_ecran() {
    printf("\033[2J\033[H");
}

/*
 * vider_buffer
 * Après un scanf(), les caractères restants dans le buffer clavier
 * (notamment le '\n' final) peuvent perturber la prochaine saisie.
 * Cette fonction lit et jette tous ces caractères jusqu'à la fin de ligne.
 */
void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/*
 * lire_nom
 * Affiche le message d'invite puis lit un nom de joueur dans nom[].
 */
void lire_nom(char nom[], char message[]) {
    int valide = 0;
    while (valide == 0) {
        printf("%s", message);
        if (fgets(nom, MAX_NOM, stdin) == NULL) {
            printf("Erreur de saisie.\n");
            continue;
        }
        if (strlen(nom) > 0 && nom[strlen(nom) - 1] == '\n') {
            nom[strlen(nom) - 1] = '\0';
        }
        if (strlen(nom) > 0) {
            valide = 1;
        } else {
            printf("Erreur : le nom ne peut pas etre vide.\n");
        }
    }
}

/*
 * lire_entier
 * Affiche le message d'invite et lit un entier compris entre min et max.
 */
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
