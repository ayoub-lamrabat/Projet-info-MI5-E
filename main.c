#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structures.h"
#include "utils.h"
#include "plateau.h"
#include "jeu.h"
#include "stats.h"

int main(void) {
    srand(time(NULL));

    Joueur historique[MAX_HISTO];
    int nb_historique = charger_historique(historique);

    Case plateau[TAILLE][TAILLE];
    char cartes[25];
    JoueurEnJeu tab_j[MAX_JOUEURS];

    int nb_joueurs, fini, en_lice, choix_menu, quitter = 0, duree;
    char gagnant_nom[MAX_NOM];
    time_t t_debut, t_fin;

    effacer_ecran();
    printf(CYAN GRAS "\n  +==================================================+\n");
    printf("  |            M E M O   -   R P G                   |\n");
    printf("  |         - LE LABYRINTHE DU DONJON -              |\n");
    printf("  +==================================================+\n\n" RESET);

    while (quitter == 0) {
        printf(CYAN "  +==============================+\n" RESET);
        printf(CYAN "  |" RESET "       MENU PRINCIPAL         " CYAN "|\n" RESET);
        printf(CYAN "  +==============================+\n" RESET);
        printf("  " JAUNE "[1]" RESET " Lancer une nouvelle partie\n");
        printf("  " JAUNE "[2]" RESET " Consulter le classement général des joueurs\n");
        printf("  " JAUNE "[3]" RESET " Fermer l'application\n");
        choix_menu = lire_entier("  Votre choix : ", 1, 3);

        if (choix_menu == 3) {
            quitter = 1;
            continue;
        }
        if (choix_menu == 2) {
            effacer_ecran();
            afficher_classement(historique, nb_historique);
            printf("\n  Appuyez sur Entrée pour retourner au menu principal...");
            vider_buffer();
            effacer_ecran();
            continue;
        }

        nb_joueurs = lire_entier("  Entrez le nombre de participants (2 à 4) : ", 2, 4);
        int i;
        for (i = 0; i < nb_joueurs; i++) {
            char message[80];
            sprintf(message, "  Saisissez le nom du Joueur %d : ", i + 1);
            lire_nom(tab_j[i].nom, message);
            printf("  Classes disponibles : [0]Guerrier, [1]Ranger, [2]Magicien, [3]Voleur\n");
            tab_j[i].class_j = lire_entier("  Sélectionnez votre rôle : ", 0, 3);
        }

        int rejouer = 1;
        while (rejouer == 1) {
            fini = 0;
            en_lice = nb_joueurs;
            gagnant_nom[0] = '\0';
            t_debut = time(NULL);

            remplir_cartes(cartes);
            melanger_carte(cartes, 25);
            remplir_plateau(cartes, plateau);
            init_positions_depart(tab_j, nb_joueurs);

            for (i = 0; i < nb_joueurs; i++) {
                tab_j[i].a_trouve_coffre = 0;
                tab_j[i].a_trouve_arme = 0;
                tab_j[i].nb_pas = 0;
                tab_j[i].actif = 1;
            }

            while (fini == 0 && en_lice > 0) {
                for (i = 0; i < nb_joueurs && fini == 0; i++) {
                    if (tab_j[i].actif == 0) continue;

                    int action_valide = 0;
                    while (action_valide == 0) {
                        afficher_plateau(plateau, tab_j, nb_joueurs, i);
                        printf("  Que souhaitez-vous faire ? (A)vancer ou (Q)uitter la partie : ");
                        char action;
                        scanf(" %c", &action);
                        vider_buffer();

                        if (action == 'q' || action == 'Q') {
                            tab_j[i].actif = 0;
                            en_lice--;
                            action_valide = 1;
                            printf(ROUGE "  %s a déclaré forfait et s'est retiré du labyrinthe.\n" RESET, tab_j[i].nom);
                            printf("  Appuyez sur Entrée pour passer la main...");
                            vider_buffer();
                            continue;
                        }
                        if (action != 'a' && action != 'A') {
                            continue;
                        }

                        int arme_choisie = choisir_arme();

                        printf("  Déplacement (Z=Haut, S=Bas, Q=Gauche, D=Droite) : ");
                        char direction;
                        scanf(" %c", &direction);
                        vider_buffer();

                        int nx = tab_j[i].x;
                        int ny = tab_j[i].y;

                        if      (direction == 'z' || direction == 'Z') ny--;
                        else if (direction == 's' || direction == 'S') ny++;
                        else if (direction == 'q' || direction == 'Q') nx--;
                        else if (direction == 'd' || direction == 'D') nx++;
                        else {
                            printf(ROUGE "  Touche directionnelle inconnue !\n" RESET);
                            printf("  Appuyez sur Entrée pour recommencer la saisie...");
                            vider_buffer();
                            continue;
                        }

                        if (tab_j[i].x == tab_j[i].x_dep && tab_j[i].y == tab_j[i].y_dep) {
                            int entree_ok = 0;
                            if      (tab_j[i].y_dep == -1 && (direction == 's' || direction == 'S')) entree_ok = 1;
                            else if (tab_j[i].y_dep == 5  && (direction == 'z' || direction == 'Z')) entree_ok = 1;
                            else if (tab_j[i].x_dep == -1 && (direction == 'd' || direction == 'D')) entree_ok = 1;
                            else if (tab_j[i].x_dep == 5  && (direction == 'q' || direction == 'Q')) entree_ok = 1;

                            if (entree_ok == 0) {
                                printf(ROUGE "  Mouvement interdit ! Vous devez marcher vers l'intérieur du plateau !\n" RESET);
                                printf("  Appuyez sur Entrée pour réessayer...");
                                vider_buffer();
                                continue;
                            }
                        } else if (nx < 0 || nx >= TAILLE || ny < 0 || ny >= TAILLE) {
                            printf(ROUGE "  Impossible ! Les murs magiques du donjon vous bloquent le passage.\n" RESET);
                            printf("  Appuyez sur Entrée pour corriger votre trajectoire...");
                            vider_buffer();
                            continue;
                        }

                        tab_j[i].x = nx;
                        tab_j[i].y = ny;
                        tab_j[i].nb_pas++;

                        plateau[ny][nx].visible = 1;
                        char type_case = plateau[ny][nx].type;
                        action_valide = 1;

                        if (type_case == 'B' || type_case == 'Z' || type_case == 'T' || type_case == 'H') {
                            if (resoudre_combat(arme_choisie, type_case) == DEFAITE) {
                                tab_j[i].x = tab_j[i].x_dep;
                                tab_j[i].y = tab_j[i].y_dep;
                                cacher_plateau(plateau);
                            }
                            printf("  Appuyez sur Entrée pour céder votre tour...");
                            vider_buffer();
                        } else if (type_case == 'P') {
                            logique_portail(&tab_j[i].x, &tab_j[i].y);
                            plateau[tab_j[i].y][tab_j[i].x].visible = 1;
                            printf("  Appuyez sur Entrée pour finaliser votre téléportation...");
                            vider_buffer();
                        } else if (type_case == 'M') {
                            logique_totem(plateau, &tab_j[i].x, &tab_j[i].y, tab_j[i].x_dep, tab_j[i].y_dep);
                            printf("  Appuyez sur Entrée pour relancer la ronde...");
                            vider_buffer();
                        } else {
                            char arme_recherche = (tab_j[i].class_j == 0) ? 'E' : (tab_j[i].class_j == 1) ? 'F' : (tab_j[i].class_j == 2) ? 'G' : 'D';
                            if (verifier_victoire(&tab_j[i].a_trouve_coffre, &tab_j[i].a_trouve_arme, type_case, arme_recherche)) {
                                strcpy(gagnant_nom, tab_j[i].nom);
                                fini = 1;
                            }
                            printf("  Appuyez sur Entrée pour conclure votre action...");
                            vider_buffer();
                        }
                    }
                }
            }

            t_fin = time(NULL);
            duree = (int)difftime(t_fin, t_debut);

            effacer_ecran();
            printf(CYAN GRAS "  +===========================================+\n" RESET);
            printf(CYAN GRAS "  |" RESET BLANC GRAS "               FIN DE LA PARTIE            " CYAN GRAS "|\n" RESET);
            printf(CYAN GRAS "  +===========================================+\n\n" RESET);

            afficher_plateau_final(plateau);
            char noms_partie[MAX_JOUEURS][MAX_NOM];
            int  cases_partie[MAX_JOUEURS];
            for (i = 0; i < nb_joueurs; i++) {
                strcpy(noms_partie[i], tab_j[i].nom);
                cases_partie[i] = tab_j[i].nb_pas;
            }
            afficher_resume_partie(noms_partie, nb_joueurs, gagnant_nom, cases_partie, duree);
            maj_historique(historique, &nb_historique, noms_partie, nb_joueurs, gagnant_nom, cases_partie);
            sauvegarder_historique(historique, nb_historique);
            printf("  [1] Rejouer une partie avec les memes joueurs\n");
            printf("  [2] Retourner au menu principal\n");
            rejouer = lire_entier("  Votre choix : ", 1, 2);
        }
        effacer_ecran();
    }
    return 0;
}
