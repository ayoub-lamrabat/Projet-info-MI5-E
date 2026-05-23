#include <stdio.h>
#include "jeu.h"
#include "jeu.h"
#include "utils.h"

/*
 * choisir_arme
 * Affiche le menu des 4 armes disponibles et demande au joueur
 * de choisir en tapant 3, 4, 5 ou 6. La saisie est répétée
 */
int choisir_arme() {
    int choix;
    printf(CYAN "\n  Choisissez votre arme pour ce deplacement :\n" RESET);
    printf("  " JAUNE "[3]" RESET " Miroir    -> contre le Basilic\n");
    printf("  " JAUNE "[4]" RESET " Torche    -> contre le Zombie\n");
    printf("  " JAUNE "[5]" RESET " Hache     -> contre le Troll\n");
    printf("  " JAUNE "[6]" RESET " Arc       -> contre la Harpie\n");
    printf("  Votre choix : ");
    do {
        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            choix = 0;
        }
        if (choix < 3 || choix > 6) {
            printf(ROUGE "  Nombre invalide ! Entrez 3, 4, 5 ou 6 : " RESET);
        }
    } while (choix < 3 || choix > 6);
    vider_buffer();
    return choix;
}

/*
 * resoudre_combat
 * Compare l'arme choisie par le joueur avec le type de monstre rencontré.
 * Chaque monstre n'est vulnérable qu'à une seule arme :
 */
int resoudre_combat(int arme, char monstre) {
    if (monstre == 'B') { // Basilic
        if (arme == MIROIR) {
            printf(VERT "  Victoire contre le Basilic !\n" RESET);
            return VICTOIRE;
        } else {
            printf(ROUGE "  Petrifie par le Basilic... Il fallait le Miroir !\n" RESET);
            return DEFAITE;
        }
    } else if (monstre == 'Z') { // Zombie
        if (arme == TORCHE) {
            printf(VERT "  Zombie vaincu !\n" RESET);
            return VICTOIRE;
        } else {
            printf(ROUGE "  Devore par le Zombie... Il fallait la Torche !\n" RESET);
            return DEFAITE;
        }
    } else if (monstre == 'T') { // Troll
        if (arme == HACHE) {
            printf(VERT "  Troll vaincu !\n" RESET);
            return VICTOIRE;
        } else {
            printf(ROUGE "  Le Troll vous tue... Il fallait la Hache !\n" RESET);
            return DEFAITE;
        }
    } else if (monstre == 'H') { // Harpie
        if (arme == ARC) {
            printf(VERT "  Harpie vaincue !\n" RESET);
            return VICTOIRE;
        } else {
            printf(ROUGE "  La Harpie vous tue... Il fallait l'Arc !\n" RESET);
            return DEFAITE;
        }
    }
    return RIEN;
}

/*
 * logique_portail
 * Gère l'effet du Portail Magique ('P') :
 * Le joueur choisit n'importe quelle case du plateau (ligne 0-4, colonne 0-4)
 * et y est téléporté immédiatement.
 *
 * Cette fonction modifie seulement la position du joueur.
 * La révélation et l'effet de la case d'arrivée sont gérés dans la boucle de jeu.
 */
void logique_portail(int *joueur_x, int *joueur_y) {
    int ligne;
    int colonne;

    printf(MAGENTA "  Portail Magique ! Choisissez une case cible.\n" RESET);
    do {
        printf("  Nouvelle ligne (0-4) : ");

        if (scanf("%d", &ligne) != 1) {
            vider_buffer();
            ligne = -1;
        }
        printf("  Nouvelle colonne (0-4) : ");
        if (scanf("%d", &colonne) != 1) {
            vider_buffer();
            colonne = -1;
        }
        if (ligne < 0 || ligne > 4 || colonne < 0 || colonne > 4) {
            printf(ROUGE "  Coordonnees invalides ! Ligne et colonne doivent etre entre 0 et 4.\n" RESET);
        }
    } while (ligne < 0 || ligne > 4 || colonne < 0 || colonne > 4);
    vider_buffer();
    *joueur_x = colonne;
    *joueur_y = ligne;
    printf(MAGENTA "  Teleportation en [ligne %d][col %d] !\n" RESET, ligne, colonne);
}

/*
 * logique_totem
 * Gère l'effet du Totem de transmutation ('M') :
 *   1. Le joueur choisit une case cachée n'importe où sur le plateau.
 *   2. Le totem et cette case échangent leurs contenus (sans les révéler).
 *   3. Le tour se termine : le joueur retourne à sa position de départ
 *      et TOUT le plateau est remis face cachée.
 */
void logique_totem(Case plateau[5][5], int *x, int *y, int depx, int depy) {
    int  cx, cy; // Coordonnées de la case cible choisie par le joueur
    Case temp;   // Variable temporaire pour l'échange des deux cases
    printf(MAGENTA "  Totem de transmutation ! Choisissez une case a echanger.\n" RESET);
    do {
        printf("  Ligne cible (0-4) : ");
        if (scanf("%d", &cy) != 1) {
            vider_buffer();
            cy = -1;
        }
        printf("  Colonne cible (0-4) : ");

        if (scanf("%d", &cx) != 1) {
            vider_buffer();
            cx = -1;
        }
        if (cx < 0 || cx > 4 || cy < 0 || cy > 4) {
            printf(ROUGE "  Coordonnees invalides !\n" RESET);
        }
    } while (cx < 0 || cx > 4 || cy < 0 || cy > 4);
    vider_buffer();
    temp = plateau[*y][*x];
    plateau[*y][*x] = plateau[cy][cx];
    plateau[cy][cx] = temp;
    // Les deux cases échangées redeviennent cachées : personne ne sait ce qu'elles contiennent
    plateau[*y][*x].visible = 0;
    plateau[cy][cx].visible = 0;

    // Le joueur est renvoyé à sa position de départ
    *x = depx;
    *y = depy;

    printf(MAGENTA "  Echange effectue ! Retour au depart.\n" RESET);
}

/*
 * verifier_victoire
 * Vérifie si la case sur laquelle vient d'arriver le joueur lui permet
 * de progresser vers la victoire, ou de gagner définitivement.
 */
int verifier_victoire(int *coffre, int *arme_trouvee, char case_actuelle, char arme_recherche) {
    if (case_actuelle == 'C') {
        printf(JAUNE "  Coffre trouve !\n" RESET);
        *coffre = 1;
        if (*arme_trouvee == 0) {
            printf("  Il vous manque encore votre arme antique...\n");
        }
    }
    if (case_actuelle == arme_recherche) {
        if (case_actuelle == 'E') {
            printf(VERT "  Epee de feu trouvee !\n" RESET);
        } else if (case_actuelle == 'F') {
            printf(VERT "  Baton familier trouve !\n" RESET);
        } else if (case_actuelle == 'G') {
            printf(VERT "  Grimoire interdit trouve !\n" RESET);
        } else if (case_actuelle == 'D') {
            printf(VERT "  Dague du sommeil trouvee !\n" RESET);
        }
        *arme_trouvee = 1;
        if (*coffre == 0) {
            printf("  Il vous manque encore un coffre au tresor...\n");
        }
    }
    if ((case_actuelle == 'E' || case_actuelle == 'F' ||case_actuelle == 'G' || case_actuelle == 'D') &&case_actuelle != arme_recherche) {
        printf(JAUNE "  Arme antique trouvee, mais ce n'est pas la votre !\n" RESET);
    }

    if (*coffre == 1 && *arme_trouvee == 1) {
        return 1;
    } else {
        return 0;
    }
}
