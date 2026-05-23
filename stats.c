#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stats.h"

static void extraire_nom(char ligne[], char nom[]) {
    int i = 5, j = 0;
    while (ligne[i] != '\0' && ligne[i] != '\n') {
        nom[j++] = ligne[i++];
    }
    nom[j] = '\0';
}

static int extraire_nombre(char ligne[]) {
    char texte_nombre[20];
    int i = 0, j = 0;
    while (ligne[i] != ':' && ligne[i] != '\0') i++;
    if (ligne[i] == ':') i++;
    if (ligne[i] == ' ') i++;
    while (ligne[i] != '\0' && ligne[i] != '\n') {
        texte_nombre[j++] = ligne[i++];
    }
    texte_nombre[j] = '\0';
    return atoi(texte_nombre);
}

static int lire_joueur_fichier(FILE *fichier, Joueur *joueur) {
    char ligne[100];
    if (fgets(ligne, 100, fichier) == NULL) return 0;
    extraire_nom(ligne, joueur->nom);
    if (fgets(ligne, 100, fichier) == NULL) return 0;
    joueur->parties = extraire_nombre(ligne);
    if (fgets(ligne, 100, fichier) == NULL) return 0;
    joueur->victoires = extraire_nombre(ligne);
    if (fgets(ligne, 100, fichier) == NULL) return 0;
    joueur->cases_parcourues = extraire_nombre(ligne);
    fgets(ligne, 100, fichier);
    return 1;
}

int charger_historique(Joueur joueurs[]) {
    FILE *fichier = fopen("stats.txt", "r");
    int nb_joueurs = 0;
    if (fichier == NULL) return 0;
    while (nb_joueurs < MAX_HISTO && lire_joueur_fichier(fichier, &joueurs[nb_joueurs])) {
        nb_joueurs++;
    }
    fclose(fichier);
    return nb_joueurs;
}

void sauvegarder_historique(Joueur joueurs[], int nb_joueurs) {
    FILE *fichier = fopen("stats.txt", "w");
    int i;
    if (fichier == NULL) return;
    for (i = 0; i < nb_joueurs; i++) {
        fprintf(fichier, "Nom: %s\nParties: %d\nVictoires: %d\nCases: %d\n---\n",
                joueurs[i].nom, joueurs[i].parties, joueurs[i].victoires, joueurs[i].cases_parcourues);
    }
    fclose(fichier);
}

void maj_historique(Joueur historique[], int *nb_historique, char noms_partie[][MAX_NOM], int nb_joueurs_partie, char gagnant[], int cases_partie[]) {
    int i;
    for (i = 0; i < nb_joueurs_partie; i++) {
        int pos = -1, k;
        for (k = 0; k < *nb_historique; k++) {
            if (strcmp(historique[k].nom, noms_partie[i]) == 0) {
                pos = k;
                break;
            }
        }
        if (pos == -1 && *nb_historique < MAX_HISTO) {
            pos = *nb_historique;
            strncpy(historique[pos].nom, noms_partie[i], MAX_NOM);
            historique[pos].parties = 0;
            historique[pos].victoires = 0;
            historique[pos].cases_parcourues = 0;
            (*nb_historique)++;
        }
        if (pos != -1) {
            historique[pos].parties++;
            historique[pos].cases_parcourues += cases_partie[i];
            if (strcmp(noms_partie[i], gagnant) == 0) {
                historique[pos].victoires++;
            }
        }
    }
}

void afficher_classement(Joueur historique[], int nb_historique) {
    int i, j;
    if (nb_historique == 0) {
        printf("\n  Aucune donnée enregistrée dans le classement général actuellement.\n");
        return;
    }
    for (i = 0; i < nb_historique - 1; i++) {
        int m = i;
        for (j = i + 1; j < nb_historique; j++) {
            if (historique[j].victoires > historique[m].victoires) m = j;
        }
        if (m != i) {
            Joueur t = historique[i];
            historique[i] = historique[m];
            historique[m] = t;
        }
    }
    printf(CYAN GRAS "\n  ================= CLASSEMENT GÉNÉRAL =================\n" RESET);
    for (i = 0; i < nb_historique; i++) {
        printf("  %2d. %-15s | Victoires: %2d | Parties: %2d | Cases: %d\n",
               i + 1, historique[i].nom, historique[i].victoires, historique[i].parties, historique[i].cases_parcourues);
    }
}

void afficher_resume_partie(char noms_partie[][MAX_NOM], int nb_joueurs_partie, char gagnant[], int cases_partie[], int duree_sec) {
    int i;
    printf(JAUNE GRAS "\n  ========= RÉSUMÉ EXCLUSIF DE LA PARTIE =========\n" RESET);
    if (strlen(gagnant) > 0) {
        printf("  Triomphateur incontesté : " VERT GRAS "%s\n" RESET, gagnant);
    } else {
        printf("  Triomphateur incontesté : " ROUGE "Aucun (Tous les héros ont abandonné)\n" RESET);
    }
    printf("  Temps d'exploration écoulé : %02d min %02d sec\n\n", duree_sec / 60, duree_sec % 60);
    for (i = 0; i < nb_joueurs_partie; i++) {
        printf("  - %-15s : %3d cases arpentées au total\n", noms_partie[i], cases_partie[i]);
    }
    printf("  ================================================\n\n");
}
