# Gestion d'Arbre Généalogique en C

> **Projet Universitaire - Structures de Données et Algorithmes II**
> *Université de Strasbourg - 2024/2025*

Ce projet implémente un système complet de gestion d'arbres généalogiques en langage C "strict". Il a été conçu pour manipuler des structures de données complexes, gérer la mémoire manuellement et optimiser les temps d'accès aux données.
## compilation et testes : 
```
gcc -Wall -Wextra -ansi -pedantic genealogie.c -o genealogie
```
Après cela il suffit juste d'exécuter l'exécutable 
## 📋 Description

L'application permet de construire, mettre à jour et interroger une base de données généalogique où les liens de parenté (parents, enfants, fratries) sont maintenus dynamiquement.

Le système repose sur une table d'individus utilisant une **double indexation** pour garantir des performances optimales :
1.  **Accès direct ($\Theta(1)$)** : Via un identifiant unique (ID).
2.  **Recherche dichotomique ($\Theta(\log n)$)** : Via le nom (le tableau est maintenu trié).

## ✨ Fonctionnalités Principales

* **Gestion Dynamique :** Ajout d'individus avec mise à jour automatique des chaînages (liens `père`, `mère`, `aîné`, `cadet`).
* **Requêtes Complexes :**
    * Identification des ancêtres et de la descendance sur $N$ générations.
    * Recherche d'ancêtres communs entre deux individus.
    * Listing des collatéraux (oncles, tantes, cousins, frères/sœurs).
* **Manipulation Structurelle :** Fusion de fratries et création de liens de parenté *a posteriori*.

## 🛠️ Contraintes Techniques (Hard Mode)

Ce projet a été réalisé sous des contraintes pédagogiques strictes pour renforcer la maîtrise algorithmique et la rigueur du code :

* **Langage :** C ANSI strict (Interdiction totale du C++ et de ses mots-clés).
* **Bibliothèque Standard Interdite :** Pas d'utilisation de `<stdlib.h>`, `<string.h>`, etc. Uniquement un fichier `base.h` fourni.
* **Gestion Mémoire Manuelle :** Allocation via des macros imposées (`MALLOC`, `FREE`) et gestion rigoureuse des fuites mémoire.
* **Structures de Contrôle Limitées :** Interdiction d'utiliser `break`, `continue`, `switch`, `goto`.
* **Fichier Unique :** L'intégralité de la logique (structures, prototypes, fonctions) est contenue dans `genealogie.c`.

## ⚙️ Structures de Données

Le cœur du projet repose sur les structures suivantes :

```c
typedef struct s_individu {
    Car nom[LG_MAX];
    Date naiss, deces;
    Ident id;          // Identifiant unique
    Ident ipere, imere; // IDs des parents
    Ident icadet;      // ID du frère/sœur cadet(te)
    Ident ifaine;      // ID de l'enfant aîné
} *Individu;

typedef struct s_genealogie {
    Individu *tab;     // Tableau dynamique des individus (trié par nom)
    Ident *rang;       // Table de correspondance ID -> Index dans tab
    // ... champs de gestion de taille
} Genealogie;
