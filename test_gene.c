#include <stdio.h>
#include <assert.h>
#include <string.h>

// Inclure ici le fichier où les fonctions sont définies
#include "canevas.c"

void test_genealogieInit() {
    Genealogie g;
    genealogieInit(&g);
    assert(cardinal(g) == 0);
    printf("✅ test_genealogieInit passé !\n");
}

void test_adj_et_getByIdent() {
    Genealogie g;
    genealogieInit(&g);
    
    Ident id1 = adj(g, "Jean", 0, 0, (Date){1990, 5, 10}, (Date){0, 0, 0});
    Ident id2 = adj(g, "Marie", 0, 0, (Date){1995, 8, 20}, (Date){0, 0, 0});
    Ident idPere = adj(g, "Père", id1, id2, (Date){1970, 1, 1}, (Date){0, 0, 0});
    Ident idMere = adj(g, "Mère", 0, id2, (Date){1975, 1, 1}, (Date){0, 0, 0});

    Individu ind1 = getByIdent(g, id1);
    Individu ind2 = getByIdent(g, id2);

    assert(strcmp(nomIndividu(ind1), "Jean") == 0);
    assert(strcmp(nomIndividu(ind2), "Marie") == 0);
    printf("✅ test_adj_et_getByIdent passé !\n");
}

void test_ontAncetreCommun() {
    Genealogie g;
    genealogieInit(&g);

    Ident idAncetre = adj(g, "Ancetre", 0, 0, (Date){1950, 1, 1}, (Date){0, 0, 0});
    Ident idParent1 = adj(g, "Parent1", idAncetre, 0, (Date){1975, 1, 1}, (Date){0, 0, 0});
    Ident idParent2 = adj(g, "Parent2", idAncetre, 0, (Date){1978, 1, 1}, (Date){0, 0, 0});
    Ident idEnfant1 = adj(g, "Enfant1", idParent1, 0, (Date){2000, 1, 1}, (Date){0, 0, 0});
    Ident idEnfant2 = adj(g, "Enfant2", idParent2, 0, (Date){2002, 1, 1}, (Date){0, 0, 0});

    assert(ontAncetreCommun(g, idEnfant1, idEnfant2) == 1);
    assert(ontAncetreCommun(g, idEnfant1, idParent1) == 1);
    assert(ontAncetreCommun(g, idEnfant1, idParent2) == 1);
    assert(ontAncetreCommun(g, idEnfant1, idAncetre) == 1);
    assert(ontAncetreCommun(g, idEnfant2, idParent1) == 1);
    assert(ontAncetreCommun(g, idEnfant2, idAncetre) == 1);

    printf("✅ test_ontAncetreCommun passé !\n");
}

void test_affichage_relations() {
    Genealogie g;
    genealogieInit(&g);

    // Création des individus
    Ident gp = adj(g, "GrandPère", 0, 0, (Date){1948, 1, 1}, (Date){0, 0, 0});
    Ident idPere = adj(g, "Père", 0, 0, (Date){1970, 1, 1}, (Date){0, 0, 0});
    Ident idMere = adj(g, "Mère", 0, 0, (Date){1975, 1, 1}, (Date){0, 0, 0});
    Ident idEnfant1 = adj(g, "Enfant1", 0, 0, (Date){2000, 1, 1}, (Date){0, 0, 0});
    Ident idEnfant2 = adj(g, "Enfant2", 0, 0, (Date){2003, 1, 1}, (Date){0, 0, 0});
    Ident idEnfant3 = adj(g, "Enfant3", idPere, idMere, (Date){2005, 1, 1}, (Date){0, 0, 0});
    Ident idEnfant4 = adj(g, "Enfant4", 0, idMere, (Date){2006, 1, 1}, (Date){0, 0, 0});
    Ident idEnfant5 = adj(g, "Enfant5", 0, idMere, (Date){2007, 1, 1}, (Date){0, 0, 0});
    Ident idEnfant6 = adj(g, "Enfant6", idPere, 0, (Date){2008, 1, 1}, (Date){0, 0, 0});
    //afficheArbre(g);
    
   
    
    

    // Relier Enfant1 et Enfant2 comme frères et sœurs
    printf("\n");
    
    devient_pere(g, gp, idPere);
    
    printf("\n");
    devient_mere(g, idMere, idEnfant2);
    //afficheArbre(g);
    
    
    //devient_mere(g, idMere, idEnfant2);
    devient_pere(g, idPere, idEnfant1);
    

    
    
    
    deviennent_freres_soeurs(g, idEnfant1, idEnfant2);  // Lier Enfant1 et Enfant2 comme frères et sœurs.
    
    
    
    

    // Tester l'affichage des frères et sœurs
    Car buffer[500];
    buffer[0] = 0;
    affiche_freres_soeurs(g, idEnfant1, buffer);
    printf("Frères et sœurs de Enfant1: %s\n", buffer);
    assert(strstr(buffer, "Enfant2") != NULL);  // Vérifier que Enfant2 est bien dans le buffer.

    // Tester l'affichage des enfants (relation parent-enfant)
    buffer[0] = 0;
    affiche_enfants(g, idPere, buffer);
    printf("Enfants de Père: %s\n", buffer);
    assert(strstr(buffer, "Enfant1") != NULL);  // Vérifier que Enfant1 est bien dans le buffer.
    assert(strstr(buffer, "Enfant2") != NULL);  // Vérifier que Enfant2 est bien dans le buffer.

    // Création d'un autre couple avec leurs enfants
    Ident idPereCousin = adj(g, "PèreCousin", 0, 0, (Date){1970, 1, 1}, (Date){0, 0, 0});
    Ident idMereCousin = adj(g, "MèreCousin", 0, 0, (Date){1975, 1, 1}, (Date){0, 0, 0});
    Ident idCousin1 = adj(g, "Cousin1", idPereCousin, idMereCousin, (Date){2000, 1, 1}, (Date){0, 0, 0});
    Ident idCousin2 = adj(g, "Cousin2", idPereCousin, idMereCousin, (Date){2003, 1, 1}, (Date){0, 0, 0});

    // Relier les parents des cousins comme frères et sœurs
    deviennent_freres_soeurs(g, idPere, idPereCousin);  // Les pères de Enfant1 et Cousin1 sont frères.
    
    afficheArbre(g);
    // Afficher les cousins d'Enfant1
    buffer[0] = 0;
    affiche_cousins(g, idEnfant1, buffer);
    printf("Cousins de Enfant1: %s\n", buffer);
    assert(strstr(buffer, "Cousin1") != NULL);  // Vérifier que Cousin1 est bien dans le buffer.
    assert(strstr(buffer, "Cousin2") != NULL);  // Vérifier que Cousin2 est bien dans le buffer.

    printf("✅ test_affichage_relations passé !\n");
}



void test_estAncetre() {
    Genealogie g;
    genealogieInit(&g);

    Ident idAncetre = adj(g, "Ancetre", 0, 0, (Date){1950, 1, 1}, (Date){0, 0, 0});
    Ident idParent = adj(g, "Parent", idAncetre, 0, (Date){1975, 1, 1}, (Date){0, 0, 0});
    Ident idEnfant = adj(g, "Enfant", idParent, 0, (Date){2000, 1, 1}, (Date){0, 0, 0});

    assert(estAncetre(g, idAncetre, idEnfant) == 1);
    assert(estAncetre(g, idParent, idEnfant) == 1);
    assert(estAncetre(g, idAncetre, idParent) == 1);
    assert(estAncetre(g, idEnfant, idAncetre) == 0);

    printf("✅ test_estAncetre passé !\n");
}

void test_plus_ancien() {
    Genealogie g;
    genealogieInit(&g);

    Ident id1 = adj(g, "Personne1", 0, 0, (Date){1920, 5, 5}, (Date){0, 0, 0});
    Ident id2 = adj(g, "Personne2", 0, 0, (Date){1950, 3, 10}, (Date){0, 0, 0});
    Ident id3 = adj(g, "Personne3", 0, 0, (Date){1980, 7, 15}, (Date){0, 0, 0});

    assert(plus_ancien(g, id1) == id1);
    assert(plus_ancien(g, id2) == id1);
    assert(plus_ancien(g, id3) == id1);

    printf("✅ test_plus_ancien passé !\n");
}

int main() {
    test_genealogieInit();
    test_adj_et_getByIdent();
    test_ontAncetreCommun();
    test_affichage_relations();
    test_estAncetre();
    //test_plus_ancien();

    printf("\n🎉 TOUS LES TESTS SONT PASSÉS AVEC SUCCÈS ! 🎉\n");
    return 0;
}



