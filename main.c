#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structures.h"
#include "utils.h"
#include "plateau.h"
#include "jeu.h"
#include "stats.h"

// ============================================================
//  FONCTION PRINCIPALE — BOUCLE DE JEU
// ============================================================

int main(void) {
    srand(time(NULL));
    // Chargement de l'historique depuis le fichier stats.txt
    Joueur historique[MAX_HISTO];
    int nb_historique = charger_historique(historique);

    Case        plateau[TAILLE][TAILLE];
    char        cartes[25];
    JoueurEnJeu tab_j[MAX_JOUEURS];
    int         nb_joueurs, fini, en_lice;
    char        gagnant_nom[MAX_NOM];
    time_t      t_debut, t_fin;
    int         duree, choix_menu, quitter = 0;
    int         i;

    // Écran de titre affiché au premier lancement
    effacer_ecran();
    printf(CYAN GRAS);
    printf("\n  +==================================================+\n");
    printf("  |                                                  |\n");
    printf("  |          M E M O  -  R  P  G                     |\n");
    printf("  |   Explorez le donjon, trouvez le coffre et       |\n");
    printf("  |   votre arme antique !                           |\n");
    printf("  |                                                  |\n");
    printf("  +==================================================+\n\n" RESET);
    printf("  Appuyez sur Entree pour demarrer...");

    while (getchar() != '\n') {
    }

    // ========================================================
    //  BOUCLE PRINCIPALE : menu → partie → menu → ...
    // ========================================================
    while (quitter == 0) {
        printf(CYAN "  +==============================+\n" RESET);
        printf(CYAN "  |" RESET "       MENU PRINCIPAL          " CYAN "|\n" RESET);
        printf(CYAN "  +==============================+\n" RESET);
        printf(CYAN "  |" RESET "  [1] Nouvelle partie          " CYAN "|\n" RESET);
        printf(CYAN "  |" RESET "  [2] Classement general       " CYAN "|\n" RESET);
        printf(CYAN "  |" RESET "  [3] Quitter                  " CYAN "|\n" RESET);
        printf(CYAN "  +==============================+\n" RESET);

        choix_menu = lire_entier("  Votre choix : ", 1, 3);

        if (choix_menu == 3) {
            quitter = 1;
            printf(CYAN "\n  Merci d'avoir joue ! A bientot.\n\n" RESET);
            continue;
        }

        if (choix_menu == 2) {
            effacer_ecran();
            afficher_classement(historique, nb_historique);
            printf("  Appuyez sur Entree pour revenir au menu...");
            getchar();
            effacer_ecran();
            continue;
        }

        // ====================================================
        //  CONFIGURATION DES JOUEURS (avant la première partie)
        // ====================================================
        printf(CYAN "\n  === CONFIGURATION DES JOUEURS ===\n\n" RESET);
        nb_joueurs = lire_entier("  Combien de joueurs (2 a 4) ? ", 2, 4);
        en_lice = nb_joueurs;

        for (i = 0; i < nb_joueurs; i++) {
            char message[80];

            printf(CYAN "\n  --- Configuration Joueur %d ---\n" RESET, i + 1);
            sprintf(message, "  Nom du joueur %d : ", i + 1);
            lire_nom(tab_j[i].nom, message);

            printf("  Choisissez votre classe :\n");
            printf("  " JAUNE "[0]" RESET " Guerrier  -> cherche l'Epee de feu\n");
            printf("  " JAUNE "[1]" RESET " Ranger    -> cherche le Baton familier\n");
            printf("  " JAUNE "[2]" RESET " Magicien  -> cherche le Grimoire interdit\n");
            printf("  " JAUNE "[3]" RESET " Voleur    -> cherche la Dague du sommeil\n");

            tab_j[i].classe = lire_entier("  Votre choix : ", 0, 3);

            // Initialisation des stats de ce joueur pour la partie
            tab_j[i].a_trouve_coffre = 0;
            tab_j[i].a_trouve_arme   = 0;
            tab_j[i].nb_pas          = 0;
            tab_j[i].actif           = 1;
        }

        // ====================================================
        //  BOUCLE "REJOUER" : même joueurs, nouveau plateau
        // ====================================================
        int rejouer = 1;

        while (rejouer == 1) {
            // Remise à zéro pour cette nouvelle partie
            fini           = 0;
            en_lice        = nb_joueurs;
            gagnant_nom[0] = '\0';       // Chaîne vide = pas encore de gagnant
            t_debut        = time(NULL); // Démarre le chronomètre

            // Génération et placement aléatoire des cartes sur le plateau
            remplir_cartes(cartes);
            melanger_carte(cartes, 25);
            remplir_plateau(cartes, plateau);
            init_positions_depart(tab_j, nb_joueurs);

            // Remise à zéro des statistiques de chaque joueur
            for (i = 0; i < nb_joueurs; i++) {
                tab_j[i].a_trouve_coffre = 0;
                tab_j[i].a_trouve_arme   = 0;
                tab_j[i].nb_pas          = 0;
                tab_j[i].actif           = 1;
            }

            // ================================================
            //  BOUCLE DE JEU : tours des joueurs en alternance
            // ================================================
            while (fini == 0 && en_lice > 0) {
                // On parcourt les joueurs en rotation
                for (i = 0; i < nb_joueurs && fini == 0; i++) {
                    if (tab_j[i].actif == 0) {
                        continue;
                    }

                    int action_valide = 0;

                    while (action_valide == 0) {
                        char action;
                        int arme;
                        char dir;
                        int nx, ny;

                        afficher_plateau(plateau, tab_j, nb_joueurs, i);

                        printf("\n  (A)vancer ou (Q)uitter la partie ? ");
                        scanf(" %c", &action);
                        vider_buffer();

                        // Le joueur choisit d'abandonner cette partie
                        if (action == 'q' || action == 'Q') {
                            printf(ROUGE "  %s quitte la partie.\n" RESET, tab_j[i].nom);
                            tab_j[i].actif = 0;
                            en_lice--;
                            action_valide = 1;
                            continue;
                        }

                        if (action != 'a' && action != 'A') {
                            printf(ROUGE "  Commande inconnue. Tapez A ou Q.\n" RESET);
                            printf("  Appuyez sur Entree pour continuer...");
                            getchar();
                            continue;
                        }

                        // --- ÉTAPE 1 : Le joueur choisit son arme ---
                        arme = choisir_arme();

                        // --- ÉTAPE 2 : Le joueur choisit sa direction ---
                        printf("\n  Deplacement : " JAUNE "Z" RESET "=Haut  "
                               JAUNE "S" RESET "=Bas  "
                               JAUNE "Q" RESET "=Gauche  "
                               JAUNE "D" RESET "=Droite\n");
                        printf("  Votre direction : ");
                        scanf(" %c", &dir);
                        vider_buffer();

                        // Calcul des nouvelles coordonnées selon la direction choisie
                        nx = tab_j[i].x;
                        ny = tab_j[i].y;

                        if (dir == 'z' || dir == 'Z') {
                            ny--; // Haut = diminue la ligne
                        } else if (dir == 's' || dir == 'S') {
                            ny++; // Bas = augmente la ligne
                        } else if (dir == 'q' || dir == 'Q') {
                            nx--; // Gauche = diminue la colonne
                        } else if (dir == 'd' || dir == 'D') {
                            nx++; // Droite = augmente la colonne
                        } else {
                            printf(ROUGE "  Direction inconnue ! Utilisez Z, S, Q ou D.\n" RESET);
                            printf("  Appuyez sur Entree pour recommencer le choix du deplacement...");
                            getchar();
                            continue;
                        }

                        // --- ÉTAPE 3 : Vérification que le déplacement est possible ---

                        // Le joueur ne peut pas sortir des bords du monde (x: 0-4, y: -1 à 5)
                        if (nx < 0 || nx >= TAILLE || ny < -1 || ny > TAILLE) {
                            printf(ROUGE "  Mur ! Impossible d'aller dans cette direction.\n" RESET);
                            printf("  Appuyez sur Entree pour recommencer...");
                            getchar();
                            continue;
                        }

                        // Un joueur dans la zone de départ (hors plateau) ne peut pas
                        // se déplacer latéralement : il doit entrer dans le donjon
                        if ((ny == -1 || ny == TAILLE) && nx != tab_j[i].x_dep) {
                            printf(ROUGE "  Vous devez entrer dans le donjon ! Impossible de marcher en dehors.\n" RESET);
                            printf("  Appuyez sur Entree pour recommencer...");
                            getchar();
                            continue;
                        }

                        tab_j[i].x = nx;
                        tab_j[i].y = ny;

                        /*
                         * Si le joueur est dans le plateau, on révèle la case
                         * puis on applique son effet : combat, portail, totem,
                         * coffre ou arme antique.
                         */
                        if (ny >= 0 && ny < TAILLE) {
                            char type_case;
                            char arme_recherche = ' ';

                            tab_j[i].nb_pas++;
                            plateau[ny][nx].visible = 1;
                            type_case = plateau[ny][nx].type;

                            afficher_plateau(plateau, tab_j, nb_joueurs, i);

                            // --- Résolution de l'effet de la case ---
                            if (type_case == 'B' || type_case == 'Z' ||
                                type_case == 'T' || type_case == 'H') {

                                if (resoudre_combat(arme, type_case) == DEFAITE) {
                                    tab_j[i].x = tab_j[i].x_dep;
                                    tab_j[i].y = tab_j[i].y_dep;
                                    cacher_plateau(plateau);

                                    printf(ROUGE "\n  DEFAITE ! Vous retournez au depart et tout le donjon est recache...\n" RESET);
                                }

                                // En cas de victoire au combat, le joueur continue son tour (boucle while)

                            } else if (type_case == 'P') {
                                char type_apres_portail;

                                logique_portail(&tab_j[i].x, &tab_j[i].y);

                                /*
                                 * Après le portail, le joueur arrive sur une nouvelle case.
                                 * On révèle cette case puis on applique aussi son effet.
                                 */
                                plateau[tab_j[i].y][tab_j[i].x].visible = 1;
                                type_apres_portail = plateau[tab_j[i].y][tab_j[i].x].type;

                                afficher_plateau(plateau, tab_j, nb_joueurs, i);

                                if (type_apres_portail == 'B' || type_apres_portail == 'Z' ||
                                    type_apres_portail == 'T' || type_apres_portail == 'H') {

                                    if (resoudre_combat(arme, type_apres_portail) == DEFAITE) {
                                        tab_j[i].x = tab_j[i].x_dep;
                                        tab_j[i].y = tab_j[i].y_dep;
                                        cacher_plateau(plateau);

                                        printf(ROUGE "\n  DEFAITE ! Vous retournez au depart et tout le donjon est recache...\n" RESET);
                                    }

                                } else if (type_apres_portail == 'M') {
                                    logique_totem(plateau, &tab_j[i].x, &tab_j[i].y,
                                                  tab_j[i].x_dep, tab_j[i].y_dep);

                                } else if (type_apres_portail == 'P') {
                                    printf(MAGENTA "  Vous etes deja sur le portail.\n" RESET);

                                } else {
                                    if (tab_j[i].classe == 0) {
                                        arme_recherche = 'E';
                                    } else if (tab_j[i].classe == 1) {
                                        arme_recherche = 'F';
                                    } else if (tab_j[i].classe == 2) {
                                        arme_recherche = 'G';
                                    } else if (tab_j[i].classe == 3) {
                                        arme_recherche = 'D';
                                    }

                                    if (verifier_victoire(&tab_j[i].a_trouve_coffre,
                                                          &tab_j[i].a_trouve_arme,
                                                          type_apres_portail,
                                                          arme_recherche)) {
                                        fini = 1;
                                        strcpy(gagnant_nom, tab_j[i].nom);

                                        printf(VERT GRAS "\n   VICTOIRE ! %s a trouve son arme et le coffre !\n" RESET,
                                               tab_j[i].nom);
                                    }
                                }

                            } else if (type_case == 'M') {
                                logique_totem(plateau, &tab_j[i].x, &tab_j[i].y,
                                              tab_j[i].x_dep, tab_j[i].y_dep);

                            } else {
                                if (tab_j[i].classe == 0) {
                                    arme_recherche = 'E';
                                } else if (tab_j[i].classe == 1) {
                                    arme_recherche = 'F';
                                } else if (tab_j[i].classe == 2) {
                                    arme_recherche = 'G';
                                } else if (tab_j[i].classe == 3) {
                                    arme_recherche = 'D';
                                }

                                if (verifier_victoire(&tab_j[i].a_trouve_coffre,
                                                      &tab_j[i].a_trouve_arme,
                                                      type_case,
                                                      arme_recherche)) {
                                    fini = 1;
                                    strcpy(gagnant_nom, tab_j[i].nom);

                                    printf(VERT GRAS "\n   VICTOIRE ! %s a trouve son arme et le coffre !\n" RESET,
                                           tab_j[i].nom);
                                }
                            }
                        } else {
                            afficher_plateau(plateau, tab_j, nb_joueurs, i);
                        }
                        action_valide = 1;
                        printf("\n  Appuyez sur Entree pour passer au tour suivant...");
                        getchar();
                    }
                }
            }
            // ================================================
            //  FIN DE PARTIE : résumé et sauvegarde
            // ================================================
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
            sauvegarder_historique(historique, nb_historique); // Sauvegarde immédiate dans le fichier
            printf("  [1] Rejouer une partie avec les memes joueurs\n");
            printf("  [2] Retourner au menu principal\n");
            rejouer = lire_entier("  Votre choix : ", 1, 2);
            if (rejouer == 2) {
                effacer_ecran();
                break;
            }
        }
    }
    return 0;
}
