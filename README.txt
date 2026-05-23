======================================================================
             PROJET MEMO-RPG — MANUEL DE COMPILATION
======================================================================

1. DESCRIPTION
Ce projet implémente un jeu de type Memo-RPG en mode multijoueur local (2 à 4 joueurs) conforme au cahier des charges.

2. CONTENU DU DOSSIER
- structures.h : Constantes et structures globales
- utils.h / utils.c : Fonctions utilitaires d'E/S et de terminal
- plateau.h / plateau.c : Initialisation et affichage de la grille
- jeu.h / jeu.c : Logique des combats et cases spéciales
- stats.h / stats.c : Gestion des scores et fichiers historiques
- main.c : Point d'entrée de l'application
- Makefile : Script de compilation unifiée

3. INSTRUCTIONS DE COMPILATION
Ouvrez un terminal dans le répertoire du projet et saisissez :
   make

Cette commande va compiler tous les modules et générer l'exécutable 'memo_rpg'.

4. EXÉCUTION
Pour lancer le jeu, utilisez la commande suivante :
   ./memo_rpg

5. NETTOYAGE
Pour supprimer les fichiers objets (.o) et l'exécutable généré :
   make clean
======================================================================
