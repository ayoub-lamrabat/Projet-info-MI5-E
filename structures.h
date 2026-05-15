#ifndef STRUCTURES_H // si STRUCTURE_H n'a pas été vu
#define STRUCTURES_H // je le note et je lis la suite, si c'est deja défini, on saute cette partie.
typedef struct {
    char type;    // 'B' Basilic, 'Z' Zombie, 'T' Troll, 'H' Harpie, 'E' Epée, etc.
    int visible;  // 0 = cachée (brouillard), 1 = révélée
} Case;

typedef struct {
    int x, y;            // Position actuelle
    int x_dep, y_dep;    // Position de départ
    char nom[50];
    int classe;
    int a_trouve_coffre; // Au début = 0 pour tous les joueurs.
    int a_trouve_arme;   // Au début = 0.
    int nb_pas;          // Pour les statistiques (Étudiant C)
} Joueur;

#endif // fin de la protection
