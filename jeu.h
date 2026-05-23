#ifndef JEU_H
#define JEU_H

#include "structures.h"

int  choisir_arme();
int  resoudre_combat(int arme, char monstre);
void logique_portail(int *joueur_x, int *joueur_y);
void logique_totem(Case plateau[5][5], int *x, int *y, int depx, int depy);
int  verifier_victoire(int *coffre, int *arme_trouvee, char case_actuelle, char arme_recherche);

#endif
