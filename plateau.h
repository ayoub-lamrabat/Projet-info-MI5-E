#ifndef PLATEAU_H
#define PLATEAU_H

#include "structures.h"

void remplir_cartes(char carte[25]);
void melanger_carte(char carte[], int n);
void remplir_plateau(char carte[25], Case plateau[TAILLE][TAILLE]);
void cacher_plateau(Case plateau[TAILLE][TAILLE]);
void init_positions_depart(JoueurEnJeu tab_j[], int nb_joueurs);
void afficher_statut_joueur(JoueurEnJeu *j, int idx);
void afficher_plateau(Case plateau[TAILLE][TAILLE], JoueurEnJeu tab_j[], int nb_joueurs, int idx_actuel);
void afficher_plateau_final(Case plateau[TAILLE][TAILLE]);

#endif
