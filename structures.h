typedef struct {
    char type;    // 'B' Basilic, 'Z' Zombie, 'T' Troll, 'H' Harpie, 'E' Epée, etc.
    int visible;  // 0 = cachée (brouillard), 1 = révélée
} Case;

typedef struct {
    int x, y;            // Position actuelle
    int x_dep, y_dep;    // Position de départ
    char nom[];
    int classe;
    int a_trouve_coffre; // Au début = 0 pour tous les joueurs.
    int a_trouve_arme;   // Au début = 0.
    int nb_pas;          // Pour les statistiques (Étudiant C)
} Joueur;
