#ifndef STATS_H
#define STATS_H

#include "structures.h"

int charger_historique(Joueur joueurs[]);
void sauvegarder_historique(Joueur joueurs[], int nb_joueurs);
void maj_historique(Joueur historique[], int *nb_historique, char noms_partie[][MAX_NOM], int nb_joueurs_partie, char gagnant[], int cases_partie[]);
void afficher_classement(Joueur historique[], int nb_historique);
void afficher_resume_partie(char noms_partie[][MAX_NOM], int nb_joueurs_partie, char gagnant[], int cases_partie[], int duree_sec);

#endif
