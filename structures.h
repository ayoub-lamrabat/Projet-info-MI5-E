#ifndef STRUCTURES_H
#define STRUCTURES_H

// ============================================================
//  CONSTANTES GLOBALES
// ============================================================
#define TAILLE      5
#define MAX_NOM     50
#define MAX_JOUEURS 4
#define MAX_HISTO   100  // Nombre maximum de joueurs différents mémorisés dans le fichier de stats

#define MIROIR 3
#define TORCHE 4
#define HACHE  5
#define ARC    6

#define VICTOIRE 1
#define DEFAITE  0
#define RIEN     2

// Codes couleur ANSI : permettent d'afficher du texte coloré dans le terminal
#define RESET     "\033[0m"   // Remet le style par défaut (annule couleur et gras)
#define GRAS      "\033[1m"   // Correction : suppression de l'antislash fautif
#define ROUGE     "\033[31m"
#define VERT      "\033[32m"
#define JAUNE     "\033[33m"
#define BLEU      "\033[34m"
#define MAGENTA   "\033[35m"
#define CYAN      "\033[36m"
#define BLANC     "\033[37m"
#define FOND_NOIR "\033[40m"  // Fond noir, utile pour simuler le brouillard de guerre '~~~'

// ============================================================
//  STRUCTURES DE DONNÉES
// ============================================================

// Représente une case individuelle du labyrinthe 5x5
typedef struct {
    char type;     // 'B'=Basilic, 'Z'=Zombie, 'T'=Troll, 'H'=Harpie, 'C'=Coffre, 'E'/'F'/'G'/'D'=Armes, 'P'=Portail, 'M'=Totem
    int visible;   // 0 = cachée (brouillard), 1 = révélée et affichée à l'écran
} Case;

// Données d'un joueur spécifiques à la partie en cours
typedef struct {
    int x, y;             // Position actuelle sur le plateau (ou coordonnées hors-plateau au départ)
    int x_dep, y_dep;     // Coordonnées de départ initiales (mémorisées pour y renvoyer le joueur en cas de défaite)
    char nom[MAX_NOM];    // Nom de l'aventurier
    int class_j;          // Renommé 'class_j' car 'classe' peut être un mot réservé sur certains compilateurs
    int a_trouve_coffre;  // Boolean (0 ou 1) : le joueur a ramassé au moins un trésor
    int a_trouve_arme;    // Boolean (0 ou 1) : le joueur a trouvé SON arme antique fétiche
    int nb_pas;           // Compteur du nombre total de cases explorées (statistique de fin de partie)
    int actif;            // 1 = joue toujours, 0 = a déclaré forfait / quitté prématurément
} JoueurEnJeu;

// Données d'un joueur destinées à être sauvegardées à long terme dans le fichier texte
typedef struct {
    char nom[MAX_NOM];
    int parties;          // Nombre de parties jouées au total
    int victoires;        // Nombre de victoires enregistrées
    int cases_parcourues; // Cumul de toutes les cases explorées à travers toutes les parties
} Joueur;

#endif
