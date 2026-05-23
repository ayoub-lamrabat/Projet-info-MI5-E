#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stats.h"
#include "utils.h"

/*
 * extraire_nom
 * Lit le nom depuis une ligne du fichier stats.txt qui a le format "Nom: prenom".
 * Le résultat est stocké dans nom[].
 */
void extraire_nom(char ligne[], char nom[]) {
    int i = 5, j = 0; // On saute les 5 premiers caractères : 'N','o','m',':',' '
    while (ligne[i] != '\0' && ligne[i] != '\n') {
        nom[j++] = ligne[i++];
    }
    nom[j] = '\0';
}

/*
 * extraire_nombre
 * Lit un entier depuis une ligne du format "Parties: 7" ou "Victoires: 3".
 * Avance dans la ligne jusqu'au ':', saute l'espace, puis lit les chiffres.
 * Retourne le nombre converti avec atoi().
 */
int extraire_nombre(char ligne[]) {
    char texte_nombre[20];
    int i = 0, j = 0;
    while (ligne[i] != ':' && ligne[i] != '\0') {
        i++;
    }
    if (ligne[i] == ':') {
        i++;
    }
    if (ligne[i] == ' ') {
        i++;
    }
    while (ligne[i] != '\0' && ligne[i] != '\n') {
        texte_nombre[j++] = ligne[i++];
    }
    texte_nombre[j] = '\0';
    return atoi(texte_nombre);
}

/*
 * lire_joueur_fichier
 * Lit les données d'un joueur depuis le fichier stats.txt ouvert.
 * Chaque joueur occupe exactement 5 lignes dans le fichier :
 * Retourne 1 si un joueur a été lu avec succès, 0 si fin de fichier.
 */
int lire_joueur_fichier(FILE *fichier, Joueur *joueur) {
    char ligne[100];
    if (fgets(ligne, 100, fichier) == NULL) {
        return 0; // Fin du fichier
    }
    extraire_nom(ligne, joueur->nom);
    if (fgets(ligne, 100, fichier) == NULL) {
        return 0;
    }
    joueur->parties = extraire_nombre(ligne);
    if (fgets(ligne, 100, fichier) == NULL) {
        return 0;
    }
    joueur->victoires = extraire_nombre(ligne);
    if (fgets(ligne, 100, fichier) == NULL) {
        return 0;
    }
    joueur->cases_parcourues = extraire_nombre(ligne);
    fgets(ligne, 100, fichier); // Lit et ignore la ligne "---" séparatrice

    return 1;
}

/*
 * chercher_joueur
 * Parcourt le tableau joueurs[] et cherche un joueur par son nom.
 * La comparaison est faite grace strcmp.
 * Retourne l'indice du joueur trouvé, ou -1 s'il n'existe pas.
 */
int chercher_joueur(Joueur joueurs[], int nb_joueurs, char nom[]) {
    int i;

    for (i = 0; i < nb_joueurs; i++) {
        if (strcmp(joueurs[i].nom, nom) == 0) {
            return i;
        }
    }

    return -1; // Joueur non trouvé
}

/*
 * ajouter_joueur
 * Cherche un joueur par nom dans l'historique. S'il n'existe pas encore,
 * il est ajouté à la fin du tableau avec des statistiques à zéro.
 */
int ajouter_joueur(Joueur joueurs[], int *nb_joueurs, char nom[]) {
    int pos = chercher_joueur(joueurs, *nb_joueurs, nom);
    if (pos == -1) { // Joueur inconnu : on le crée
        if (*nb_joueurs < MAX_HISTO) {
            pos = *nb_joueurs;
            strncpy(joueurs[pos].nom, nom, MAX_NOM - 1);
            joueurs[pos].nom[MAX_NOM - 1] = '\0';
            joueurs[pos].parties          = 0;
            joueurs[pos].victoires        = 0;
            joueurs[pos].cases_parcourues = 0;
            (*nb_joueurs)++;
        } else {
            printf("Erreur : historique plein.\n");
        }
    }
    return pos;
}

/*
 * charger_historique
 * Ouvre le fichier "stats.txt" et charge tous les joueurs enregistrés
 * dans le tableau joueurs[].
 */
int charger_historique(Joueur joueurs[]) {
    FILE *fichier = fopen("stats.txt", "r");
    int nb_joueurs = 0;
    if (fichier == NULL) {
        return 0;
    }
    while (nb_joueurs < MAX_HISTO && lire_joueur_fichier(fichier, &joueurs[nb_joueurs])) {
        nb_joueurs++;
    }
    fclose(fichier);
    return nb_joueurs;
}

/*
 * sauvegarder_historique
 * Écrase le fichier "stats.txt" et y réécrit tous les joueurs
 * avec leurs statistiques mises à jour.
 */
void sauvegarder_historique(Joueur joueurs[], int nb_joueurs) {
    FILE *fichier = fopen("stats.txt", "w");
    int i;

    if (fichier == NULL) {
        printf("Erreur : sauvegarde impossible.\n");
        return;
    }

    for (i = 0; i < nb_joueurs; i++) {
        fprintf(fichier, "Nom: %s\n",       joueurs[i].nom);
        fprintf(fichier, "Parties: %d\n",   joueurs[i].parties);
        fprintf(fichier, "Victoires: %d\n", joueurs[i].victoires);
        fprintf(fichier, "Cases: %d\n",     joueurs[i].cases_parcourues);
        fprintf(fichier, "---\n"); // Séparateur entre deux joueurs
    }

    fclose(fichier);
}

/*
 * maj_historique
 * Met à jour l'historique après une partie terminée :
 *   - Pour chaque joueur de la partie : incrémente parties et cases_parcourues
 *   - Pour le gagnant uniquement : incrémente victoires
 * Si un joueur n'existait pas encore dans l'historique, il est créé.
 */
void maj_historique(Joueur historique[], int *nb_historique,
                    char noms_partie[][MAX_NOM], int nb_joueurs_partie,
                    char gagnant[], int cases_partie[]) {
    int i, position;

    for (i = 0; i < nb_joueurs_partie; i++) {
        position = ajouter_joueur(historique, nb_historique, noms_partie[i]);
        if (position != -1) {
            historique[position].parties++;
            historique[position].cases_parcourues += cases_partie[i];

            if (strcmp(noms_partie[i], gagnant) == 0) {
                historique[position].victoires++;
            }
        }
    }
}

/*
 * joueur_superieur
 * Critère de comparaison pour le classement entre deux joueurs a et b.
 * Retourne 1 si a est meilleur que b, selon ces priorités
 */
int joueur_superieur(Joueur a, Joueur b) {
    if (a.victoires > b.victoires) {
        return 1;
    }

    if (a.victoires == b.victoires && a.parties < b.parties) {
        return 1;
    }

    if (a.victoires == b.victoires &&
        a.parties == b.parties &&
        a.cases_parcourues > b.cases_parcourues) {
        return 1;
    }

    return 0;
}

/*
 * trier_classement
 * Trie le tableau joueurs[] du meilleur au moins bon,
 * en utilisant l'algorithme de tri par sélection
 */
void trier_classement(Joueur joueurs[], int nb_joueurs) {
    int    i, j, meilleur;
    Joueur temp;

    for (i = 0; i < nb_joueurs - 1; i++) {
        meilleur = i;

        for (j = i + 1; j < nb_joueurs; j++) {
            if (joueur_superieur(joueurs[j], joueurs[meilleur])) {
                meilleur = j; // On a trouvé un meilleur candidat
            }
        }

        if (meilleur != i) { // Échange uniquement si nécessaire
            temp = joueurs[i];
            joueurs[i] = joueurs[meilleur];
            joueurs[meilleur] = temp;
        }
    }
}

/*
 * afficher_classement
 * Trie et affiche le classement général de tous les joueurs
 * qui ont joué sur cette machine.
 */
void afficher_classement(Joueur historique[], int nb_historique) {
    int i;

    if (nb_historique == 0) {
        printf("\n  Aucun joueur dans l'historique.\n");
        return;
    }

    trier_classement(historique, nb_historique);
    printf(CYAN GRAS "\n  ======= CLASSEMENT GENERAL =======\n" RESET);

    for (i = 0; i < nb_historique; i++) {
        if (i == 0) {
            printf(JAUNE "  1. " RESET); // Or
        } else if (i == 1) {
            printf(BLANC "  2. " RESET); // Argent
        } else if (i == 2) {
            printf(ROUGE "  3. " RESET); // Bronze
        } else {
            printf("  %d. ", i + 1);
        }

        printf("%-15s | Victoires: %d | Parties: %d | Cases: %d\n",
               historique[i].nom, historique[i].victoires,
               historique[i].parties, historique[i].cases_parcourues);
    }

    printf(CYAN GRAS "  ==================================\n\n" RESET);
}

/*
 * afficher_resume_partie
 * Affiche le bilan de la partie qui vient de se terminer
 */
void afficher_resume_partie(char noms_partie[][MAX_NOM], int nb_joueurs_partie,
                            char gagnant[], int cases_partie[], int duree_sec) {
    int i;
    int minutes  = duree_sec / 60;
    int secondes = duree_sec % 60;

    printf(JAUNE GRAS "\n  ========= RESUME DE PARTIE =========\n" RESET);

    if (strlen(gagnant) > 0) {
        printf("  Gagnant : " VERT GRAS "%s\n" RESET, gagnant);
    } else {
        printf("  Gagnant : " ROUGE "Aucun (Abandon general)\n" RESET);
    }

    printf("  Duree   : %02d min %02d sec\n\n", minutes, secondes);

    for (i = 0; i < nb_joueurs_partie; i++) {
        printf("  - %-15s : %d cases parcourues\n", noms_partie[i], cases_partie[i]);
    }

    printf(JAUNE GRAS "  ====================================\n\n" RESET);
}
