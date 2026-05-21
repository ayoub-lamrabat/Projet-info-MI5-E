# Projet-info-MI5-E

# Memo-RPG
 
Projet informatique réalisé dans le cadre de la filière préING1 à CY Tech (2025-2026).
 
Memo-RPG est un jeu de mémoire compétitif en mode local multi-joueurs, développé en C. Entre 2 et 4 aventuriers s'affrontent sur un plateau de 5x5 cases cachées. Chacun explore le labyrinthe à tour de rôle, cherche son arme antique et un coffre au trésor, tout en combattant les monstres qui s'y trouvent.
 
---
 
## Sommaire
 
- [Règles du jeu](#règles-du-jeu)
- [Contenu du plateau](#contenu-du-plateau)
- [Classes et armes antiques](#classes-et-armes-antiques)
- [Déroulement d'un tour](#déroulement-dun-tour)
- [Compilation et lancement](#compilation-et-lancement)
- [Structure du projet](#structure-du-projet)
- [Fonctionnalités](#fonctionnalités)
- [Auteurs](#auteurs)
---
 
## Règles du jeu
 
Chaque joueur choisit une classe parmi quatre. Son objectif est de parcourir le labyrinthe, de trouver son arme antique et au moins un coffre au trésor, sans se faire vaincre par un monstre. Le premier joueur à remplir ces deux conditions remporte la partie.
 
A chaque tour, le joueur choisit une arme de combat, puis se déplace d'une case. Si la case révélée contient un monstre, le résultat du combat dépend de l'arme choisie. Une mauvaise arme entraîne la mort du personnage : il retourne à sa position de départ et toutes les cases sont remises face cachée.
 
La dimension mémoire est centrale : les joueurs doivent mémoriser les cases déjà révélées lors des tours précédents pour naviguer efficacement.
 
---
 
## Contenu du plateau
 
Le plateau est un carré de 5x5 cases, toutes cachées au départ et mélangées aléatoirement.
 
| Contenu | Quantité |
|---|---|
| Basilic | 4 |
| Zombie | 4 |
| Troll | 4 |
| Harpie | 4 |
| Coffre au trésor | 2 |
| Arme antique | 4 (une par classe) |
| Portail magique | 1 |
| Totem de transmutation | 2 |
 
---
 
## Classes et armes antiques
 
| Classe | Arme antique recherchée | Arme de combat efficace contre... |
|---|---|---|
| Guerrier | Epée de feu | Hache (contre le Troll) |
| Ranger | Baton des familiers | Arc (contre la Harpie) |
| Magicien | Grimoire interdit | Miroir (contre le Basilic) |
| Voleur | Dague du sommeil | Torche (contre le Zombie) |
 
Les armes de combat disponibles à chaque tour sont : le Miroir, la Torche, la Hache et l'Arc.
 
---
 
## Déroulement d'un tour
 
1. Le joueur choisit une arme de combat parmi les quatre disponibles.
2. Il se déplace d'une case adjacente à sa position actuelle.
3. La case est révélée et son contenu est résolu :
   - **Monstre** : victoire ou défaite selon l'arme choisie. En cas de défaite, retour au départ et remise à zéro du plateau.
   - **Coffre** : l'objectif coffre est validé.
   - **Arme antique correspondante** : l'objectif arme est validé.
   - **Arme antique d'une autre classe** : aucun effet, le tour continue.
   - **Portail magique** : le joueur peut choisir n'importe quelle case cachée du plateau pour son prochain déplacement.
   - **Totem de transmutation** : le joueur échange le totem avec une case cachée de son choix, sans révéler cette case. Son tour se termine et il retourne à sa position de départ. Toutes les cases sont remises face cachée.
4. Si le joueur a trouvé son arme antique ET un coffre, il gagne. Sinon, son tour continue à l'étape 1.
---
 
## Compilation et lancement
 
**Prérequis** : un compilateur C (GCC recommandé) et un terminal compatible ANSI (Linux ou macOS).
 
```bash
# Compiler le projet
gcc -o memo_rpg memo_rpg.c
 
# Lancer le jeu
./memo_rpg
```
 
Le fichier `stats.txt` est créé automatiquement dans le répertoire courant pour stocker l'historique des joueurs. Ne pas le supprimer entre deux sessions si vous souhaitez conserver le classement.
 
---
 
## Structure du projet
 
```
memo_rpg/
├── memo_rpg.c       # Code source principal
├── stats.txt        # Historique des joueurs (généré automatiquement)
└── README.md        # Ce fichier
```
 
Le code est organisé en sections distinctes :
 
- **Structures** : `Case`, `JoueurEnJeu`, `Joueur`
- **Utilitaires** : saisie sécurisée, vidage de buffer, effacement écran
- **Remplissage** : initialisation et mélange du plateau
- **Logique de combat** : résolution des affrontements et des cases spéciales
- **Affichage** : rendu du plateau avec codes couleur ANSI
- **Fichiers** : lecture, écriture et tri de l'historique
- **Boucle principale** : menu, configuration des joueurs, déroulement de la partie
---
 
## Fonctionnalités
 
- Partie de 2 à 4 joueurs en local, chacun avec un nom et une classe au choix
- Plateau 5x5 généré aléatoirement à chaque partie
- Brouillard de guerre : les cases sont cachées et mémorisées progressivement
- Système de combat arme/monstre
- Cases spéciales : portail magique et totem de transmutation
- Affichage coloré dans le terminal (codes ANSI)
- Historique persistant des joueurs sauvegardé dans `stats.txt`
- Classement général triable par victoires, parties jouées et cases parcourues
- Mesure de la durée de chaque partie
- Option de rejouer avec les mêmes joueurs sans repasser par le menu
---
 
## Auteurs
 
Projet encadré par Eva Ansermin et Romuald Grignon — CY Tech, filière préING1, 2025-2026.
 
