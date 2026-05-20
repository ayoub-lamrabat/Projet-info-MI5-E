#include "jeu.h"
#include "plateau.h"
#include <string.h>
#include <stdlib.h>

int choisir_arme(void) { 
    int choix; 
    printf(CYAN "\n  Choisissez votre arme pour ce deplacement :\n" RESET);
    printf("  " JAUNE "[3]" RESET " Miroir    -> contre le Basilic\n");
    printf("  " JAUNE "[4]" RESET " Torche    -> contre le Zombie\n");
    printf("  " JAUNE "[5]" RESET " Hache     -> contre le Troll\n");
    printf("  " JAUNE "[6]" RESET " Arc       -> contre la Harpie\n");
    printf("  Votre choix : ");

    do {
        if (scanf("%d", &choix) != 1) { 
            while (getchar() != '\n'); 
            choix = 0;                 
        }
        if (choix < 3 || choix > 6) { 
            printf(ROUGE "  Nombre invalide ! Entrez 3, 4, 5 ou 6 : " RESET);
        }
    } while (choix < 3 || choix > 6); 
    vider_buffer(); 
    return choix; 
}

int resoudre_combat(int arme, char monstre) { 
    if (monstre == 'B') { 
        if (arme == MIROIR) { printf(VERT "  Victoire contre le Basilic !\n" RESET); return VICTOIRE; }
        else { printf(ROUGE "  Petrifie par le Basilic... Il fallait le Miroir !\n" RESET); return DEFAITE; }
    } else if (monstre == 'Z') { 
        if (arme == TORCHE) { printf(VERT "  Zombie vaincu !\n" RESET); return VICTOIRE; }
        else { printf(ROUGE "  Devore par le Zombie... Il fallait la Torche !\n" RESET); return DEFAITE; }
    } else if (monstre == 'T') { 
        if (arme == HACHE) { printf(VERT "  Troll vaincu !\n" RESET); return VICTOIRE; }
        else { printf(ROUGE "  Le Troll vous tue... Il fallait la Hache !\n" RESET); return DEFAITE; }
    } else if (monstre == 'H') { 
        if (arme == ARC) { printf(VERT "  Harpie vaincue !\n" RESET); return VICTOIRE; }
        else { printf(ROUGE "  La Harpie vous tue... Il fallait l'Arc !\n" RESET); return DEFAITE; }
    }
    return RIEN; 
}

void logique_portail(int *joueur_x, int *joueur_y) { 
    printf(MAGENTA "  Portail Magique ! Choisissez une case cible.\n" RESET);
    do { 
        printf("  Nouvelle ligne (0-4) : "); 
        if (scanf("%d", joueur_y) != 1) { vider_buffer(); *joueur_y = -1; }
        printf("  Nouvelle colonne (0-4) : "); 
        if (scanf("%d", joueur_x) != 1) { vider_buffer(); *joueur_x = -1; }

        if (*joueur_x < 0 || *joueur_x > 4 || *joueur_y < 0 || *joueur_y > 4) {
            printf(ROUGE "  Coordonnees invalides !\n" RESET);
        }
    } while (*joueur_x < 0 || *joueur_x > 4 || *joueur_y < 0 || *joueur_y > 4); 
    vider_buffer();
    printf(MAGENTA "  Teleportation en [ligne %d][col %d] !\n" RESET, *joueur_y, *joueur_x); 
}

void logique_totem(Case plateau[5][5], int *x, int *y, int depx, int depy) {
    int cx, cy; 
    Case temp;  
    printf(MAGENTA "  Totem de transmutation ! Choisissez une case a echanger.\n" RESET);

    do {
        printf("  Ligne cible (0-4) : "); 
        if (scanf("%d", &cy) != 1) { vider_buffer(); cy = -1; }
        printf("  Colonne cible (0-4) : "); 
        if (scanf("%d", &cx) != 1) { vider_buffer(); cx = -1; }

        if (cx < 0 || cx > 4 || cy < 0 || cy > 4) {
            printf(ROUGE "  Coordonnees invalides !\n" RESET);
        }
    } while (cx < 0 || cx > 4 || cy < 0 || cy > 4);
    vider_buffer();

    temp              = plateau[*y][*x];  
    plateau[*y][*x]   = plateau[cy][cx]; 
    plateau[cy][cx]   = temp;            

    plateau[*y][*x].visible = 0;
    plateau[cy][cx].visible = 0;
    *x = depx; 
    *y = depy; 
    printf(MAGENTA "  Echange effectue ! Retour au depart.\n" RESET); 
}

int verifier_victoire(int *coffre, int *arme_trouvee, char case_actuelle, char arme_recherche) { 
    if (case_actuelle == 'C') { 
        printf(JAUNE "  Coffre trouve !\n" RESET);
        *coffre = 1; 
    }
    if (case_actuelle == arme_recherche) { 
        *arme_trouvee = 1; 
        printf(VERT "  Votre arme antique specifique a ete trouvee !\n" RESET);
    }
    if (*coffre == 1 && *arme_trouvee == 1) return 1; 
    return 0; 
}

// --- Fonctions fichiers privées ---
static void extraire_nom(char ligne[], char nom[]) {
    int i = 5, j = 0;
    while (ligne[i] != '\0' && ligne[i] != '\n') nom[j++] = ligne[i++];
    nom[j] = '\0';
}

static int extraire_nombre(char ligne[]) {
    char texte_nombre[20]; int i = 0, j = 0;
    while (ligne[i] != ':' && ligne[i] != '\0') i++;
    if (ligne[i] == ':') i++;
    if (ligne[i] == ' ') i++;
    while (ligne[i] != '\0' && ligne[i] != '\n') texte_nombre[j++] = ligne[i++];
    texte_nombre[j] = '\0';
    return atoi(texte_nombre);
}

static int lire_joueur_fichier(FILE *fichier, Joueur *joueur) {
    char ligne[100];
    if (fgets(ligne, 100, fichier) == NULL) return 0;
    extraire_nom(ligne, joueur->nom);
    if (fgets(ligne, 100, fichier) == NULL) return 0;
    joueur->parties = extraire_nombre(ligne);
    if (fgets(ligne, 100, fichier) == NULL) return 0;
    joueur->victoires = extraire_nombre(ligne);
    if (fgets(ligne, 100, fichier) == NULL) return 0;
    joueur->cases_parcourues = extraire_nombre(ligne);
    fgets(ligne, 100, fichier); 
    return 1;
}

int charger_historique(Joueur joueurs[]) {
    FILE *fichier = fopen("stats.txt", "r");
    int nb_joueurs = 0;
    if (fichier == NULL) return 0; 
    while (nb_joueurs < MAX_HISTO && lire_joueur_fichier(fichier, &joueurs[nb_joueurs])) { nb_joueurs++; }
    fclose(fichier);
    return nb_joueurs;
}

void sauvegarder_historique(Joueur joueurs[], int nb_joueurs) {
    FILE *fichier = fopen("stats.txt", "w"); int i;
    if (fichier == NULL) return;
    for (i = 0; i < nb_joueurs; i++) {
        fprintf(fichier, "Nom: %s\nParties: %d\nVictoires: %d\nCases: %d\n---\n",       
                joueurs[i].nom, joueurs[i].parties, joueurs[i].victoires, joueurs[i].cases_parcourues);
    }
    fclose(fichier);
}

void maj_historique(Joueur historique[], int *nb_historique, char noms_partie[][MAX_NOM], int nb_joueurs_partie, char gagnant[], int cases_partie[]) {
    int i;
    for (i = 0; i < nb_joueurs_partie; i++) {
        int pos = -1, k;
        for (k = 0; k < *nb_historique; k++) {
            if (strcmp(historique[k].nom, noms_partie[i]) == 0) { pos = k; break; }
        }
        if (pos == -1 && *nb_historique < MAX_HISTO) {
            pos = *nb_historique;
            strncpy(historique[pos].nom, noms_partie[i], MAX_NOM);
            historique[pos].parties = 0; historique[pos].victoires = 0; historique[pos].cases_parcourues = 0;
            (*nb_historique)++;
        }
        if (pos != -1) {
            historique[pos].parties++;
            historique[pos].cases_parcourues += cases_partie[i];
            if (strcmp(noms_partie[i], gagnant) == 0) historique[pos].victoires++;
        }
    }
}

void afficher_classement(Joueur historique[], int nb_historique) {
    int i, j;
    if (nb_historique == 0) { printf("\n  Aucun historique disponible.\n"); return; }
    for (i = 0; i < nb_historique - 1; i++) {
        int m = i;
        for (j = i + 1; j < nb_historique; j++) {
            if (historique[j].victoires > historique[m].victoires) m = j;
        }
        if (m != i) { Joueur t = historique[i]; historique[i] = historique[m]; historique[m] = t; }
    }
    printf(CYAN GRAS "\n  ======= CLASSEMENT GENERAL =======\n" RESET);
    for (i = 0; i < nb_historique; i++) {
        printf("  %d. %-15s | Victoires: %d | Parties: %d | Cases: %d\n", i + 1, historique[i].nom, historique[i].victoires, historique[i].parties, historique[i].cases_parcourues);
    }
}

void afficher_resume_partie(char noms_partie[][MAX_NOM], int nb_joueurs_partie, char gagnant[], int cases_partie[], int duree_sec) {
    int i;
    printf(JAUNE GRAS "\n  ========= RESUME DE PARTIE =========\n" RESET);
    if (strlen(gagnant) > 0) printf("  Gagnant : " VERT GRAS "%s\n" RESET, gagnant);
    else                     printf("  Gagnant : " ROUGE "Aucun (Abandon)\n" RESET);
    printf("  Duree   : %02d min %02d sec\n\n", duree_sec / 60, duree_sec % 60);
    for (i = 0; i < nb_joueurs_partie; i++) { printf("  - %-15s : %d cases parcourues\n", noms_partie[i], cases_partie[i]); }
}
