# Projet: Gestion de B-Tree avec une Interface REPL

## Description
Ce projet implémente un B-Tree en C, combiné avec une interface REPL (Read-Eval-Print Loop) permettant d'insérer, rechercher et afficher des éléments dans un arbre binaire. Ce projet est divisé en plusieurs fichiers sources et en-têtes : `btree.c`, `btree.h`, `repl.c`, `repl.h` et `main.c`.

L'interface REPL permet une interaction dynamique avec l'utilisateur pour effectuer des opérations sur la structure de données.

## Fichiers du Projet
- **btree.c** : Contient les fonctions de manipulation du B-Tree, comme la création d'un nœud, l'insertion et la recherche d'éléments.
- **btree.h** : Définit les structures de données et les prototypes de fonctions utilisées dans le B-Tree.
- **repl.c** : Implémente l'interface utilisateur REPL qui permet d'interagir avec le B-Tree via des commandes textuelles.
- **repl.h** : Déclare les fonctions nécessaires pour le REPL.
- **main.c** : Point d'entrée du programme, lance simplement l'interface REPL.

## Fonctionnalités
1. **Insertion** : Permet d'insérer un nouvel élément (id, nom, email) dans le B-Tree.
2. **Recherche** : Recherche un élément dans le B-Tree par identifiant.
3. **Suppression** : Supprime un élément du B-Tree par identifiant.
4. **REPL** : Interface interactive pour entrer des commandes comme `.exit` pour quitter ou `insert` pour ajouter des éléments.

## Utilisation
Pour compiler et exécuter le programme, utilisez les commandes suivantes :

```sh
# Compilation
gcc -o btree main.c btree.c repl.c

# Exécution
./btree
```

## Version du compilateur
Le projet a été compilé avec GCC version **11.0 w64**. 

## Exemples de Commandes
Une fois le programme lancé, vous pouvez utiliser les commandes suivantes :
- **insert <id> <nom> <email>** : Insère un nouvel enregistrement dans le B-Tree.
- **select <id>** : Recherche et affiche un enregistrement par identifiant.
- **select** : Affiche tous les enregistrements.
- **delete <id>** : Supprime un enregistrement par identifiant.
- **.exit** : Quitte le programme.

### Exemples
- `insert 1 "Jean" "jean@example.com"` : Ajoute un enregistrement avec l'id 1, le nom "Jean" et l'email "jean@example.com".
- `select 1` : Affiche l'enregistrement avec l'id 1.
- `select` : Affiche tous les enregistrements
- `delete 1` : Supprime l'enregistrement avec l'id 1.
- `.exit` : Quitte l'application.

## Notes de Conception
- Le B-Tree est implémenté de façon récursive pour permettre une insertion et une recherche efficaces.
- La mémoire allouée dynamiquement pour les nœuds est libérée grâce à la fonction `free_tree()` afin d'éviter les fuites de mémoire.
- L'interface REPL est conçue pour faciliter l'interaction avec la structure de données, et chaque commande est préparée et exécutée dans une boucle continue.

## Auteur
Ce projet a été réalisé pour démontrer la gestion des structures de données en C et la création d'une interface interactive.