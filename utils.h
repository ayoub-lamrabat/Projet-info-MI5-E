#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

// --- CONSTANTES ---
#define TAILLE      5    /* Taille du plateau de jeu (5x5) */
#define MAX_NOM     50   /* Longueur maximale d'un nom de joueur */
#define MAX_JOUEURS 4    /* Nombre maximum de joueurs par partie */
#define MAX_HISTO   100  /* Nombre maximum de joueurs dans l'historique sauvegardé */

#define MIROIR 3
#define TORCHE 4
#define HACHE  5
#define ARC    6         

#define VICTOIRE 1
#define DEFAITE  0
#define RIEN     2

// Codes couleur ANSI
#define RESET   "\033[0m"
#define GRAS    "\033[1m"   
#define ROUGE   "\033[31m"
#define VERT    "\033[32m"
#define JAUNE   "\033[33m"
#define BLEU    "\033[34m"
#define MAGENTA "\033[35m"  
#define CYAN    "\033[36m"
#define BLANC   "\033[37m"
#define FOND_NOIR "\033[40m" 

// --- STRUCTURES ---
typedef struct {
    char type;    // 'B' Basilic, 'Z' Zombie, 'T' Troll, 'H' Harpie, 'C' Coffre, etc.
    int visible;  // 0 = cachée, 1 = révélée
} Case;

typedef struct {
    int x, y;            // Position actuelle
    int x_dep, y_dep;    // Position de départ obligatoire
    char nom[MAX_NOM];
    int classe;
    int a_trouve_coffre; 
    int a_trouve_arme;   
    int nb_pas;          
    int actif;           // 1 = en jeu, 0 = a abandonné
} JoueurEnJeu;

typedef struct {
    char nom[MAX_NOM];     
    int  parties;          
    int  victoires;        
    int  cases_parcourues; 
} Joueur;

// --- FONCTIONS PROTOTYPES ---
void effacer_ecran(void);
void vider_buffer(void);
void lire_nom(char nom[], char message[]);
int lire_entier(char message[], int min, int max);

#endif
