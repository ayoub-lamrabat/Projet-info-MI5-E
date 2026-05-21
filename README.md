## Présentation du projet
 
Ce projet a été réalisé dans le cadre du cours d'informatique de la filière préING1 à CY Tech. L'objectif pédagogique est de concevoir et implémenter un programme complet en C, en mettant en pratique les notions de tableaux, structures, pointeurs, fichiers, et organisation modulaire du code.
 
Le jeu lui-même s'inspire des jeux de mémoire classiques (type Memory) combinés à une mécanique RPG légère. Chaque joueur doit retenir mentalement la position des cases révélées lors des tours précédents pour optimiser ses déplacements et éviter les monstres avec la mauvaise arme. La composante compétitive vient du fait que tous les joueurs partagent le même plateau : les informations révélées par un joueur profitent potentiellement aux autres.
 
---
 
## Règles du jeu
 
Chaque joueur choisit une classe parmi quatre au début de la partie. Son objectif est de parcourir le labyrinthe, de trouver son arme antique et au moins un coffre au trésor, sans jamais se faire vaincre par un monstre.
 
A chaque tour, le joueur choisit d'abord une arme de combat, puis se déplace d'une case adjacente à sa position. La case est alors révélée et son contenu déclenche un événement. Si un monstre s'y trouve, le résultat du combat dépend uniquement de l'arme choisie avant le déplacement : une bonne arme permet de continuer, une mauvaise arme entraîne la mort du personnage.
 
En cas de mort, le joueur retourne à sa position de départ, à l'extérieur du plateau, et toutes les cases du labyrinthe sont remises face cachée. C'est ensuite au tour du joueur suivant.
 
La dimension mémoire est au coeur du jeu : les joueurs doivent retenir les emplacements des cases révélées lors des tours passés pour naviguer efficacement, éviter les monstres avec la mauvaise arme, et localiser le coffre et leur arme antique le plus rapidement possible.
 
---
 
## Contenu du plateau
 
Le plateau est un carré de 5x5 cases, toutes cachées au départ et mélangées aléatoirement avant chaque partie. La composition est toujours la même mais la disposition change à chaque nouvelle partie.
 
| Contenu | Symbole affiché | Quantité |
|---|---|---|
| Basilic | Bs | 4 |
| Zombie | Zm | 4 |
| Troll | Tr | 4 |
| Harpie | Hp | 4 |
| Coffre au trésor | Co | 2 |
| Epée de feu | Ep | 1 |
| Baton des familiers | Bt | 1 |
| Grimoire interdit | Gr | 1 |
| Dague du sommeil | Dg | 1 |
| Portail magique | Po | 1 |
| Totem de transmutation | To | 2 |
 
Total : 25 cases pour un plateau 5x5.
 
---
 
## Classes et armes antiques
 
Chaque joueur choisit sa classe au début de la partie. La classe détermine l'arme antique que le joueur doit trouver pour gagner. Il n'y a qu'une seule copie de chaque arme antique sur le plateau.
 
| Classe | Arme antique recherchée |
|---|---|
| Guerrier | Epée de feu |
| Ranger | Baton des familiers |
| Magicien | Grimoire interdit |
| Voleur | Dague du sommeil |
 
Si un joueur tombe sur l'arme antique d'une autre classe, rien ne se passe : il peut simplement continuer son tour.
 
---
 
## Les monstres et leurs faiblesses
 
Il y a 16 monstres sur le plateau, répartis en 4 catégories de 4. Avant chaque déplacement, le joueur choisit une arme de combat parmi quatre. Si la case révélée contient un monstre, seule l'arme correspondante permet de le vaincre.
 
| Monstre | Description | Arme efficace |
|---|---|---|
| Basilic | Creature dont le regard pétrifie | Miroir |
| Zombie | Mort-vivant insensible aux armes classiques | Torche |
| Troll | Creature massive aux défenses élevées | Hache |
| Harpie | Creature volante difficile à atteindre | Arc |
 
Si le joueur choisit une mauvaise arme, son aventurier est vaincu : il retourne à sa position de départ et toutes les cases du plateau sont remises face cachée.
 
---
 
## Cases spéciales
 
Deux types de cases spéciales peuvent modifier le déroulement d'un tour de manière significative.
 
**Portail magique (1 exemplaire)**
 
Lorsqu'un joueur révèle le portail magique, il peut choisir n'importe quelle case encore cachée sur le plateau pour son prochain déplacement, sans être limité aux cases adjacentes. Cela permet de traverser le plateau rapidement si le joueur se souvient d'une case intéressante.
 
**Totem de transmutation (2 exemplaires)**
 
Lorsqu'un joueur révèle un totem, il doit choisir une case cachée n'importe où sur le plateau. Le contenu du totem et le contenu de la case choisie sont échangés, sans que la case choisie ne soit révélée. Le tour du joueur se termine immédiatement : il retourne à sa position de départ et toutes les cases du plateau sont remises face cachée. Le totem est donc une case pénalisante qui réorganise le plateau de façon imprévisible.
 
---
 
## Déroulement d'un tour
 
Un tour se déroule de la façon suivante :
 
**Etape 1 — Choix de l'arme**
 
Le joueur choisit une arme de combat parmi les quatre disponibles : Miroir (3), Torche (4), Hache (5), Arc (6). Ce choix est définitif pour ce déplacement.
 
**Etape 2 — Déplacement**
 
Le joueur se déplace d'une case dans l'une des quatre directions : Z (haut), S (bas), Q (gauche), D (droite). Il ne peut se déplacer que vers une case adjacente, sauf s'il vient de révéler un portail magique lors du tour précédent.
 
**Etape 3 — Résolution de la case**
 
La case est révélée et son contenu est résolu immédiatement :
 
- Monstre avec la bonne arme : le monstre est vaincu, le joueur continue son tour (retour à l'étape 1).
- Monstre avec la mauvaise arme : le joueur meurt, retourne au départ, toutes les cases sont remises face cachée. Tour terminé.
- Coffre au trésor : l'objectif coffre est validé. Si l'arme antique a déjà été trouvée, le joueur gagne.
- Arme antique correspondant à la classe du joueur : l'objectif arme est validé. Si un coffre a déjà été trouvé, le joueur gagne.
- Arme antique d'une autre classe : aucun effet, le tour continue.
- Portail magique : au prochain déplacement, le joueur peut choisir n'importe quelle case cachée du plateau.
- Totem de transmutation : échange de cases, retour au départ, remise à zéro du plateau. Tour terminé.
**Etape 4 — Fin du tour ou continuation**
 
Si le joueur n'est ni mort ni vainqueur, son tour continue et il repasse à l'étape 1. Si toutes les cases autour de lui sont déjà révélées et qu'il ne peut plus avancer, son tour se termine également.
 
---
 
## Conditions de victoire et de défaite
 
Un joueur remporte la partie lorsqu'il a réuni les deux conditions suivantes au cours du même trajet sans mourir :
 
- Avoir révélé au moins un coffre au trésor.
- Avoir révélé l'arme antique correspondant à sa classe.
L'ordre dans lequel ces deux objectifs sont accomplis n'a pas d'importance. Dès que les deux conditions sont remplies, la partie s'arrête immédiatement, le plateau entier est révélé et les scores sont mis à jour.
 
Si tous les joueurs abandonnent la partie sans qu'un vainqueur soit désigné, la partie se termine sans gagnant.
 
---
 

**Navigation dans le menu**
 
Au lancement, un menu principal propose trois options :
 
- `[1]` Nouvelle partie : configure le nombre de joueurs (2 à 4), leurs noms et leurs classes, puis lance la partie.
- `[2]` Classement général : affiche le classement de tous les joueurs enregistrés sur cette machine.
- `[3]` Quitter : ferme le programme.
A la fin d'une partie, il est possible de rejouer directement avec les mêmes joueurs sans repasser par le menu de configuration.
 
---
 
## Structure du projet
 

 
Le fichier `stats.txt` est créé automatiquement dans le répertoire courant lors de la première sauvegarde. Il ne doit pas être supprimé entre deux sessions si vous souhaitez conserver le classement et l'historique des joueurs.
 
Le code source est organisé en sections clairement délimitées par des commentaires :
 
| Section | Contenu |
|---|---|
| Constantes  | Taille du plateau, codes couleur ANSI, identifiants d'armes |
| Structures | `Case`, `JoueurEnJeu`, `Joueur` |
| Fonctions utilitaires | Saisie sécurisée, vidage de buffer, effacement de l'écran |
| Remplissage | Initialisation des cartes, mélange aléatoire, remplissage du plateau |
| Logique de combat | Choix de l'arme, résolution des combats, portail, totem, victoire |
| Affichage | Rendu du plateau avec codes ANSI, affichage du statut des joueurs |
| Fichiers | Lecture, écriture, recherche et tri de l'historique |
| Main | Menu principal, configuration, boucle de jeu |
 
---
 
## Fonctionnalités
 
**Jeu**
 
- Partie de 2 à 4 joueurs en local, chacun avec un nom et une classe au choix.
- Plateau 5x5 généré et mélangé aléatoirement à chaque nouvelle partie.
- Brouillard de guerre : les cases sont cachées et se révèlent progressivement lors des déplacements.
- Système de combat basé sur le choix d'arme avant chaque déplacement.
- Gestion complète des cases spéciales : portail magique et totem de transmutation.
- Retour au départ et remise à zéro du plateau en cas de défaite ou de totem.
- Détection automatique de la victoire dès que les deux objectifs sont remplis.
- Possibilité pour un joueur d'abandonner la partie à tout moment.
**Interface**
 
- Affichage coloré dans le terminal grâce aux codes couleur ANSI.
- Le plateau est réaffiché à chaque tour avec la position de chaque joueur.
- Le statut de chaque joueur (coffre trouvé, arme trouvée, nombre de pas) est affiché en permanence.
- A la fin de la partie, le plateau entier est révélé pour permettre l'analyse des emplacements.
**Persistance**
 
- Historique des joueurs sauvegardé dans un fichier texte (`stats.txt`) entre les sessions.
- Classement général triable selon trois critères : nombre de victoires, nombre de parties jouées, nombre de cases parcourues.
- Mise à jour automatique des statistiques à la fin de chaque partie.
- Affichage du résumé de fin de partie : gagnant, durée, cases parcourues par chaque joueur.
---
 
## Historique et classement
 
L'historique est sauvegardé dans le fichier `stats.txt` au format texte structuré. Chaque joueur ayant joué au moins une partie sur la machine y est enregistré avec les informations suivantes :
 
- Nom du joueur
- Nombre total de parties jouées
- Nombre de victoires
- Nombre total de cases parcourues (toutes parties confondues)
Le classement est trié selon les critères suivants, par ordre de priorité :
 
1. Le nombre de victoires (décroissant).
2. Le nombre de parties jouées (croissant, pour récompenser l'efficacité).
3. Le nombre de cases parcourues (décroissant, pour récompenser l'exploration).
---
 
