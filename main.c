#include "utils.h"
#include "plateau.h"
#include "jeu.h"
#include <time.h>
#include <string.h>

int main(void) {
    srand(time(NULL));
    Joueur historique[MAX_HISTO];
    int nb_historique = charger_historique(historique);

    Case plateau[TAILLE][TAILLE];
    char cartes[25];
    JoueurEnJeu tab_j[MAX_JOUEURS];
    int nb_joueurs, fini, en_lice, choix_menu, quitter = 0;
    char gagnant_nom[MAX_NOM];
    time_t t_debut, t_fin;

    effacer_ecran();
    printf(CYAN GRAS "\n  +==================================================+\n  |      M E M O  -  R  P  G                        |\n  +==================================================+\n\n" RESET);

    while (quitter == 0) {
        printf(CYAN "  +==============================+\n  |" RESET "       MENU PRINCIPAL         " CYAN "|\n  +==============================+\n" RESET);
        printf("  [1] Nouvelle partie\n  [2] Classement general\n  [3] Quitter\n");
        choix_menu = lire_entier("  Votre choix : ", 1, 3);

        if (choix_menu == 3) { quitter = 1; continue; }
        if (choix_menu == 2) {
            effacer_ecran(); afficher_classement(historique, nb_historique);
            printf("\n  Appuyez sur Entree..."); vider_buffer(); effacer_ecran(); continue;
        }

        nb_joueurs = lire_entier("  Nombre de joueurs (2 a 4) : ", 2, 4);
        int i;
        for (i = 0; i < nb_joueurs; i++) {
            char msg[80]; sprintf(msg, "  Nom J%d : ", i + 1);
            lire_nom(tab_j[i].nom, msg);
            printf("  Classe : [0]Guerrier [1]Ranger [2]Magicien [3]Voleur\n");
            tab_j[i].classe = lire_entier("  Choix : ", 0, 3);
        }

        int rejouer = 1;
        while (rejouer == 1) {
            fini = 0; en_lice = nb_joueurs; gagnant_nom[0] = '\0'; t_debut = time(NULL);
            remplir_cartes(cartes); melanger_carte(cartes, 25); remplir_plateau(cartes, plateau);
            init_positions_depart(tab_j, nb_joueurs);

            for (i = 0; i < nb_joueurs; i++) {
                tab_j[i].a_trouve_coffre = 0; tab_j[i].a_trouve_arme = 0; tab_j[i].nb_pas = 0; tab_j[i].actif = 1;
            }

            while (fini == 0 && en_lice > 0) {
                for (i = 0; i < nb_joueurs && fini == 0; i++) {
                    if (tab_j[i].actif == 0) continue;
                    int action_valide = 0;
                    while (action_valide == 0) {
                        afficher_plateau(plateau, tab_j, nb_joueurs, i);
                        printf("  (A)vancer ou (Q)uitter ? ");
                        char act; scanf(" %c", &act); vider_buffer();

                        if (act == 'q' || act == 'Q') {
                            tab_j[i].actif = 0; en_lice--; action_valide = 1; continue;
                        }
                        if (act != 'a' && act != 'A') continue;

                        int arme = choisir_arme();
                        printf("  Direction (Z=Haut, S=Bas, Q=Gauche, D=Droite) : ");
                        char dir; scanf(" %c", &dir); vider_buffer();

                        int nx = tab_j[i].x, ny = tab_j[i].y;
                        if      (dir == 'z' || dir == 'Z') ny--;
                        else if (dir == 's' || dir == 'S') ny++;
                        else if (dir == 'q' || dir == 'Q') nx--;
                        else if (dir == 'd' || dir == 'D') nx++;
                        else { printf("Direction invalide.\n"); vider_buffer(); continue; }

                        if (tab_j[i].x == tab_j[i].x_dep && tab_j[i].y == tab_j[i].y_dep) {
                            int ok = 0;
                            if      (tab_j[i].y_dep == -1 && (dir == 's' || dir == 'S')) ok = 1;
                            else if (tab_j[i].y_dep == 5  && (dir == 'z' || dir == 'Z')) ok = 1;
                            else if (tab_j[i].x_dep == -1 && (dir == 'd' || dir == 'D')) ok = 1;
                            else if (tab_j[i].x_dep == 5  && (dir == 'q' || dir == 'Q')) ok = 1;
                            if (!ok) { printf("Entrez vers l'interieur !\n"); vider_buffer(); continue; }
                        } else if (nx < 0 || nx >= 5 || ny < 0 || ny >= 5) { continue; }

                        tab_j[i].x = nx; tab_j[i].y = ny; tab_j[i].nb_pas++;
                        plateau[ny][nx].visible = 1; char type_c = plateau[ny][nx].type; action_valide = 1;

                        if (type_c == 'B' || type_c == 'Z' || type_c == 'T' || type_c == 'H') {
                            if (resoudre_combat(arme, type_c) == DEFAITE) {
                                tab_j[i].x = tab_j[i].x_dep; tab_j[i].y = tab_j[i].y_dep; cacher_plateau(plateau);
                            }
                            printf("  Entree pour finir..."); vider_buffer();
                        } else if (type_c == 'P') {
                            logique_portail(&tab_j[i].x, &tab_j[i].y); plateau[tab_j[i].y][tab_j[i].x].visible = 1;
                            printf("  Entree pour finir..."); vider_buffer();
                        } else if (type_c == 'M') {
                            logique_totem(plateau, &tab_j[i].x, &tab_j[i].y, tab_j[i].x_dep, tab_j[i].y_dep);
                            melanger_carte(cartes, 25); remplir_plateau(cartes, plateau); cacher_plateau(plateau);
                            printf("  Entree pour finir..."); vider_buffer();
                        } else {
                            char req = (tab_j[i].classe == 0) ? 'E' : (tab_j[i].classe == 1) ? 'F' : (tab_j[i].classe == 2) ? 'G' : 'D';
                            if (verifier_victoire(&tab_j[i].a_trouve_coffre, &tab_j[i].a_trouve_arme, type_c, req)) {
                                strcpy(gagnant_nom, tab_j[i].nom); fini = 1;
                            }
                            printf("  Entree pour finir..."); vider_buffer();
                        }
                    }
                }
            }
            t_fin = time(NULL);
            afficher_plateau_final(plateau);

            char noms_partie[MAX_JOUEURS][MAX_NOM]; int cases_partie[MAX_JOUEURS];
            for (i = 0; i < nb_joueurs; i++) { strcpy(noms_partie[i], tab_j[i].nom); cases_partie[i] = tab_j[i].nb_pas; }
            maj_historique(historique, &nb_historique, noms_partie, nb_joueurs, gagnant_nom, cases_partie);
            sauvegarder_historique(historique, nb_historique);

            afficher_resume_partie(noms_partie, nb_joueurs, gagnant_nom, cases_partie, (int)difftime(t_fin, t_debut));
            printf("  [1] Rejouer avec les memes joueurs\n  [2] Retour menu\n");
            if (lire_entier("  Choix : ", 1, 2) == 2) rejouer = 0;
        }
        effacer_ecran();
    }
    return 0;
}
