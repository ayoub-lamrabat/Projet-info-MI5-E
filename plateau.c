#include <stdio.h>
#include <stdlib.h>
#include "plateau.h"
#include "utils.h"

void remplir_cartes(char carte[25]) {
    int i;
    for (i = 0; i < 4; i++)  carte[i] = 'B';
    for (i = 4; i < 8; i++)  carte[i] = 'T';
    for (i = 8; i < 12; i++) carte[i] = 'Z';
    for (i = 12; i < 16; i++) carte[i] = 'H';
    for (i = 16; i < 18; i++) carte[i] = 'C';

    carte[18] = 'E'; // Épée de feu
    carte[19] = 'F'; // Bâton de contrôle
    carte[20] = 'G'; // Grimoire interdit
    carte[21] = 'D'; // Dague de sommeil
    carte[22] = 'P'; // Portail magique
    carte[23] = 'M'; // Totem de transmutation
    carte[24] = 'M';
}

void melanger_carte(char carte[], int n) {
    int i;
    for (i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = carte[i];
        carte[i] = carte[j];
        carte[j] = temp;
    }
}

void remplir_plateau(char carte[25], Case plateau[TAILLE][TAILLE]) {
    int a = 0;
    int i, j;
    for (i = 0; i < TAILLE; i++) {
        for (j = 0; j < TAILLE; j++) {
            plateau[i][j].type = carte[a];
            plateau[i][j].visible = 0;
            a++;
        }
    }
}

void cacher_plateau(Case plateau[TAILLE][TAILLE]) {
    int r, c;
    for (r = 0; r < TAILLE; r++) {
        for (c = 0; c < TAILLE; c++) {
            plateau[r][c].visible = 0;
        }
    }
}

void init_positions_depart(JoueurEnJeu tab_j[], int nb_joueurs) {
    int i;
    tab_j[0].x_dep = 0; tab_j[0].y_dep = -1;
    if (nb_joueurs > 1) { 
        tab_j[1].x_dep = 4; 
        tab_j[1].y_dep = 5; }
    if (nb_joueurs > 2) { tab_j[2].x_dep = 4; tab_j[2].y_dep = -1; }
if (nb_joueurs > 3) {
    tab_j[3].x_dep = 0; 
    tab_j[3].y_dep = 5; }
    for (i = 0; i < nb_joueurs; i++) {
        tab_j[i].x = tab_j[i].x_dep;
        tab_j[i].y = tab_j[i].y_dep;
    }
}

void afficher_statut_joueur(JoueurEnJeu *j, int idx) {
    char *couleurs[4] = {CYAN, JAUNE, VERT, MAGENTA};
    printf(GRAS "%s  J%d: %-12s" RESET, couleurs[idx], idx + 1, j->nom);

    if      (j->class_j == 0) printf("Guerrier ");
    else if (j->class_j == 1) printf("Ranger   ");
    else if (j->class_j == 2) printf("Magicien ");
    else if (j->class_j == 3) printf("Voleur   ");

    printf("| Coffre: ");
    if (j->a_trouve_coffre) printf(VERT "OUI" RESET);
    else                    printf(ROUGE "NON" RESET);

    printf(" | Arme: ");
    if (j->a_trouve_arme) printf(VERT "OUI" RESET);
    else                  printf(ROUGE "NON" RESET);

    printf(" | Pas: %d", j->nb_pas);
    if (j->actif == 0) printf(ROUGE " [QUITTE]" RESET);
    printf("\n");
}

void afficher_plateau(Case plateau[TAILLE][TAILLE], JoueurEnJeu tab_j[], int nb_joueurs, int idx_actuel) {
    int i, j, k;
    JoueurEnJeu *j_actuel = &tab_j[idx_actuel];

    effacer_ecran();

    printf(CYAN GRAS "  +===========================================+\n" RESET);
    printf(CYAN GRAS "  |" RESET BLANC GRAS "          MEMO-RPG  -  LE DONJON           " CYAN GRAS "|\n" RESET);
    printf(CYAN GRAS "  +===========================================+\n\n" RESET);

    for (k = 0; k < nb_joueurs; k++) {
        printf("  ");
        afficher_statut_joueur(&tab_j[k], k);
    }
    printf("\n");

    printf(GRAS "  >> Tour de : " RESET);
    printf(JAUNE GRAS "%s\n\n" RESET, j_actuel->nom);

    int j_nord = -1;
    for (k = 0; k < nb_joueurs; k++) {
        if (tab_j[k].actif && tab_j[k].y == -1 && tab_j[k].x == 2) { j_nord = k; break; }
    }
    if (j_nord != -1) {
        char *couleurs[4] = {CYAN, JAUNE, VERT, MAGENTA};
        printf("                        %s[J%d]" RESET "\n\n", couleurs[j_nord], j_nord + 1);
    } else {
        printf("\n\n");
    }

    printf("           0     1     2     3     4\n");
    printf("        +-----+-----+-----+-----+-----+\n");

    for (i = 0; i < TAILLE; i++) {
        int j_ouest = -1;
        if (i == 2) {
            for (k = 0; k < nb_joueurs; k++) {
                if (tab_j[k].actif && tab_j[k].x == -1 && tab_j[k].y == 2) { j_ouest = k; break; }
            }
        }

        if (j_ouest != -1) {
            char *couleurs[4] = {CYAN, JAUNE, VERT, MAGENTA};
            printf("  %s[J%d]" RESET " %d |", couleurs[j_ouest], j_ouest + 1, i);
        } else {
            printf("       %d |", i);
        }

        for (j = 0; j < TAILLE; j++) {
            int joueur_ici = -1;
            for (k = 0; k < nb_joueurs; k++) {
                if (tab_j[k].actif && tab_j[k].y == i && tab_j[k].x == j) { joueur_ici = k; break; }
            }

            if (joueur_ici != -1) {
                char *couleurs[4] = {CYAN, JAUNE, VERT, MAGENTA};
                printf(GRAS "%s J%d  " RESET, couleurs[joueur_ici], joueur_ici + 1);
            } else if (plateau[i][j].visible == 1) {
                char t = plateau[i][j].type;
                switch (t) {
                    case 'B': printf(ROUGE " Bs  " RESET); break;
                    case 'Z': printf(ROUGE " Zm  " RESET); break;
                    case 'T': printf(ROUGE " Tr  " RESET); break;
                    case 'H': printf(ROUGE " Hp  " RESET); break;
                    case 'E': printf(VERT  " Ep  " RESET); break;
                    case 'F': printf(VERT  " Bt  " RESET); break;
                    case 'G': printf(VERT  " Gr  " RESET); break;
                    case 'D': printf(VERT  " Dg  " RESET); break;
                    case 'C': printf(JAUNE " Co  " RESET); break;
                    case 'M': printf(MAGENTA " To  " RESET); break;
                    case 'P': printf(MAGENTA " Po  " RESET); break;
                    default:  printf(" [?] "); break;
                }
            } else {
                printf(FOND_NOIR BLANC " ~~~ " RESET);
            }
            printf("|");
        }

        int j_est = -1;
        if (i == 2) {
            for (k = 0; k < nb_joueurs; k++) {
                if (tab_j[k].actif && tab_j[k].x == 5 && tab_j[k].y == 2) { j_est = k; break; }
            }
        }
        if (j_est != -1) {
            char *couleurs[4] = {CYAN, JAUNE, VERT, MAGENTA};
            printf(" %s[J%d]" RESET, couleurs[j_est], j_est + 1);
        }

        printf("\n");
        printf("        +-----+-----+-----+-----+-----+\n");
    }

    int j_sud = -1;
    for (k = 0; k < nb_joueurs; k++) {
        if (tab_j[k].actif && tab_j[k].y == 5 && tab_j[k].x == 2) { j_sud = k; break; }
    }
    if (j_sud != -1) {
        char *couleurs[4] = {CYAN, JAUNE, VERT, MAGENTA};
        printf("\n                        %s[J%d]" RESET "\n", couleurs[j_sud], j_sud + 1);
    } else {
        printf("\n");
    }

    if (j_actuel->y == j_actuel->y_dep && j_actuel->x == j_actuel->x_dep) {
        printf(JAUNE "  Vous etes aux portes du donjon. Entrez sur le plateau !\n" RESET);
    }
}

void afficher_plateau_final(Case plateau[TAILLE][TAILLE]) {
    int i, j;
    printf(CYAN GRAS "\n  Voici le contenu intégral du donjon décrypté :\n\n" RESET);
    printf("           0     1     2     3     4\n");
    printf("        +-----+-----+-----+-----+-----+\n");

    for (i = 0; i < TAILLE; i++) {
        printf("       %d |", i);
        for (j = 0; j < TAILLE; j++) {
            char t = plateau[i][j].type;
            switch (t) {
                case 'B': printf(ROUGE " Bs  " RESET); break;
                case 'Z': printf(ROUGE " Zm  " RESET); break;
                case 'T': printf(ROUGE " Tr  " RESET); break;
                case 'H': printf(ROUGE " Hp  " RESET); break;
                case 'E': printf(VERT   " Ep  " RESET); break;
                case 'F': printf(VERT   " Bt  " RESET); break;
                case 'G': printf(VERT   " Gr  " RESET); break;
                case 'D': printf(VERT   " Dg  " RESET); break;
                case 'C': printf(JAUNE  " Co  " RESET); break;
                case 'M': printf(MAGENTA " To  " RESET); break;
                case 'P': printf(MAGENTA " Po  " RESET); break;
                default:  printf(" [?] "); break;
            }
            printf("|");
        }
        printf("\n");
        printf("        +-----+-----+-----+-----+-----+\n");
    }
    printf("\n");
}
