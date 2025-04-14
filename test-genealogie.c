#include "check.h"
#include "canevas.c"

// ----------------------------
// Tests pour genealogieInit
// ----------------------------
START_TEST(test_genealogieInit_1) {
    Genealogie g;
    genealogieInit(&g);
    ck_assert_ptr_nonnull(g->tab);
    ck_assert_int_eq(g->nb_individus, 0);
    ck_assert_int_eq(g->id_cur, 1);
    // pas de test sur `g->taille_max_tab`
    ck_assert_ptr_nonnull(g->rang);
    // pas de test sur `g->taille_max_rang`
    genealogieFree(&g);
} END_TEST

START_TEST(test_genealogieInit_2) {
    genealogieInit(NULL); // Doit gérer NULL sans crash
} END_TEST

// ----------------------------
// Tests pour genealogieFree
// ----------------------------
START_TEST(test_genealogieFree_1) {
    Genealogie g;
    genealogieInit(&g);
    genealogieFree(&g); // Doit libérer sans erreur
    ck_assert_ptr_null(g);
} END_TEST

START_TEST(test_genealogieFree_2) {
    Genealogie g;
    genealogieInit(&g);
    genealogieFree(&g);
    genealogieFree(&g); // Doit gérer le double free
    ck_assert_ptr_null(g);
} END_TEST

START_TEST(test_genealogieFree_3) {
    Genealogie g;
    genealogieInit(&g);
    g->nb_individus = 100;
    genealogieFree(&g);
    ck_assert_ptr_null(g);  // Test même quand le nombre d'individu est erroné
} END_TEST

START_TEST(test_genealogieFree_4) {
    genealogieFree(NULL); // Doit gérer si NULL
} END_TEST

// ----------------------------
// Tests pour nouvIndividu
// ----------------------------
START_TEST(test_nouvIndividu_1) {
    Date naissance = {1, 1, 2000};
    Date deces = {0, 0, 0}; // Vivant
    Individu ind = nouvIndividu(1, "John", omega, omega, naissance, deces);
    
    ck_assert_str_eq(ind->nom, "John");

    ck_assert_int_eq(ind->naiss.annee, 2000);
    ck_assert_int_eq(ind->naiss.mois, 1);
    ck_assert_int_eq(ind->naiss.jour, 1);

    ck_assert_int_eq(ind->deces.annee, 0);
    ck_assert_int_eq(ind->deces.mois, 0);
    ck_assert_int_eq(ind->deces.jour, 0);

    ck_assert_int_eq(ind->id, 1);

    ck_assert_int_eq(ind->ipere, omega);
    ck_assert_int_eq(ind->imere, omega);
    ck_assert_int_eq(ind->icadet, omega);;
    ck_assert_int_eq(ind->ifaine, omega);
    freeIndividu(ind);
} END_TEST

START_TEST(test_nouvIndividu_2) {
    Date naissance = {1, 1, 2000};
    Individu ind = nouvIndividu(omega, "Invalid", omega, omega, naissance, naissance);
    ck_assert_ptr_null(ind); // Doit retourner NULL pour ID invalide
} END_TEST

START_TEST(test_nouvIndividu_3) {
    Date naissance = {1, 1, 2000};
    Individu ind = nouvIndividu(2, "", omega, omega, naissance, naissance);
    ck_assert_ptr_null(ind); // Doit rejeter un nom vide
} END_TEST

// ----------------------------
// Tests pour freeIndividu
// ----------------------------
START_TEST(test_freeIndividu_1) {
    Date n = {1, 1, 1970};
    Individu ind = nouvIndividu(1, "Alice", omega, omega, n, n);
    ck_assert_ptr_nonnull(ind);

    void *ptr_before_free = ind;

    freeIndividu(ind);  // Doit libérer sans crash
    ck_assert_msg(ptr_before_free != NULL, "Le pointeur était déjà NULL");
} END_TEST

START_TEST(test_freeIndividu_2) {
    Date n = {1, 1, 2000};
    Individu ind = nouvIndividu(3, "Alice", omega, omega, n, n);
    ck_assert_ptr_nonnull(ind);

    freeIndividu(ind); // Doit gérer le double free
    ind = NULL;
    freeIndividu(ind); // Doit gérer le double free
    ck_assert_ptr_null(ind);
} END_TEST

START_TEST(test_freeIndividu_3) {
    freeIndividu(NULL); // Doit gérer NULL
} END_TEST

// ----------------------------
// Tests pour nomIndividu
// ----------------------------
START_TEST(test_nomIndividu_1) {
    Individu ind = nouvIndividu(1, "Jean Dupont", omega, omega, (Date){1,1,1990}, (Date){0,0,0});
    ck_assert_str_eq(nomIndividu(ind), "Jean Dupont");
    freeIndividu(ind);
} END_TEST

START_TEST(test_nomIndividu_2) {
    ck_assert_str_eq(nomIndividu(NULL), "");
} END_TEST

START_TEST(test_nomIndividu_3) {
    Individu ind = nouvIndividu(2, "", omega, omega, (Date){1,1,1990}, (Date){0,0,0});
    ck_assert_str_eq(nomIndividu(ind), "");
    freeIndividu(ind);
} END_TEST

// ----------------------------
// Tests pour naissIndividu
// ----------------------------
START_TEST(test_naissIndividu_1) {
    Individu ind = nouvIndividu(3, "Marie", omega, omega, (Date){15,5,1985}, (Date){0,0,0});
    Date d = naissIndividu(ind);
    ck_assert_int_eq(d.jour, 15);
    ck_assert_int_eq(d.mois, 5);
    ck_assert_int_eq(d.annee, 1985);
    freeIndividu(ind);
} END_TEST

START_TEST(test_naissIndividu_2) {
    Date d = naissIndividu(NULL);
    ck_assert_int_eq(d.jour, 0);
    ck_assert_int_eq(d.mois, 0);
    ck_assert_int_eq(d.annee, 0);
} END_TEST

// ----------------------------
// Tests pour cardinal
// ----------------------------
START_TEST(test_cardinal_1) {
    Genealogie g;
    genealogieInit(&g);
    
    // Ajout de 2 individus (simulé)
    g->nb_individus = 2;
    ck_assert_int_eq(cardinal(g), 2);
    
    //genealogieFree(&g);
} END_TEST

START_TEST(test_cardinal_2) {
    ck_assert_int_eq(cardinal(NULL), 0);
} END_TEST

START_TEST(test_cardinal_3) {
    Genealogie g;
    genealogieInit(&g);
    ck_assert_int_eq(cardinal(g), 0);
    genealogieFree(&g);
} END_TEST

// ----------------------------
// Tests pour kieme
// ----------------------------
START_TEST(test_kieme_1) {
    Genealogie g;
    genealogieInit(&g);
    
    // Simulation d'un tableau avec 1 élément
    g->nb_individus = 1;
    Individu ind = nouvIndividu(4, "Paul", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    g->tab[0] = ind;

    ck_assert_ptr_eq(kieme(g, 0), ind);
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_kieme_2) {
    Genealogie g;
    genealogieInit(&g);

    // Simulation d'un tableau avec 1 élément
    g->nb_individus = 1;
    
    ck_assert_ptr_null(kieme(g, 0));
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_kieme_3) {
    ck_assert_ptr_null(kieme(NULL, 0));
} END_TEST

// ----------------------------
// Tests pour getPos
// ----------------------------
START_TEST(test_getPos_1) {
    Genealogie g;
    genealogieInit(&g);
    
    // Ajout d'individus triés (ordre alphabétique simulé)
    adj(g, "Alice", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    adj(g, "Bob", omega, omega, (Date){1,1,2001}, (Date){0,0,0});
    adj(g, "Charlie", omega, omega, (Date){1,1,2002}, (Date){0,0,0});
    
    ck_assert_int_eq(getPos(g, "Alice"), 0); // Position d'insertion
    ck_assert_int_eq(getPos(g, "Bob"), 1);
    ck_assert_int_eq(getPos(g, "Charlie"), 2);

    genealogieFree(&g);
} END_TEST

START_TEST(test_getPos_2) {
    Genealogie g;
    genealogieInit(&g);
    
    adj(g, "Alice", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    adj(g, "Bob", omega, omega, (Date){1,1,2001}, (Date){0,0,0});
    adj(g, "Bob", omega, omega, (Date){1,1,2002}, (Date){0,0,0}); // Doublon
    
    ck_assert_int_eq(getPos(g, "Alice"), 0); // Première occurrence
    ck_assert_int_eq(getPos(g, "Bob"), 1); // Première occurrence

    genealogieFree(&g);
} END_TEST

START_TEST(test_getPos_3) {
    Genealogie g;
    genealogieInit(&g);
    
    adj(g, "Alice", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    adj(g, "Charlie", omega, omega, (Date){1,1,2002}, (Date){0,0,0});
    
    ck_assert_int_eq(getPos(g, "Ali"), 0); // Position d'insertion
    ck_assert_int_eq(getPos(g, "Bob"), 1);
    ck_assert_int_eq(getPos(g, "Turing"), 2);

    genealogieFree(&g);
} END_TEST

START_TEST(test_getPos_4) {
    Genealogie g;
    genealogieInit(&g);
    
    ck_assert_int_eq(getPos(g, "Alice"), 0); // Doit retourner 0
    genealogieFree(&g);
} END_TEST

START_TEST(test_getPos_5) {
    Genealogie g;
    genealogieInit(&g);
    
    adj(g, "Bob", omega, omega, (Date){1,1,2001}, (Date){0,0,0});
    ck_assert_int_eq(getPos(g, "Alice"), 0); // Avant la première entrée
    genealogieFree(&g);
} END_TEST

START_TEST(test_getPos_6) {
    Genealogie g;
    genealogieInit(&g);
    
    adj(g, "Alice", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    ck_assert_int_eq(getPos(g, "Charlie"), 1); // Après la dernière entrée
    genealogieFree(&g);
} END_TEST

// ----------------------------
// Tests pour adj
// ----------------------------
START_TEST(test_adj_1) {
    Genealogie g;
    genealogieInit(&g);
    
    Ident id = adj(g, "Orphelin", omega, omega, (Date){1,1,2000}, (Date){0,0,0});

    ck_assert_int_gt(id, 0); // ID valide
    ck_assert_ptr_nonnull(getByIdent(g, id));
    ck_assert_str_eq(getByIdent(g, id)->nom, "Orphelin");
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_adj_2) {
    Genealogie g;
    genealogieInit(&g);
    
    // Ajout du parent
    Ident id_pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    // Ajout de l'enfant
    Ident id_enfant = adj(g, "Enfant", id_pere, omega, (Date){1,1,2000}, (Date){0,0,0});
    
    // Vérification des liens
    ck_assert_int_eq(getByIdent(g, id_pere)->ifaine, id_enfant); // Premier enfant
    ck_assert_int_eq(getByIdent(g, id_enfant)->ipere, id_pere);
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_adj_3) {
    Genealogie g;
    genealogieInit(&g);
    
    // Ajout du parent
    Ident id_mere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    // Ajout de l'enfant
    Ident id_enfant = adj(g, "Enfant", omega, id_mere, (Date){1,1,2000}, (Date){0,0,0});
    
    // Vérification des liens
    ck_assert_int_eq(getByIdent(g, id_mere)->ifaine, id_enfant); // Premier enfant
    ck_assert_int_eq(getByIdent(g, id_enfant)->imere, id_mere);
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_adj_4) {
    Genealogie g;
    genealogieInit(&g);
    
    // Ajout du parent
    Ident id_mere = adj(g, "Mère", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    // Ajout du parent
    Ident id_pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});

    // Ajout de l'enfant
    Ident id_enfant = adj(g, "Enfant", id_pere, id_mere, (Date){1,1,2000}, (Date){0,0,0});
    
    // Vérification des liens
    ck_assert_int_eq(getByIdent(g, id_mere)->ifaine, id_enfant); // Premier enfant
    ck_assert_int_eq(getByIdent(g, id_enfant)->imere, id_mere);
    ck_assert_int_eq(getByIdent(g, id_pere)->ifaine, id_enfant); // Premier enfant
    ck_assert_int_eq(getByIdent(g, id_enfant)->ipere, id_pere);
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_adj_5) {
    Genealogie g;
    genealogieInit(&g);
    
    // Ajout du parent
    Ident id_mere = adj(g, "Mère", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    // Ajout du parent
    Ident id_pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});

    // Ajout de l'enfant
    Ident id_enfant1 = adj(g, "Enfant", id_pere, id_mere, (Date){1,1,2000}, (Date){0,0,0});
    Ident id_enfant2 = adj(g, "Enfant1", id_pere, id_mere, (Date){12,1,2019}, (Date){0,0,0});
    Ident id_enfant3 = adj(g, "Enfant2", id_pere, id_mere, (Date){2,1,1990}, (Date){0,0,0});
    Ident id_enfant4 = adj(g, "Enfant3", id_pere, id_mere, (Date){1,1,1990}, (Date){0,0,0});
    
    // Vérification des liens
    ck_assert_int_eq(getByIdent(g, id_mere)->ifaine, id_enfant4); // Premier enfant
    ck_assert_int_eq(getByIdent(g, id_enfant4)->imere, id_mere);
    ck_assert_int_eq(getByIdent(g, id_pere)->ifaine, id_enfant4); // Premier enfant
    ck_assert_int_eq(getByIdent(g, id_enfant4)->ipere, id_pere);

    ck_assert_int_eq(getByIdent(g, id_enfant4)->icadet, id_enfant3);
    ck_assert_int_eq(getByIdent(g, id_enfant3)->icadet, id_enfant1);
    ck_assert_int_eq(getByIdent(g, id_enfant1)->icadet, id_enfant2);
    ck_assert_int_eq(getByIdent(g, id_enfant2)->icadet, omega);
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_adj_6) {
    Genealogie g;
    genealogieInit(&g);
    
    Ident id1 = adj(g, "A", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    Ident id2 = adj(g, "B", omega, omega, (Date){2,2,2001}, (Date){0,0,0});
    ck_assert_int_lt(id1, id2); // ID strictement croissants
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_adj_7) {
    Genealogie g;
    genealogieInit(&g);
    
    // Ajout du parent
    Ident id_pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident id_mere = adj(g, "Mère", omega, omega, (Date){1,1,1970}, (Date){0,0,0});

    // Ajout de l'enfant
    Ident id_enfant1 = adj(g, "Enfant1", id_pere, id_mere, (Date){1,1,2000}, (Date){0,0,0});
    Ident id_enfant2 = adj(g, "Enfant2", omega, id_mere, (Date){12,1,1990}, (Date){0,0,0});
    Ident id_enfant3 = adj(g, "Enfant3", id_pere, omega, (Date){2,1,2019}, (Date){0,0,0});
    Ident id_enfant4 = adj(g, "Enfant4", omega, id_mere, (Date){1,1,1990}, (Date){0,0,0});
    
    // Vérification des liens
    ck_assert_int_eq(getByIdent(g, id_mere)->ifaine, id_enfant4);
    ck_assert_int_eq(getByIdent(g, id_enfant4)->imere, id_mere);
    ck_assert_int_eq(getByIdent(g, id_pere)->ifaine, id_enfant4);
    ck_assert_int_eq(getByIdent(g, id_enfant4)->ipere, id_pere);

    ck_assert_int_eq(getByIdent(g, id_enfant4)->icadet, id_enfant2);
    ck_assert_int_eq(getByIdent(g, id_enfant2)->icadet, id_enfant1);
    ck_assert_int_eq(getByIdent(g, id_enfant1)->icadet, id_enfant3);
    ck_assert_int_eq(getByIdent(g, id_enfant3)->icadet, omega);
    
    genealogieFree(&g);
} END_TEST

// ----------------------------
// Tests pour getByName
// ----------------------------
START_TEST(test_getByName_1) {
    Genealogie g;
    genealogieInit(&g);
    
    adj(g, "Alice", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    Individu ind = getByName(g, "Alice", (Date){1,1,2000});
    
    ck_assert_ptr_nonnull(ind);
    ck_assert_str_eq(ind->nom, "Alice");
    ck_assert_int_eq(ind->naiss.annee, 2000);
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_getByName_2) {
    Genealogie g;
    genealogieInit(&g);
    
    adj(g, "Bob", omega, omega, (Date){1,1,1990}, (Date){0,0,0});
    Individu ind = getByName(g, "Bob", (Date){0,0,0});
    
    ck_assert_ptr_nonnull(ind);
    ck_assert_str_eq(ind->nom, "Bob"); // Retourne le seul "Bob"
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_getByName_3) {
    Genealogie g;
    genealogieInit(&g);
    
    adj(g, "Alice", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    adj(g, "Alice", omega, omega, (Date){1,1,2010}, (Date){0,0,0}); // Plus jeune
    
    Individu ind = getByName(g, "Alice", (Date){0,0,0});
    ck_assert_ptr_nonnull(ind);
    ck_assert_int_eq(ind->naiss.annee, 2010); // Doit retourner le plus jeune
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_getByName_4) {
    Genealogie g;
    genealogieInit(&g);
    
    adj(g, "Alice", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    Individu ind = getByName(g, "Bob", (Date){0,0,0});
    
    ck_assert_ptr_null(ind); // Doit retourner NULL
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_getByName_5) {
    Genealogie g;
    genealogieInit(&g);
    
    adj(g, "Alice", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    Individu ind = getByName(g, "Alice", (Date){2,2,2002});
    
    ck_assert_ptr_null(ind); // Date ne correspond à aucun "Alice"
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_getByName_6) {
    Genealogie g;
    genealogieInit(&g);
    
    Individu ind = getByName(g, "Alice", (Date){0,0,0});
    ck_assert_ptr_null(ind);
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_getByName_7) {
    Genealogie g;
    genealogieInit(&g);

    ck_assert_ptr_null(getByName(NULL, "Alice", (Date){0,0,0})); // g NULL
    ck_assert_ptr_null(getByName(g, NULL, (Date){0,0,0}));      // name NULL

    genealogieFree(&g);
} END_TEST

// ----------------------------
// Tests pour affiche_freres_soeurs
// ----------------------------
START_TEST(test_affiche_freres_soeurs_1) {
    Genealogie g;
    genealogieInit(&g);
    
    Ident id = adj(g, "Seul", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    char buff[256];
    affiche_freres_soeurs(g, id, buff);
    
    ck_assert_str_eq(buff, ""); // Doit être vide
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_freres_soeurs_2) {
    Genealogie g;
    genealogieInit(&g);
    
    // Parents communs
    Ident pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident mere = adj(g, "Mère", omega, omega, (Date){1,1,1975}, (Date){0,0,0});
    
    // Enfants
    adj(g, "Aîné", pere, mere, (Date){1,1,2000}, (Date){0,0,0});
    Ident id = adj(g, "Cadet", pere, mere, (Date){1,1,2005}, (Date){0,0,0});
    
    char buff[256];
    affiche_freres_soeurs(g, id, buff);
    ck_assert(strstr(buff, "Aîné")); // Doit lister l'aîné seulement
    ck_assert(!strstr(buff, "Cadet"));
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_freres_soeurs_3) {
    Genealogie g;
    genealogieInit(&g);
    
    // Père commun, mère différente
    Ident pere1 = adj(g, "Père1", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident pere2 = adj(g, "Père2", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident mere1 = adj(g, "Mère1", omega, omega, (Date){1,1,1975}, (Date){0,0,0});
    Ident mere2 = adj(g, "Mère2", omega, omega, (Date){1,1,1980}, (Date){0,0,0});

    Ident pere = adj(g, "Père", pere1, mere1, (Date){1,1,1970}, (Date){0,0,0});
    Ident mere = adj(g, "Mère", pere2, mere2, (Date){1,1,1975}, (Date){0,0,0});
    
    adj(g, "Enfant1", pere, mere, (Date){1,1,2005}, (Date){0,0,0});
    adj(g, "Enfant2", omega, mere, (Date){1,1,2905}, (Date){0,0,0});
    Ident id = adj(g, "Enfant3", pere, omega, (Date){1,1,2105}, (Date){0,0,0});
    adj(g, "Enfant4", pere, mere, (Date){1,1,305}, (Date){0,0,0});
    
    char buff[256];
    affiche_freres_soeurs(g, id, buff);
    ck_assert(strstr(buff, "Enfant1"));
    ck_assert(strstr(buff, "Enfant2"));
    ck_assert(strstr(buff, "Enfant4"));
    ck_assert(!strstr(buff, "Enfant3"));
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_freres_soeurs_4) {
    Genealogie g;
    genealogieInit(&g);
    
    char buff[256];
    affiche_freres_soeurs(g, omega, buff); // ID invalide
    ck_assert_str_eq(buff, "");
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_freres_soeurs_5) {
    char buff[256];
    affiche_freres_soeurs(NULL, 42, buff); // ID invalide
    ck_assert_str_eq(buff, "");
} END_TEST

START_TEST(test_affiche_freres_soeurs_6) {
    Genealogie g;
    genealogieInit(&g);
    
    char buff[256];
    affiche_freres_soeurs(g, 42, buff); // ID invalide
    ck_assert_str_eq(buff, "");
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_freres_soeurs_7) {
    // Cas 7: buff Initialise avec des valeurs
    Genealogie g;
    genealogieInit(&g);
    
    Ident id = adj(g, "Orphelin", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    char buff[256] = "Anmol";
    affiche_freres_soeurs(g, id, buff);
    
    ck_assert_str_eq(buff, "");
    genealogieFree(&g);
} END_TEST

// ----------------------------
// Tests pour affiche_enfants
// ----------------------------
START_TEST(test_affiche_enfants_1) {
    Genealogie g;
    genealogieInit(&g);
    
    Ident id = adj(g, "Parent", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    char buff[256];
    affiche_enfants(g, id, buff);
    
    ck_assert_str_eq(buff, ""); // Doit être vide
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_enfants_2) {
    Genealogie g;
    genealogieInit(&g);
    
    // Création parent
    Ident pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    
    // Création enfants (le chaînage se fait automatiquement via adj)
    adj(g, "Aine", pere, omega, (Date){1,1,2000}, (Date){0,0,0});
    adj(g, "Cadet", pere, omega, (Date){1,1,2005}, (Date){0,0,0});
    
    char buff[256];
    affiche_enfants(g, pere, buff);
    
    // Vérifie que tous les enfants sont listés
    ck_assert(strstr(buff, "Aine"));
    ck_assert(strstr(buff, "Cadet"));
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_enfants_3) {
    Genealogie g;
    genealogieInit(&g);
    
    Ident mere = adj(g, "Mère", omega, omega, (Date){1,1,1975}, (Date){0,0,0});
    
    adj(g, "Premier", omega, mere, (Date){1,1,2000}, (Date){0,0,0});
    adj(g, "Second", omega, mere, (Date){1,1,2002}, (Date){0,0,0});
    
    char buff[256];
    affiche_enfants(g, mere, buff);
    
    // Vérifie l'ordre aîné -> cadet
    ck_assert(strstr(buff, "Premier"));
    ck_assert(strstr(buff, "Second"));
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_enfants_4) {
    Genealogie g;
    genealogieInit(&g);
    
    char buff[256];
    affiche_enfants(g, omega, buff); // ID invalide
    ck_assert_str_eq(buff, "");
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_enfants_5) {
    char buff[256];
    affiche_enfants(NULL, 42, buff); // g NULL
    ck_assert_str_eq(buff, "");
} END_TEST

START_TEST(test_affiche_enfants_6) {
    // Cas 6: buff Initialise avec des valeurs
    Genealogie g;
    genealogieInit(&g);
    
    Ident id = adj(g, "Orphelin", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    char buff[256] = "Anmol";
    affiche_enfants(g, id, buff);
    
    ck_assert_str_eq(buff, "");
    genealogieFree(&g);
} END_TEST

// ----------------------------
// Tests pour affiche_cousins
// ----------------------------
START_TEST(test_affiche_cousins_1) {
    // Cas 1: Aucun cousin (pas d'oncles/tantes)
    Genealogie g;
    genealogieInit(&g);
    
    Ident id = adj(g, "Seul", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    char buff[256];
    affiche_cousins(g, id, buff);
    
    ck_assert_str_eq(buff, "\0");
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_cousins_2) {
    // Cas 2: Cousins germains (enfants des frères/soeurs complets des parents)
    Genealogie g;
    genealogieInit(&g);
    
    // Grands-parents
    Ident grandpere = adj(g, "GP", omega, omega, (Date){1,1,1950}, (Date){0,0,0});
    Ident grandmere = adj(g, "GM", omega, omega, (Date){1,1,1955}, (Date){0,0,0});
    
    // Parents et oncle (mêmes parents)
    Ident pere = adj(g, "Père", grandpere, grandmere, (Date){1,1,1980}, (Date){0,0,0});
    Ident oncle = adj(g, "Oncle", grandpere, grandmere, (Date){1,1,1985}, (Date){0,0,0});
    
    // Individu test et cousin
    adj(g, "Cousin", oncle, omega, (Date){1,1,2010}, (Date){0,0,0});
    Ident id = adj(g, "Moi", pere, omega, (Date){1,1,2015}, (Date){0,0,0});
    
    char buff[256];
    affiche_cousins(g, id, buff);
    
    // Doit contenir le cousin mais pas soi-même
    ck_assert(strstr(buff, "Cousin"));
    ck_assert(!strstr(buff, "Moi"));
    ck_assert(!strstr(buff, "Père")); // Ne doit pas inclure les parents
    ck_assert(!strstr(buff, "Oncle")); // Ne doit pas inclure les oncles
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_cousins_3) {
    // Cas 3: Paramètres invalides
    char buff[256];
    
    // Test avec g = NULL
    affiche_cousins(NULL, 42, buff);
    ck_assert_str_eq(buff, "");
    
    // Test avec id invalide
    Genealogie g;
    genealogieInit(&g);

    affiche_cousins(g, omega, buff);
    ck_assert_str_eq(buff, "");
    affiche_cousins(NULL, omega, buff);
    ck_assert_str_eq(buff, "");

    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_cousins_4) {
    // Cas 4: Individu sans parents (ne peut pas avoir de cousins)
    Genealogie g;
    genealogieInit(&g);
    
    Ident id = adj(g, "Orphelin", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    char buff[256];
    affiche_cousins(g, id, buff);
    
    ck_assert_str_eq(buff, "");
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_cousins_5) {
    // Cas 5: buff Initialise avec des valeurs
    Genealogie g;
    genealogieInit(&g);
    
    Ident id = adj(g, "Orphelin", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    char buff[256] = "Anmol";
    affiche_cousins(g, id, buff);
    
    ck_assert_str_eq(buff, "");
    genealogieFree(&g);
} END_TEST

// ----------------------------
// Tests pour affiche_oncles
// ----------------------------
START_TEST(test_affiche_oncles_1) {
    // Cas 1: Aucun oncle/tante
    Genealogie g;
    genealogieInit(&g);
    
    Ident id = adj(g, "Enfant", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    char buff[256];
    affiche_oncles(g, id, buff);
    
    ck_assert_str_eq(buff, "");
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_oncles_2) {
    // Cas 2: Oncles/tantes complets (côté père)
    Genealogie g;
    genealogieInit(&g);
    
    // Grands-parents
    Ident grandpere = adj(g, "GP", omega, omega, (Date){1,1,1950}, (Date){0,0,0});
    Ident grandmere = adj(g, "GM", omega, omega, (Date){1,1,1955}, (Date){0,0,0});
    
    // Père et oncle (mêmes parents)
    Ident pere = adj(g, "Père", grandpere, grandmere, (Date){1,1,1980}, (Date){0,0,0});
    Ident oncle = adj(g, "Oncle", grandpere, grandmere, (Date){1,1,1985}, (Date){0,0,0});
    
    // Individu test
    Ident id = adj(g, "Enfant", pere, omega, (Date){1,1,2010}, (Date){0,0,0});
    
    char buff[256];
    affiche_oncles(g, id, buff);
    
    ck_assert(strstr(buff, "Oncle"));
    ck_assert(!strstr(buff, "Père")); // Ne doit pas inclure le père
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_oncles_3) {
    // Cas 3: Oncles/tantes complets (côté mère)
    Genealogie g;
    genealogieInit(&g);
    
    // Grands-parents
    Ident grandpere = adj(g, "GP", omega, omega, (Date){1,1,1950}, (Date){0,0,0});
    Ident grandmere = adj(g, "GM", omega, omega, (Date){1,1,1955}, (Date){0,0,0});
    
    // Mère et tante (mêmes parents)
    Ident mere = adj(g, "Mère", grandpere, grandmere, (Date){1,1,1980}, (Date){0,0,0});
    Ident tante = adj(g, "Tante", grandpere, grandmere, (Date){1,1,1985}, (Date){0,0,0});
    
    // Individu test
    Ident id = adj(g, "Enfant", omega, mere, (Date){1,1,2010}, (Date){0,0,0});
    
    char buff[256];
    affiche_oncles(g, id, buff);
    
    ck_assert(strstr(buff, "Tante"));
    ck_assert(!strstr(buff, "Mère")); // Ne doit pas inclure la mère
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_oncles_4) {
    // Cas 4: Paramètres invalides
    char buff[256];
    
    // Test avec g = NULL
    affiche_oncles(NULL, 42, buff);
    ck_assert_str_eq(buff, "");
    
    // Test avec id invalide
    Genealogie g;
    genealogieInit(&g);
    affiche_oncles(g, omega, buff);
    ck_assert_str_eq(buff, "");
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_oncles_5) {
    // Cas 5: Buff Initialise avec des valeurs
    char buff[256] = "Anmol";
    
    // Test avec g = NULL
    affiche_oncles(NULL, 42, buff);
    ck_assert_str_eq(buff, "");
} END_TEST

// ----------------------------
// Tests pour deviennent_freres_soeurs
// ----------------------------
START_TEST(test_deviennent_freres_soeurs_1) {
    // Cas 1: Création d'une nouvelle fratrie
    Genealogie g;
    genealogieInit(&g);
    
    // Parents communs
    Ident pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident mere = adj(g, "Mère", omega, omega, (Date){1,1,1975}, (Date){0,0,0});
    
    // Deux enfants initialement sans lien
    Ident a = adj(g, "A", omega, mere, (Date){1,1,2000}, (Date){0,0,0});
    Ident b = adj(g, "B", pere, omega, (Date){1,1,2005}, (Date){0,0,0});
    
    deviennent_freres_soeurs(g, a, b);
    
    // Vérifie que les liens sont créés
    ck_assert_int_eq(getByIdent(g, a)->icadet, b);
    ck_assert_int_eq(getByIdent(g, b)->icadet, omega);
    ck_assert_int_eq(getByIdent(g, pere)->ifaine, a);
    ck_assert_int_eq(getByIdent(g, mere)->ifaine, a);
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_deviennent_freres_soeurs_2) {
    // Cas 2: Fusion de deux fratries existantes
    Genealogie g;
    genealogieInit(&g);
    
    Ident pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident mere = adj(g, "Mère", omega, omega, (Date){1,1,1975}, (Date){0,0,0});
    
    // Fratrie 1 : A -> C
    Ident a = adj(g, "A", omega, mere, (Date){1,1,2000}, (Date){0,0,0});
    Ident c = adj(g, "C", omega, mere, (Date){1,1,2002}, (Date){0,0,0});
    getByIdent(g, a)->icadet = c;
    
    // Fratrie 2 : B -> D
    Ident b = adj(g, "B", pere, omega, (Date){1,1,2005}, (Date){0,0,0});
    Ident d = adj(g, "D", pere, omega, (Date){1,1,2007}, (Date){0,0,0});
    getByIdent(g, b)->icadet = d;
    
    deviennent_freres_soeurs(g, a, b);
    
    // Vérifie la fusion : A -> C -> B -> D
    ck_assert_int_eq(getByIdent(g, a)->icadet, c);
    ck_assert_int_eq(getByIdent(g, c)->icadet, b);
    ck_assert_int_eq(getByIdent(g, b)->icadet, d);
    ck_assert_int_eq(getByIdent(g, d)->icadet, omega);
    ck_assert_int_eq(getByIdent(g, pere)->ifaine, a);
    ck_assert_int_eq(getByIdent(g, mere)->ifaine, a);
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_deviennent_freres_soeurs_3) {
    // Cas 3: Fusion de deux fratries existantes
    Genealogie g;
    genealogieInit(&g);
    
    Ident pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident mere = adj(g, "Mère", omega, omega, (Date){1,1,1975}, (Date){0,0,0});
    
    // Fratrie 1 : A -> C
    Ident a = adj(g, "A", omega, mere, (Date){1,1,2005}, (Date){0,0,0});
    Ident c = adj(g, "C", omega, mere, (Date){1,1,2010}, (Date){0,0,0});
    getByIdent(g, a)->icadet = c;
    
    // Fratrie 2 : B -> D
    Ident b = adj(g, "B", pere, omega, (Date){1,1,2000}, (Date){0,0,0});
    Ident d = adj(g, "D", pere, omega, (Date){1,1,2002}, (Date){0,0,0});
    getByIdent(g, b)->icadet = d;
    
    deviennent_freres_soeurs(NULL, a, d);
    
    // Vérifie la fusion : B -> D -> A -> C
    ck_assert_int_eq(getByIdent(g, b)->icadet, d);
    ck_assert_int_eq(getByIdent(g, d)->icadet, omega);
    ck_assert_int_eq(getByIdent(g, a)->icadet, c);
    ck_assert_int_eq(getByIdent(g, c)->icadet, omega);
    ck_assert_int_eq(getByIdent(g, pere)->ifaine, b);
    ck_assert_int_eq(getByIdent(g, mere)->ifaine, a);
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_deviennent_freres_soeurs_4) {
    // Cas 4: Parents différents
    Genealogie g;
    genealogieInit(&g);
    
    Ident pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    
    // Fratrie 1 : A -> C
    Ident a = adj(g, "A", omega, omega, (Date){1,1,2005}, (Date){0,0,0});
    Ident c = adj(g, "C", omega, omega, (Date){1,1,2010}, (Date){0,0,0});
    getByIdent(g, a)->icadet = c;
    
    // Fratrie 2 : B -> D
    Ident b = adj(g, "B", pere, omega, (Date){1,1,2000}, (Date){0,0,0});
    Ident d = adj(g, "D", pere, omega, (Date){1,1,2002}, (Date){0,0,0});
    getByIdent(g, b)->icadet = d;
    
    deviennent_freres_soeurs(g, a, b);
    
    // Vérifie la fusion : B -> D -> A -> C
    ck_assert_int_eq(getByIdent(g, b)->icadet, d);
    ck_assert_int_eq(getByIdent(g, d)->icadet, a);
    ck_assert_int_eq(getByIdent(g, a)->icadet, c);
    ck_assert_int_eq(getByIdent(g, c)->icadet, omega);
    ck_assert_int_eq(getByIdent(g, pere)->ifaine, b);
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_deviennent_freres_soeurs_5) {
    // Cas 4: Parents différents
    Genealogie g;
    genealogieInit(&g);
    
    Ident mere = adj(g, "mere", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    
    // Fratrie 1 : A -> C
    Ident a = adj(g, "A", omega, omega, (Date){1,1,2005}, (Date){0,0,0});
    Ident c = adj(g, "C", omega, omega, (Date){1,1,2010}, (Date){0,0,0});
    getByIdent(g, a)->icadet = c;
    
    // Fratrie 2 : B -> D
    Ident b = adj(g, "B", omega, mere, (Date){1,1,2000}, (Date){0,0,0});
    Ident d = adj(g, "D", omega, mere, (Date){1,1,2002}, (Date){0,0,0});
    getByIdent(g, b)->icadet = d;
    
    deviennent_freres_soeurs(g, a, b);
    
    // Vérifie la fusion : B -> D -> A -> C
    ck_assert_int_eq(getByIdent(g, b)->icadet, d);
    ck_assert_int_eq(getByIdent(g, d)->icadet, a);
    ck_assert_int_eq(getByIdent(g, a)->icadet, c);
    ck_assert_int_eq(getByIdent(g, c)->icadet, omega);
    ck_assert_int_eq(getByIdent(g, mere)->ifaine, b);
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_deviennent_freres_soeurs_6) {
    // Cas 4: Parents différents
    Genealogie g;
    genealogieInit(&g);
    
    Ident mere = adj(g, "mere", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    
    // Fratrie 1 : A -> C
    Ident a = adj(g, "A", omega, mere, (Date){1,1,2005}, (Date){0,0,0});
    Ident c = adj(g, "C", omega, mere, (Date){1,1,2010}, (Date){0,0,0});
    getByIdent(g, a)->icadet = c;
    
    // Fratrie 2 : B -> D
    Ident b = adj(g, "B", omega, mere, (Date){1,1,2000}, (Date){0,0,0});
    Ident d = adj(g, "D", omega, mere, (Date){1,1,2002}, (Date){0,0,0});
    getByIdent(g, b)->icadet = d;
    
    deviennent_freres_soeurs(g, a, b);
    
    // Vérifie la fusion : B -> D -> A -> C
    ck_assert_int_eq(getByIdent(g, b)->icadet, d);
    ck_assert_int_eq(getByIdent(g, d)->icadet, a);
    ck_assert_int_eq(getByIdent(g, a)->icadet, c);
    ck_assert_int_eq(getByIdent(g, c)->icadet, omega);
    ck_assert_int_eq(getByIdent(g, mere)->ifaine, b);
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_deviennent_freres_soeurs_7) {
    // Cas 4: Parents différents
    Genealogie g;
    genealogieInit(&g);
    
    Ident pere = adj(g, "pere", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    
    // Fratrie 1 : A -> C
    Ident a = adj(g, "A", pere, omega, (Date){1,1,2005}, (Date){0,0,0});
    Ident c = adj(g, "C", pere, omega, (Date){1,1,2010}, (Date){0,0,0});
    getByIdent(g, a)->icadet = c;
    
    // Fratrie 2 : B -> D
    Ident b = adj(g, "B", pere, omega, (Date){1,1,2000}, (Date){0,0,0});
    Ident d = adj(g, "D", pere, omega, (Date){1,1,2002}, (Date){0,0,0});
    getByIdent(g, b)->icadet = d;
    
    deviennent_freres_soeurs(g, a, b);
    
    // Vérifie la fusion : B -> D -> A -> C
    ck_assert_int_eq(getByIdent(g, b)->icadet, d);
    ck_assert_int_eq(getByIdent(g, d)->icadet, a);
    ck_assert_int_eq(getByIdent(g, a)->icadet, c);
    ck_assert_int_eq(getByIdent(g, c)->icadet, omega);
    ck_assert_int_eq(getByIdent(g, pere)->ifaine, b);
    
    genealogieFree(&g);
} END_TEST

// ----------------------------
// Tests pour devient_pere
// ----------------------------
START_TEST(test_devient_pere_1) {
    // Cas 1: y n'a pas de père
    Genealogie g;
    genealogieInit(&g);
    
    Ident pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident enfant = adj(g, "Enfant", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    
    devient_pere(g, pere, enfant);
    
    // Vérifie les liens
    ck_assert_int_eq(getByIdent(g, enfant)->ipere, pere);
    ck_assert_int_eq(getByIdent(g, pere)->ifaine, enfant);  // Premier enfant
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_devient_pere_2) {
    // Cas 2: y a des frere et soeur
    Genealogie g;
    genealogieInit(&g);
    
    Ident pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    
    Ident mere = adj(g, "Mere", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident enfant1 = adj(g, "Enfant1", omega, mere, (Date){1,1,2010}, (Date){0,0,0});
    Ident enfant2 = adj(g, "Enfant2", omega, mere, (Date){1,1,2009}, (Date){0,0,0});
    getByIdent(g, enfant2)->icadet = enfant1;
    Ident enfant3 = adj(g, "Enfant3", omega, mere, (Date){1,1,2008}, (Date){0,0,0});
    getByIdent(g, enfant3)->icadet = enfant2;
    
    devient_pere(g, pere, enfant1);
    
    // Vérifie les liens
    ck_assert_int_eq(getByIdent(g, enfant1)->ipere, pere);
    ck_assert_int_eq(getByIdent(g, enfant2)->ipere, pere);
    ck_assert_int_eq(getByIdent(g, enfant3)->ipere, pere);
    ck_assert_int_eq(getByIdent(g, enfant1)->icadet, omega);  // Premier enfant
    ck_assert_int_eq(getByIdent(g, enfant2)->icadet, enfant1);  // Premier enfant
    ck_assert_int_eq(getByIdent(g, enfant3)->icadet, enfant2);  // Premier enfant
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_devient_pere_3) {
    // Cas 2: y a des frere et soeur et x a des enfants
    Genealogie g;
    genealogieInit(&g);
    
    Ident pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident mere = adj(g, "Mere", omega, omega, (Date){1,1,1970}, (Date){0,0,0});

    Ident enfant4 = adj(g, "Enfant4", pere, omega, (Date){1,1,2021}, (Date){0,0,0});
    Ident enfant5 = adj(g, "Enfant5", pere, omega, (Date){1,1,2008}, (Date){0,0,0});
    Ident enfant6 = adj(g, "Enfant6", pere, omega, (Date){1,1,2005}, (Date){0,0,0});

    Ident enfant1 = adj(g, "Enfant1", omega, mere, (Date){1,1,2010}, (Date){0,0,0});
    Ident enfant2 = adj(g, "Enfant2", omega, mere, (Date){1,1,2009}, (Date){0,0,0});
    Ident enfant3 = adj(g, "Enfant3", omega, mere, (Date){1,1,2000}, (Date){0,0,0});

    getByIdent(g, enfant3)->icadet = enfant2;
    getByIdent(g, enfant2)->icadet = enfant1;
    getByIdent(g, enfant6)->icadet = enfant5;
    getByIdent(g, enfant5)->icadet = enfant4;
    
    devient_pere(g, pere, enfant1);
    
    // Vérifie les liens
    ck_assert_int_eq(getByIdent(g, enfant1)->ipere, pere);
    ck_assert_int_eq(getByIdent(g, enfant2)->ipere, pere);
    ck_assert_int_eq(getByIdent(g, enfant3)->ipere, pere);
    ck_assert_int_eq(getByIdent(g, enfant4)->ipere, pere);
    ck_assert_int_eq(getByIdent(g, enfant5)->ipere, pere);
    ck_assert_int_eq(getByIdent(g, enfant6)->ipere, pere);
    ck_assert_int_eq(getByIdent(g, enfant3)->icadet, enfant6);
    ck_assert_int_eq(getByIdent(g, enfant6)->icadet, enfant5);
    ck_assert_int_eq(getByIdent(g, enfant5)->icadet, enfant2);
    ck_assert_int_eq(getByIdent(g, enfant2)->icadet, enfant1);
    ck_assert_int_eq(getByIdent(g, enfant1)->icadet, enfant4);
    ck_assert_int_eq(getByIdent(g, enfant4)->icadet, omega);
    
    genealogieFree(&g);
} END_TEST

// ----------------------------
// Tests pour devient_mere
// ----------------------------
START_TEST(test_devient_mere_1) {
    // Cas 1: y n'a pas de mère
    Genealogie g;
    genealogieInit(&g);
    
    Ident mere = adj(g, "Mere", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident enfant = adj(g, "Enfant", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    
    devient_mere(g, mere, enfant);
    
    // Vérifie les liens
    ck_assert_int_eq(getByIdent(g, enfant)->imere, mere);
    ck_assert_int_eq(getByIdent(g, mere)->ifaine, enfant);  // Premier enfant
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_devient_mere_2) {
    // Cas 2: y a des frere et soeur
    Genealogie g;
    genealogieInit(&g);
    
    Ident pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident mere = adj(g, "Mere", omega, omega, (Date){1,1,1970}, (Date){0,0,0});

    Ident enfant1 = adj(g, "Enfant1", pere, omega, (Date){1,1,2010}, (Date){0,0,0});
    Ident enfant2 = adj(g, "Enfant2", pere, omega, (Date){1,1,2009}, (Date){0,0,0});
    getByIdent(g, enfant2)->icadet = enfant1;
    Ident enfant3 = adj(g, "Enfant3", pere, omega, (Date){1,1,2008}, (Date){0,0,0});
    getByIdent(g, enfant3)->icadet = enfant2;
    
    devient_mere(g, mere, enfant1);
    
    // Vérifie les liens
    ck_assert_int_eq(getByIdent(g, enfant1)->imere, mere);
    ck_assert_int_eq(getByIdent(g, enfant2)->imere, mere);
    ck_assert_int_eq(getByIdent(g, enfant3)->imere, mere);
    ck_assert_int_eq(getByIdent(g, enfant1)->icadet, omega);  // Premier enfant
    ck_assert_int_eq(getByIdent(g, enfant2)->icadet, enfant1);  // Premier enfant
    ck_assert_int_eq(getByIdent(g, enfant3)->icadet, enfant2);  // Premier enfant
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_devient_mere_3) {
    // Cas 2: y a des frere et soeur et x a des enfants
    Genealogie g;
    genealogieInit(&g);
    
    Ident pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident mere = adj(g, "Mere", omega, omega, (Date){1,1,1970}, (Date){0,0,0});

    Ident enfant4 = adj(g, "Enfant4", omega, mere, (Date){1,1,2021}, (Date){0,0,0});
    Ident enfant5 = adj(g, "Enfant5", omega, mere, (Date){1,1,2008}, (Date){0,0,0});
    Ident enfant6 = adj(g, "Enfant6", omega, mere, (Date){1,1,2005}, (Date){0,0,0});

    Ident enfant1 = adj(g, "Enfant1", pere, omega, (Date){1,1,2010}, (Date){0,0,0});
    Ident enfant2 = adj(g, "Enfant2", pere, omega, (Date){1,1,2009}, (Date){0,0,0});
    Ident enfant3 = adj(g, "Enfant3", pere, omega, (Date){1,1,2000}, (Date){0,0,0});

    getByIdent(g, enfant3)->icadet = enfant2;
    getByIdent(g, enfant2)->icadet = enfant1;
    getByIdent(g, enfant6)->icadet = enfant5;
    getByIdent(g, enfant5)->icadet = enfant4;
    
    devient_mere(g, mere, enfant1);
    
    // Vérifie les liens
    ck_assert_int_eq(getByIdent(g, enfant1)->imere, mere);
    ck_assert_int_eq(getByIdent(g, enfant2)->imere, mere);
    ck_assert_int_eq(getByIdent(g, enfant3)->imere, mere);
    ck_assert_int_eq(getByIdent(g, enfant4)->imere, mere);
    ck_assert_int_eq(getByIdent(g, enfant5)->imere, mere);
    ck_assert_int_eq(getByIdent(g, enfant6)->imere, mere);
    ck_assert_int_eq(getByIdent(g, enfant3)->icadet, enfant6);
    ck_assert_int_eq(getByIdent(g, enfant6)->icadet, enfant5);
    ck_assert_int_eq(getByIdent(g, enfant5)->icadet, enfant2);
    ck_assert_int_eq(getByIdent(g, enfant2)->icadet, enfant1);
    ck_assert_int_eq(getByIdent(g, enfant1)->icadet, enfant4);
    ck_assert_int_eq(getByIdent(g, enfant4)->icadet, omega);
    
    genealogieFree(&g);
} END_TEST

// ----------------------------
// Tests pour estAncetre
// ----------------------------
START_TEST(test_estAncetre_1) {
    // Cas 1: x est père de y
    Genealogie g;
    genealogieInit(&g);
    
    Ident x = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident y = adj(g, "Enfant", x, omega, (Date){1,1,2000}, (Date){0,0,0});
    
    ck_assert(estAncetre(g, x, y) == true);
    genealogieFree(&g);
} END_TEST

START_TEST(test_estAncetre_2) {
    // Cas 2: x est grand-père de y
    Genealogie g;
    genealogieInit(&g);
    
    Ident x = adj(g, "GrandPère", omega, omega, (Date){1,1,1950}, (Date){0,0,0});
    Ident p = adj(g, "Père", x, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident y = adj(g, "Enfant", p, omega, (Date){1,1,2000}, (Date){0,0,0});
    
    ck_assert(estAncetre(g, x, y) == true);
    genealogieFree(&g);
} END_TEST

START_TEST(test_estAncetre_3) {
    // Cas 3: x n'est pas ancêtre
    Genealogie g;
    genealogieInit(&g);
    
    Ident x = adj(g, "Etranger", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident y = adj(g, "Enfant", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    
    ck_assert(estAncetre(g, x, y) == false);
    genealogieFree(&g);
} END_TEST

START_TEST(test_estAncetre_4) {
    // Cas 4: x == y
    Genealogie g;
    genealogieInit(&g);
    
    Ident x = adj(g, "Moi", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    
    ck_assert(estAncetre(g, x, x) == false);
    genealogieFree(&g);
} END_TEST

START_TEST(test_estAncetre_5) {
    // Cas 5: Paramètres invalides
    Genealogie g;
    genealogieInit(&g);
    
    ck_assert(estAncetre(NULL, 1, 2) == false); // g NULL
    ck_assert(estAncetre(g, omega, 1) == false); // x invalide
    ck_assert(estAncetre(g, 1, omega) == false); // y invalide
    
    genealogieFree(&g);
} END_TEST

// ----------------------------
// Tests pour ontAncetreCommun
// ----------------------------
START_TEST(test_ontAncetreCommun_1) {
    // Cas 1: Frères/soeurs → true
    Genealogie g;
    genealogieInit(&g);
    
    Ident pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident x = adj(g, "Enfant1", pere, omega, (Date){1,1,2000}, (Date){0,0,0});
    Ident y = adj(g, "Enfant2", pere, omega, (Date){1,1,2005}, (Date){0,0,0});
    
    ck_assert(ontAncetreCommun(g, x, y) == true);
    genealogieFree(&g);
} END_TEST

START_TEST(test_ontAncetreCommun_2) {
    // Cas 2: Cousins → true
    Genealogie g;
    genealogieInit(&g);
    
    // Grands-parents communs
    Ident grandpere = adj(g, "GP", omega, omega, (Date){1,1,1950}, (Date){0,0,0});
    
    // Parents (frères)
    Ident pere1 = adj(g, "Père1", grandpere, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident pere2 = adj(g, "Père2", grandpere, omega, (Date){1,1,1975}, (Date){0,0,0});
    
    // Cousins
    Ident x = adj(g, "Enfant1", pere1, omega, (Date){1,1,2000}, (Date){0,0,0});
    Ident y = adj(g, "Enfant2", pere2, omega, (Date){1,1,2005}, (Date){0,0,0});
    
    ck_assert(ontAncetreCommun(g, x, y) == true);
    genealogieFree(&g);
} END_TEST

START_TEST(test_ontAncetreCommun_3) {
    // Cas 3: Aucun ancêtre commun → false (Harry et Bill)
    Genealogie g;
    genealogieInit(&g);
    
    Ident x = adj(g, "Harry", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    Ident y = adj(g, "Bill", omega, omega, (Date){1,1,2005}, (Date){0,0,0});
    
    ck_assert(ontAncetreCommun(g, x, y) == false);
    genealogieFree(&g);
} END_TEST

START_TEST(test_ontAncetreCommun_4) {
    // Cas 4: x == y → true
    Genealogie g;
    genealogieInit(&g);
    
    Ident x = adj(g, "Moi", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    
    ck_assert(ontAncetreCommun(g, x, x) == false);
    genealogieFree(&g);
} END_TEST

START_TEST(test_ontAncetreCommun_5) {
    // Cas 5: Paramètres invalides → false
    Genealogie g;
    genealogieInit(&g);
    
    ck_assert(ontAncetreCommun(NULL, 1, 2) == false); // g NULL
    ck_assert(ontAncetreCommun(g, omega, 1) == false); // x invalide
    ck_assert(ontAncetreCommun(g, 1, omega) == false); // y invalide
    
    genealogieFree(&g);
} END_TEST

// ----------------------------
// Tests pour plus_ancien
// ----------------------------
START_TEST(test_plus_ancien_1) {
    // Cas 1: Lignée complète (ex: Albus → Henri)
    Genealogie g;
    genealogieInit(&g);
    
    // Création de la lignée: Henri -> Fleamont -> James -> Albus
    Ident henri = adj(g, "Henri", omega, omega, (Date){1,1,1900}, (Date){0,0,0});
    Ident fleamont = adj(g, "Fleamont", henri, omega, (Date){1,1,1930}, (Date){0,0,0});
    Ident james = adj(g, "James", fleamont, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident albus = adj(g, "Albus", james, omega, (Date){1,1,2000}, (Date){0,0,0});
    
    ck_assert_int_eq(plus_ancien(g, albus), henri);
    genealogieFree(&g);
} END_TEST

START_TEST(test_plus_ancien_2) {
    // Cas 2: Individu sans parents → retourne lui-même
    Genealogie g;
    genealogieInit(&g);
    
    Ident x = adj(g, "SansParents", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    
    ck_assert_int_eq(plus_ancien(g, x), x);
    genealogieFree(&g);
} END_TEST

START_TEST(test_plus_ancien_3) {
    // Cas 3: Paramètres invalides
    Genealogie g;
    genealogieInit(&g);
    
    ck_assert_int_eq(plus_ancien(NULL, 1), omega); // g NULL
    ck_assert_int_eq(plus_ancien(g, omega), omega); // x invalide
    
    genealogieFree(&g);
} END_TEST

START_TEST(test_plus_ancien_4) {
    // Cas 4: Parents avec mêmes dates
    Genealogie g;
    genealogieInit(&g);
    
    // Parents nés la même année
    Ident pere = adj(g, "Père", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident mere = adj(g, "Mère", omega, omega, (Date){1,1,1970}, (Date){0,0,0});
    Ident x = adj(g, "Enfant", pere, mere, (Date){1,1,2000}, (Date){0,0,0});
    
    // Doit retourner un des deux parents (ordre d'exploration)
    Ident result = plus_ancien(g, x);
    ck_assert(result == pere || result == mere);
    
    genealogieFree(&g);
} END_TEST

// ----------------------------
// Tests pour affiche_parente
// ----------------------------
START_TEST(test_affiche_parente_1) {
    // Cas 1: Lignée complète (ex: Albus)
    Genealogie g;
    genealogieInit(&g);
    
    // Construction de l'arbre généalogique
    Ident henri = adj(g, "Henri", omega, omega, (Date){1,1,1900}, (Date){0,0,0});

    Ident fleamont = adj(g, "Fleamont", henri, omega, (Date){1,1,1930}, (Date){0,0,0});
    Ident euphemia = adj(g, "Euphemia", omega, omega, (Date){1,1,1935}, (Date){0,0,0});

    Ident james = adj(g, "James", fleamont, euphemia, (Date){1,1,1970}, (Date){0,0,0});
    Ident lily = adj(g, "Lily", omega, omega, (Date){1,1,1975}, (Date){0,0,0});

    Ident albus = adj(g, "Albus", james, lily, (Date){1,1,2000}, (Date){0,0,0});
    Ident bill = adj(g, "Bill", james, lily, (Date){1,1,1995}, (Date){0,0,0});

    char buff[1024]; // Taille suffisante
    affiche_parente(g, albus, buff);

    // Vérification du format
    ck_assert(strstr(buff, "- 1 :\nJames Lily ") != NULL);
    ck_assert(strstr(buff, "- 2 :\nFleamont Euphemia") != NULL);
    ck_assert(strstr(buff, "- 3 :\nHenri") != NULL);

    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_parente_2) {
    // Cas 2: Individu sans parents
    Genealogie g;
    genealogieInit(&g);
    
    Ident x = adj(g, "Orphelin", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    char buff[256];
    affiche_parente(g, x, buff);
    
    ck_assert_str_eq(buff, ""); // Doit être vide sauf l'en-tête
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_parente_3) {
    // Cas 3: Paramètres invalides
    char buff[256];
    
    // Test avec g = NULL
    affiche_parente(NULL, 1, buff);
    ck_assert_str_eq(buff, "");
    
    // Test avec x = omega
    Genealogie g;
    genealogieInit(&g);
    affiche_parente(g, omega, buff);
    ck_assert_str_eq(buff, "");
    genealogieFree(&g);
} END_TEST

// ----------------------------
// Tests pour affiche_descendance
// ----------------------------

START_TEST(test_affiche_descendance_1) {
    // Cas 1: Descendance complète (ex: Euphemia)
    Genealogie g;
    genealogieInit(&g);
    
    // Construction de l'arbre généalogique
    Ident euphemia = adj(g, "Euphemia", omega, omega, (Date){1,1,1935}, (Date){0,0,0});
    Ident james = adj(g, "James", omega, euphemia, (Date){1,1,1970}, (Date){0,0,0});
    Ident harry = adj(g, "Harry", james, omega, (Date){1,1,2000}, (Date){0,0,0});
    Ident albus = adj(g, "Albus", harry, omega, (Date){1,1,2030}, (Date){0,0,0});

    char buff[1024]; // Taille suffisante
    affiche_descendance(g, euphemia, buff);

    // Vérification du format
    ck_assert(strstr(buff, "- 1 :\nJames") != NULL);
    ck_assert(strstr(buff, "- 2 :\nHarry") != NULL);
    ck_assert(strstr(buff, "- 3 :\nAlbus") != NULL);

    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_descendance_2) {
    // Cas 2: Individu sans enfants
    Genealogie g;
    genealogieInit(&g);
    
    Ident x = adj(g, "SansEnfants", omega, omega, (Date){1,1,2000}, (Date){0,0,0});
    char buff[256];
    affiche_descendance(g, x, buff);
    
    ck_assert_str_eq(buff, ""); // Doit être vide sauf l'en-tête
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_descendance_3) {
    // Cas 3: Paramètres invalides
    char buff[256];
    
    // Test avec g = NULL
    affiche_descendance(NULL, 1, buff);
    ck_assert_str_eq(buff, "");
    
    // Test avec x = omega
    Genealogie g;
    genealogieInit(&g);
    affiche_descendance(g, omega, buff);
    ck_assert_str_eq(buff, "");
    genealogieFree(&g);
} END_TEST

START_TEST(test_affiche_descendance_4) {
    // Cas 4: Branches multiples
    Genealogie g;
    genealogieInit(&g);
    
    Ident x = adj(g, "Ancetre", omega, omega, (Date){1,1,1900}, (Date){0,0,0});
    Ident e1 = adj(g, "Enfant1", x, omega, (Date){1,1,1930}, (Date){0,0,0});
    Ident e2 = adj(g, "Enfant2", x, omega, (Date){1,1,1935}, (Date){0,0,0});
    Ident p1 = adj(g, "PetitEnfant", e1, omega, (Date){1,1,1960}, (Date){0,0,0});

    char buff[1024];
    affiche_descendance(g, x, buff);

    ck_assert(strstr(buff, "1 :\nEnfant1 Enfant2") != NULL);
    ck_assert(strstr(buff, "2 :\nPetitEnfant") != NULL);

    genealogieFree(&g);
} END_TEST




// ----------------------------
// Suite de tests
// ----------------------------
Suite* genealogie_suite(void) {
    Suite* s = suite_create ("Genealogie");

    TCase* tc_01 = tcase_create ("genealogieInit");
    TCase* tc_02 = tcase_create ("genealogieFree");
    TCase* tc_03 = tcase_create ("nouvIndividu");
    TCase* tc_04 = tcase_create ("freeIndividu");

    TCase* tc_05 = tcase_create ("nomIndividu");
    TCase* tc_06 = tcase_create ("naissIndividu");
    TCase* tc_07 = tcase_create ("cardinal");
    TCase* tc_08 = tcase_create ("kieme");

    TCase* tc_09 = tcase_create ("getPos");
    TCase* tc_10 = tcase_create ("adj");
    TCase* tc_11 = tcase_create ("getByName");

    TCase* tc_12 = tcase_create ("affiche_freres_soeurs");
    TCase* tc_13 = tcase_create ("affiche_enfants");
    TCase* tc_14 = tcase_create ("affiche_cousins");
    TCase* tc_15 = tcase_create ("affiche_oncles");

    TCase* tc_16 = tcase_create ("deviennent_freres_soeurs");
    TCase* tc_17 = tcase_create ("devient_pere");
    TCase* tc_18 = tcase_create ("devient_mere");

    TCase* tc_19 = tcase_create ("estAncetre");
    TCase* tc_20 = tcase_create ("ontAncetreCommun");
    TCase* tc_21 = tcase_create ("plus_ancien");
    TCase* tc_22 = tcase_create ("affiche_parente");
    TCase* tc_23 = tcase_create ("affiche_descendance");

    suite_add_tcase (s, tc_01);
    suite_add_tcase (s, tc_02);
    suite_add_tcase (s, tc_03);
    suite_add_tcase (s, tc_04);

    suite_add_tcase (s, tc_05);
    suite_add_tcase (s, tc_06);
    suite_add_tcase (s, tc_07);
    suite_add_tcase (s, tc_08);

    suite_add_tcase (s, tc_09);
    suite_add_tcase (s, tc_10);
    suite_add_tcase (s, tc_11);

    suite_add_tcase (s, tc_12);
    suite_add_tcase (s, tc_13);
    suite_add_tcase (s, tc_14);
    suite_add_tcase (s, tc_15);

    suite_add_tcase (s, tc_16);
    suite_add_tcase (s, tc_17);
    suite_add_tcase (s, tc_18);

    suite_add_tcase (s, tc_19);
    suite_add_tcase (s, tc_20);
    suite_add_tcase (s, tc_21);
    suite_add_tcase (s, tc_22);
    suite_add_tcase (s, tc_23);

    {
        tcase_add_test (tc_01, test_genealogieInit_1);
        tcase_add_test (tc_01, test_genealogieInit_2);
        
        tcase_add_test (tc_02, test_genealogieFree_1);
        tcase_add_test (tc_02, test_genealogieFree_2);
        tcase_add_test (tc_02, test_genealogieFree_3);
        tcase_add_test (tc_02, test_genealogieFree_4);

        tcase_add_test (tc_03, test_nouvIndividu_1);
        tcase_add_test (tc_03, test_nouvIndividu_2);
        tcase_add_test (tc_03, test_nouvIndividu_3);

        tcase_add_test (tc_04, test_freeIndividu_1);
        tcase_add_test (tc_04, test_freeIndividu_2);
        tcase_add_test (tc_04, test_freeIndividu_3);
    }

    {
        tcase_add_test (tc_05, test_nomIndividu_1);
        tcase_add_test (tc_05, test_nomIndividu_2);
        tcase_add_test (tc_05, test_nomIndividu_3);

        tcase_add_test (tc_06, test_naissIndividu_1);
        tcase_add_test (tc_06, test_naissIndividu_2);

        tcase_add_test (tc_07, test_cardinal_1);
        tcase_add_test (tc_07, test_cardinal_2);
        tcase_add_test (tc_07, test_cardinal_3);

        tcase_add_test (tc_08, test_kieme_1);
        tcase_add_test (tc_08, test_kieme_2);
        tcase_add_test (tc_08, test_kieme_3);
    }

    {
        tcase_add_test (tc_09, test_getPos_1);
        tcase_add_test (tc_09, test_getPos_2);
        tcase_add_test (tc_09, test_getPos_3);
        tcase_add_test (tc_09, test_getPos_4);
        tcase_add_test (tc_09, test_getPos_5);
        tcase_add_test (tc_09, test_getPos_6);

        tcase_add_test (tc_10, test_adj_1);
        tcase_add_test (tc_10, test_adj_2);
        tcase_add_test (tc_10, test_adj_3);
        tcase_add_test (tc_10, test_adj_4);
        tcase_add_test (tc_10, test_adj_5);
        tcase_add_test (tc_10, test_adj_6);
        tcase_add_test (tc_10, test_adj_7);

        tcase_add_test (tc_11, test_getByName_1);
        tcase_add_test (tc_11, test_getByName_2);
        tcase_add_test (tc_11, test_getByName_3);
        tcase_add_test (tc_11, test_getByName_4);
        tcase_add_test (tc_11, test_getByName_5);
        tcase_add_test (tc_11, test_getByName_6);
        tcase_add_test (tc_11, test_getByName_7);
    }

    {
        tcase_add_test (tc_12, test_affiche_freres_soeurs_1);
        tcase_add_test (tc_12, test_affiche_freres_soeurs_2);
        tcase_add_test (tc_12, test_affiche_freres_soeurs_3);
        tcase_add_test (tc_12, test_affiche_freres_soeurs_4);
        tcase_add_test (tc_12, test_affiche_freres_soeurs_5);
        tcase_add_test (tc_12, test_affiche_freres_soeurs_6);
        tcase_add_test (tc_12, test_affiche_freres_soeurs_6);
        tcase_add_test (tc_12, test_affiche_freres_soeurs_7);

        tcase_add_test (tc_13, test_affiche_enfants_1);
        tcase_add_test (tc_13, test_affiche_enfants_2);
        tcase_add_test (tc_13, test_affiche_enfants_3);
        tcase_add_test (tc_13, test_affiche_enfants_4);
        tcase_add_test (tc_13, test_affiche_enfants_5);
        tcase_add_test (tc_13, test_affiche_enfants_6);

        tcase_add_test (tc_14, test_affiche_cousins_1);
        tcase_add_test (tc_14, test_affiche_cousins_2);
        tcase_add_test (tc_14, test_affiche_cousins_3);
        tcase_add_test (tc_14, test_affiche_cousins_4);
        tcase_add_test (tc_14, test_affiche_cousins_5);

        tcase_add_test (tc_15, test_affiche_oncles_1);
        tcase_add_test (tc_15, test_affiche_oncles_2);
        tcase_add_test (tc_15, test_affiche_oncles_3);
        tcase_add_test (tc_15, test_affiche_oncles_4);
        tcase_add_test (tc_15, test_affiche_oncles_5);
    }

    {
        tcase_add_test (tc_16, test_deviennent_freres_soeurs_1);
        tcase_add_test (tc_16, test_deviennent_freres_soeurs_2);
        tcase_add_test (tc_16, test_deviennent_freres_soeurs_3);
        tcase_add_test (tc_16, test_deviennent_freres_soeurs_4);
        tcase_add_test (tc_16, test_deviennent_freres_soeurs_5);
        tcase_add_test (tc_16, test_deviennent_freres_soeurs_6);
        tcase_add_test (tc_16, test_deviennent_freres_soeurs_7);

        tcase_add_test (tc_17, test_devient_pere_1);
        tcase_add_test (tc_17, test_devient_pere_2);
        tcase_add_test (tc_17, test_devient_pere_3);

        tcase_add_test (tc_18, test_devient_mere_1);
        tcase_add_test (tc_18, test_devient_mere_2);
        tcase_add_test (tc_18, test_devient_mere_3);
    }

    {
        tcase_add_test (tc_19, test_estAncetre_1);
        tcase_add_test (tc_19, test_estAncetre_2);
        tcase_add_test (tc_19, test_estAncetre_3);
        tcase_add_test (tc_19, test_estAncetre_4);
        tcase_add_test (tc_19, test_estAncetre_5);

        tcase_add_test (tc_20, test_ontAncetreCommun_1);
        tcase_add_test (tc_20, test_ontAncetreCommun_2);
        tcase_add_test (tc_20, test_ontAncetreCommun_3);
        tcase_add_test (tc_20, test_ontAncetreCommun_4);
        tcase_add_test (tc_20, test_ontAncetreCommun_5);

        tcase_add_test (tc_21, test_plus_ancien_1);
        tcase_add_test (tc_21, test_plus_ancien_2);
        tcase_add_test (tc_21, test_plus_ancien_3);
        tcase_add_test (tc_21, test_plus_ancien_4);

        tcase_add_test (tc_22, test_affiche_parente_1);
        tcase_add_test (tc_22, test_affiche_parente_2);
        tcase_add_test (tc_22, test_affiche_parente_3);

        tcase_add_test (tc_23, test_affiche_descendance_1);
        tcase_add_test (tc_23, test_affiche_descendance_2);
        tcase_add_test (tc_23, test_affiche_descendance_3);
    }

    return s;
}

// int main(void) {
//     int failures;
//     Suite *s = genealogie_suite();
//     SRunner *sr = srunner_create(s);

//     srunner_run_all(sr, CK_VERBOSE);
//     failures = srunner_ntests_failed(sr);
//     srunner_free(sr);

//     return (failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
// }
