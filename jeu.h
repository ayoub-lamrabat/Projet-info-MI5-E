#ifndef JEU_H
#define JEU_H

#include "utils.h"

int choisir_arme(void);
int resoudre_combat(int arme, char monstre);
void logique_portail(int *joueur_x, int *joueur_y);
void logique_totem(Case plateau[5][5], int *x, int *y, int depx, int depy);
int verifier_victoire(int *coffre, int *arme_trouvee, char case_actuelle, char arme_recherche);

int charger_historique(Joueur joueurs[]);
void sauvegarder_historique(Joueur joueurs[], int nb_joueurs);
void maj_historique(Joueur historique[], int *nb_historique, char noms_partie[][MAX_NOM], int nb_joueurs_partie, char gagnant[], int cases_partie[]);
void afficher_classement(Joueur historique[], int nb_historique);
void afficher_resume_partie(char noms_partie[][MAX_NOM], int nb_joueurs_partie, char gagnant[], int cases_partie[], int duree_sec);

#endif
