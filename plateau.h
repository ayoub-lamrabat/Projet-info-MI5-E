#ifndef PLATEAU_H
#define PLATEAU_H

#include "utils.h"

void remplir_cartes(char carte[25]);
void melanger_carte(char carte[], int n);
void remplir_plateau(char carte[25], Case plateau[5][5]);
void cacher_plateau(Case plateau[5][5]);
void afficher_statut_joueur(JoueurEnJeu *j, int idx);
void afficher_plateau(Case plateau[TAILLE][TAILLE], JoueurEnJeu tab_j[], int nb_joueurs, int idx_actuel);
void afficher_plateau_final(Case plateau[TAILLE][TAILLE]);
void init_positions_depart(JoueurEnJeu tab_j[], int nb_joueurs);

#endif
