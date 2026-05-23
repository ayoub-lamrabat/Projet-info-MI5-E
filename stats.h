#ifndef STATS_H
#define STATS_H

#include "structures.h"

void extraire_nom(char ligne[], char nom[]);
int  extraire_nombre(char ligne[]);
int  lire_joueur_fichier(FILE *fichier, Joueur *joueur);
int  chercher_joueur(Joueur joueurs[], int nb_joueurs, char nom[]);
int  ajouter_joueur(Joueur joueurs[], int *nb_joueurs, char nom[]);
int  charger_historique(Joueur joueurs[]);
void sauvegarder_historique(Joueur joueurs[], int nb_joueurs);
void maj_historique(Joueur historique[], int *nb_historique,
                    char noms_partie[][MAX_NOM], int nb_joueurs_partie,
                    char gagnant[], int cases_partie[]);
int  joueur_superieur(Joueur a, Joueur b);
void trier_classement(Joueur joueurs[], int nb_joueurs);
void afficher_classement(Joueur historique[], int nb_historique);
void afficher_resume_partie(char noms_partie[][MAX_NOM], int nb_joueurs_partie,
                            char gagnant[], int cases_partie[], int duree_sec);

#endif
