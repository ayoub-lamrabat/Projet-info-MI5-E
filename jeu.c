#include <stdio.h>
#include "jeu.h"
#include "utils.h"
#include "structures.h"

int choisir_arme(void) {
    int choix;
    printf(CYAN "\n  Sélectionnez votre équipement protecteur pour cette avancée :\n" RESET);
    printf("  " JAUNE "[3]" RESET " Bouclier Réfléchissant -> Nécessaire contre le Basilic\n");
    printf("  " JAUNE "[4]" RESET " Torche Enflammée       -> Nécessaire contre le Zombie\n");
    printf("  " JAUNE "[5]" RESET " Hache Double en Pierre -> Nécessaire contre le Troll\n");
    printf("  " JAUNE "[6]" RESET " Arc Long de Précision  -> Nécessaire contre la Harpie\n");
    printf("  Votre choix tactique : ");

    do {
        if (scanf("%d", &choix) != 1) {
            while (getchar() != '\n');
            choix = 0;
        }
        if (choix < 3 || choix > 6) {
            printf(ROUGE "  Équipement inconnu ! Choisissez entre 3, 4, 5 ou 6 : " RESET);
        }
    } while (choix < 3 || choix > 6);
    vider_buffer();
    return choix;
}

int resoudre_combat(int arme, char monstre) {
    if (monstre == 'B') {
        if (arme == MIROIR) { printf(VERT "  Combat Réussi ! Le regard du Basilic s'est retourné contre lui !\n" RESET); return VICTOIRE; }
        else { printf(ROUGE "  Défaite Fatale ! Vous êtes pétrifié par le Basilic... Il fallait le Bouclier (3) !\n" RESET); return DEFAITE; }
    } else if (monstre == 'Z') {
        if (arme == TORCHE) { printf(VERT "  Combat Réussi ! Le Zombie est réduit en cendres par les flammes !\n" RESET); return VICTOIRE; }
        else { printf(ROUGE "  Défaite Fatale ! Dévoré par le Zombie... Il fallait la Torche (4) !\n" RESET); return DEFAITE; }
    } else if (monstre == 'T') {
        if (arme == HACHE) { printf(VERT "  Combat Réussi ! La cuirasse du Troll est brisée par votre hache !\n" RESET); return VICTOIRE; }
        else { printf(ROUGE "  Défaite Fatale ! Le Troll vous écrase... Il fallait la Hache double (5) !\n" RESET); return DEFAITE; }
    } else if (monstre == 'H') {
        if (arme == ARC) { printf(VERT "  Combat Réussi ! La Harpie est abattue en plein vol par votre flèche !\n" RESET); return VICTOIRE; }
        else { printf(ROUGE "  Défaite Fatale ! La Harpie fond sur vous... Il fallait l'Arc long (6) !\n" RESET); return DEFAITE; }
    }
    return RIEN;
}

void logique_portail(int *joueur_x, int *joueur_y) {
    printf(MAGENTA "  🌀 Portail dimensionnel activé ! Saisissez vos coordonnées de distorsion.\n" RESET);
    do {
        printf("  Ligne de destination (0-4) : ");
        if (scanf("%d", joueur_y) != 1) { vider_buffer(); *joueur_y = -1; }
        printf("  Colonne de destination (0-4) : ");
        if (scanf("%d", joueur_x) != 1) { vider_buffer(); *joueur_x = -1; }

        if (*joueur_x < 0 || *joueur_x > 4 || *joueur_y < 0 || *joueur_y > 4) {
            printf(ROUGE "  Coordonnées en dehors de la matrice du donjon !\n" RESET);
        }
    } while (*joueur_x < 0 || *joueur_x > 4 || *joueur_y < 0 || *joueur_y > 4) ;
    vider_buffer();
    printf(MAGENTA "  Téléportation accomplie avec succès en [Ligne %d][Colonne %d] !\n" RESET, *joueur_y, *joueur_x);
}

void logique_totem(Case plateau[TAILLE][TAILLE], int *x, int *y, int depx, int depy) {
    int cx, cy;
    Case temp;
    printf(MAGENTA "  🗿 Totem de Transmutation trouvé ! Choisissez une carte cachée à permuter.\n" RESET);

    do {
        printf("  Ligne de la carte cible (0-4) : ");
        if (scanf("%d", &cy) != 1) { vider_buffer(); cy = -1; }
        printf("  Colonne de la carte cible (0-4) : ");
        if (scanf("%d", &cx) != 1) { vider_buffer(); cx = -1; }

        if (cx < 0 || cx > 4 || cy < 0 || cy > 4) {
            printf(ROUGE "  Coordonnées cibles invalides !\n" RESET);
        }
    } while (cx < 0 || cx > 4 || cy < 0 || cy > 4);
    vider_buffer();

    temp = plateau[*y][*x];
    plateau[*y][*x] = plateau[cy][cx];
    plateau[cy][cx] = temp;

    plateau[*y][*x].visible = 0;
    plateau[cy][cx].visible = 0;

    *x = depx;
    *y = depy;
    printf(MAGENTA "  Distorsion terminée ! Les cartes ont été échangées en secret. Retour au camp.\n" RESET);
}

int verifier_victoire(int *coffre, int *arme_trouvee, char case_actuelle, char arme_recherche) {
    if (case_actuelle == 'C') {
        printf(JAUNE "  Merveilleux ! Vous ouvrez un coffre débordant de pièces d'or !\n" RESET);
        *coffre = 1;
    }
    if (case_actuelle == arme_recherche) {
        *arme_trouvee = 1;
        printf(VERT "  Incroyable ! Vous mettez la main sur l'arme antique légendaire de votre classe !\n" RESET);
    }
    if (*coffre == 1 && *arme_trouvee == 1) {
        return 1;
    }
    return 0;
}
