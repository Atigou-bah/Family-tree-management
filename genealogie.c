// canevas.c   pour projet Genealogie SDA2 2024-25�
//

// Bah Mamadou Atigou 
///////////////////////////////////

#include "base.h"

// Tyes predefinis
#define LG_MAX  64

typedef struct s_date { Nat jour, mois, annee; } Date;

typedef Nat Ident;
#define omega  0

typedef struct s_individu {
	Car nom[LG_MAX];
    Date naiss; 
    Date deces; 
    Ident id; 
    Ident ipere; 
    Ident imere; 
    Ident icadet; 
    Ident ifaine; 
	
} *Individu;

typedef struct s_genealogie {
	Individu* tab;				// tableau des individus tri�s par nom
    Nat nb_individus;  
    Nat id_cur; 
    Nat taille_max_tab;
    Nat* rang;    
    Nat taille_max_rang; 
} *Genealogie;

// DEFINIR ICI VOS CONSTANTES
#define TAILLE_MAX_TAB 100
#define TAILLE_MAX_RANG 100
Date d_null = {0,0,0};


// PARTIE 1: PROTOTYPES des operations imposees
Ent compDate(Date d1, Date d2);
void genealogieInit(Genealogie* g);
void genealogieFree(Genealogie* g);
Individu nouvIndividu(Ident i, Chaine s, Ident p, Ident m, Date n, Date d);
void freeIndividu(Individu id);
// selecteurs
Chaine nomIndividu(Individu ind);
Date naissIndividu(Individu ind);
Nat cardinal(Genealogie g);
Individu kieme(Genealogie g, Nat k);
Individu getByIdent(Genealogie g, Ident i);
Nat getPos(Genealogie g, Chaine name);
Individu getByName(Genealogie g, Chaine name, Date naissance);
// modificateurs
Ident adj(Genealogie g, Chaine s, Ident p, Ident m, Date n, Date d);
void insert(Genealogie g, Nat pos, Chaine s, Ident p, Ident m, Date n, Date d);
void adjFils(Genealogie g, Ident idx, Ident fils, Ident pp, Ident mm);

//PARTIE 2: PROTOTYPES des operations imposees
void affiche_freres_soeurs(Genealogie g, Ident x, Chaine buff);
void affiche_enfants(Genealogie g, Ident x, Chaine buff);
void affiche_cousins(Genealogie g, Ident x, Chaine buff);
void affiche_oncles(Genealogie g, Ident x, Chaine buff);

//PARTIE 2: PROTOTYPES des operations imposees
void deviennent_freres_soeurs(Genealogie g, Ident x, Ident y);
void devient_pere(Genealogie g, Ident x, Ident y);
void devient_mere(Genealogie g, Ident x, Ident y);

//PARTIE 4: PROTOTYPES des operations imposees
Bool estAncetre(Genealogie g, Ident x, Ident y);
Bool ontAncetreCommun(Genealogie g, Ident x, Ident y);
Ident plus_ancien(Genealogie g, Ident x);
void affiche_parente(Genealogie g, Ident x, Chaine buf);
void affiche_descendance(Genealogie g, Ident x, Chaine buf);

// PROTOTYPES DE VOS FONCTIONS INTERMEDIAIRES
Bool est_freres_soeurs(Genealogie g, Ident x , Ident y);
Individu find_oldest(Genealogie g, Ident x); 


/// PARTIE 1: Construction de l�arbre g�n�alogique et acc�s 
/// ///////////////////////////////////////////////////////
/// 

//PRE: None
Ent compDate(Date d1, Date d2)
{
    if (d1.annee != d2.annee) {
        return d1.annee - d2.annee;  
    }
    if (d1.mois != d2.mois) {
        return d1.mois - d2.mois;  
    }
    return d1.jour - d2.jour;
}


//PRE: None
void genealogieInit(Genealogie *g)
{
	if (g== NULL || (*g) == NULL) return;
	*g = MALLOC(struct s_genealogie); 
    (*g)->id_cur = 1; 
    (*g)->nb_individus = 0; 
    (*g)->tab = MALLOCN(Individu,TAILLE_MAX_TAB); 
	for (Nat i = 0; i < TAILLE_MAX_RANG; i++)
	{
		(*g)->tab[i] = NULL; 
	}
	
    (*g)->rang = MALLOCN(Ident, TAILLE_MAX_RANG); 
    (*g)->taille_max_rang = TAILLE_MAX_RANG; 
    (*g)->taille_max_tab = TAILLE_MAX_TAB;  

}

//PRE: None
void genealogieFree(Genealogie *g)
{
	if (g== NULL || (*g) == NULL) return;

    for (Nat i = 0; i < (*g)->nb_individus; i++)
    {
        freeIndividu((*g)->tab[i]); 
    }
    FREE((*g)->tab); 
    FREE((*g)->rang);
    FREE(*g); 
    *g = NULL; 
}

//PRE: None
Individu nouvIndividu(Ident i, Chaine s, Ident p, Ident m, Date n, Date d)	
{   
	if (s[0] == '\0' || i == omega) return NULL;
    Individu Personne = (Individu)MALLOC(struct s_individu); 
	if (Personne == NULL)
	{
		printf("erreur d'allocation memoire de l'individu \n"); 
	}
	
    Personne->id = i; 
    Personne->imere = m; 
    Personne->ipere = p; 
    Personne->deces = d; 
    Personne->naiss = n; 
	Personne->icadet = omega; 
	Personne->ifaine = omega;
    chaineCopie(Personne->nom,s); 
	return Personne;
}

//PRE: None
void freeIndividu(Individu id)
{
    FREE(id); 
}

// Selecteur
// ////////////////////
Chaine nomIndividu(Individu ind) { 
	if (ind == NULL) return ""; 
	return ind->nom;
}

Date naissIndividu(Individu ind) { if (ind != NULL){ return ind->naiss; } return d_null ;}
Nat cardinal(Genealogie g) { 
	if (g == NULL) return 0; 
	return g->nb_individus; }

Individu kieme(Genealogie g, Nat k) { 
	if ( g == NULL || k > g->nb_individus) return NULL; 
	return g->tab[k];
}

//PRE: None
Individu getByIdent(Genealogie g, Ident i)
{

    if (i <= 0 || i >= g->id_cur) {  
        return NULL; 
    }
	Nat indice = g->rang[i-1]; 
    return g->tab[indice];  
}


//PRE: None
Nat getPos(Genealogie g, Chaine name)
{   
    if (g->nb_individus == 0)
    {
        return 0; 
    }

    Ent gauche = 0; 
    Ent droite = g->nb_individus;


    while (gauche < droite)
    {  
       
        Ent milieu = (gauche + droite) / 2; 
        
        if (chaineCompare(g->tab[milieu]->nom, name) == 0 )
        {
            return milieu;  
        }
        else if (chaineCompare(g->tab[milieu]->nom, name) < 0) 
        {
           
            gauche = milieu + 1 ; 
        }
        else 
        {
 
            droite = milieu; 
        }
    }
    
   
    return (Nat)gauche; 
}

Individu getByName(Genealogie g, Chaine name, Date naissance)
{
    if (g == NULL || name == NULL || name[0] == '\0') return NULL;
    
    Date date_0 = {0, 0, 0};
    Nat pos = getPos(g, name);

    // Remonter pour s'assurer qu'on ne saute pas le premier avec ce nom
    while (pos > 0 && chaineCompare(g->tab[pos - 1]->nom, name) == 0) {
        pos--;
    }

    Individu result = NULL;
    Nat i = pos;

    for (i; i < g->nb_individus && chaineCompare(g->tab[i]->nom, name) == 0; i++) {
        if (compDate(date_0, naissance) == 0) {
            // Si naissance non précisée → chercher le plus jeune
            if (result == NULL || compDate(g->tab[i]->naiss, result->naiss) > 0) {
                result = g->tab[i];
            }
        } else {
            // Si naissance précisée → chercher correspondance exacte
            if (compDate(g->tab[i]->naiss, naissance) == 0) {
                return g->tab[i];
            }
        }
    }

    return result;
}
   
    
 
    

//PRE: (pos>=1 => chaineCompare(g[pos-1]->nom,s)<=0) 
//     && (pos<g->nb_individus-1 => chaineCompare(g[pos+1]->nom,s)>=0)
void insert(Genealogie g, Nat pos, Chaine s, Ident p, Ident m, Date n, Date d)
{		
	if ( pos < g->nb_individus && chaineCompare(g->tab[pos]->nom,s) == 0 && compDate(g->tab[pos]->naiss, n) == 0)
		{
			return;

		}
		Individu Individu_to_add = nouvIndividu(g->id_cur,s,p,m,n,d);
		if (g->taille_max_tab <= pos)
		{
			g->taille_max_tab *= 2; 
			g->taille_max_rang *=2;
            g->tab = REALLOC(g->tab, Individu, g->taille_max_tab); // Doubler la taille du tableau
			g->rang = REALLOC(g->rang,Ident,g->taille_max_rang);

			
		}

		for (Nat i = g->nb_individus; i > pos; i--)
		{
			g->tab[i] = g->tab[i-1]; 
			g->rang[g->tab[i]->id-1]=i; 
		}

		g->tab[pos] = Individu_to_add;  
		g->rang[Individu_to_add->id - 1] = pos; 
		g->nb_individus+=1; 
        g->id_cur+=1; 
	}

	


// PRE: getByIdent(g,x)!=NULL) && getByIdent(g,filsa)!=NULL &&  (pp!=omega || mm!=omega)
void adjFils(Genealogie g, Ident idx, Ident fils, Ident p, Ident m) {
    // Sécurité de base : éviter de faire de soi-même son propre frère
    if (idx == fils) return;

    Individu enfant = getByIdent(g, idx);
    if (!enfant) return;

    Individu pere = (getByIdent(g, idx)->ipere != omega) ? getByIdent(g, getByIdent(g, idx)->ipere) : getByIdent(g, getByIdent(g, fils)->ipere);
    Individu mere = (getByIdent(g, idx)->imere != omega) ? getByIdent(g, getByIdent(g, idx)->imere) : getByIdent(g, getByIdent(g, fils)->imere);
    
    // Récupération du premier enfant existant (aîné)
    Individu premier_fils = getByIdent(g, fils);

    // Associer les parents
    if (pere) enfant->ipere = pere->id;
    if (mere) enfant->imere = mere->id;

    // Cas sans frères/soeurs existants
    if (!premier_fils || compDate(enfant->naiss, premier_fils->naiss) < 0) {
        enfant->icadet = premier_fils ? premier_fils->id : omega;
        if (pere) pere->ifaine = enfant->id;
        if (mere) mere->ifaine = enfant->id;
    } else {
        // Insertion dans la bonne position selon la date de naissance
        Individu precedent = premier_fils;
        Individu suivant = getByIdent(g, precedent->icadet);

        while (suivant && compDate(enfant->naiss, suivant->naiss) >= 0) {
            if (suivant->id == idx) return; // déjà présent
            precedent = suivant;
            suivant = getByIdent(g, suivant->icadet);
        }

        // Insertion dans la fratrie
        enfant->icadet = precedent->icadet;
        precedent->icadet = enfant->id;

        // Réassigner ifaine si nécessaire
        if (pere && pere->ifaine != omega) pere->ifaine = premier_fils->id;
        if (mere && mere->ifaine != omega) mere->ifaine = premier_fils->id;
    }

    Individu courant = find_oldest(g, fils);

    while(courant != NULL) {
        courant->ipere = (pere != NULL) ? pere->id : omega;
        courant->imere = (mere != NULL) ? mere->id : omega;
        courant = getByIdent(g, courant->icadet);
    }
}

	


//PRE:  (p==omega || getByIdent(g,p)!=NULL) && (m==omega || getByIdent(g,m)!=NULL) &&
//      !freres_soeurs(g,p,m) && getByIdent(g,p)->ifaine == getByIdent(g,m)->ifaine &&
//      compDate(getByIdent(g,p)->naiss,n)<0 && compDate(getByIdent(g,m)->naiss,n)<0
Ident adj(Genealogie g, Chaine s, Ident p, Ident m, Date n, Date d)
{
    Nat pos = getPos(g, s);
    Ident id = g->id_cur;
    insert(g, pos, s, p, m, n, d);

    Individu enfant = getByIdent(g, id);
    if (!enfant) return omega;

    // Si les parents existent et ont déjà un premier enfant, adjFils est appelé une seule fois.
    if (p != omega && getByIdent(g, p)->ifaine != omega) {
        adjFils(g, id, getByIdent(g, p)->ifaine, p, m);
    }
    else if (m != omega && getByIdent(g, m)->ifaine != omega) {
        adjFils(g, id, getByIdent(g, m)->ifaine, p, m);
    }
    else {
        // Aucun des parents n'a encore d'enfants -> définir ifaine pour les deux parents
        if (p != omega) getByIdent(g, p)->ifaine = id;
        if (m != omega) getByIdent(g, m)->ifaine = id;
    }

    return id;
}





// 
/// PARTIE 2: Affichages 
/// ///////////////////////////////////////////////////////
/// 

//PRE: None
void affiche_freres_soeurs(Genealogie g, Ident x, Chaine buff)
{	
	buff[0] = '\0';
	if (x == omega || g == NULL) return; 
	
	Individu idx = getByIdent(g,x); 
	 
	Individu parent = NULL;
	if(idx->ipere != omega) parent = getByIdent(g,idx->ipere); 
	if (idx->imere != omega) parent = getByIdent(g,idx->imere); 
	if (parent != NULL)
	{
			
	Individu aine = getByIdent(g,parent->ifaine); 

	if ( aine != NULL)
	{
		while ( aine != omega)
		{	
			if (chaineCompare(aine->nom,idx->nom) != 0)
			{
				chaineConcat(buff,aine->nom); 
				chaineConcat(buff, " "); 
			}
			aine = getByIdent(g,aine->icadet); 
			
		}
		
	}
	
	}

	
	
	
	
}

//PRE: None
void affiche_enfants(Genealogie g, Ident x, Chaine buff)
{
	buff[0] = '\0';
	if (x == omega || g == NULL) return; 
	Individu parent = getByIdent(g,x);
	Individu aine = getByIdent(g,parent->ifaine); 
	if ( aine != NULL)
	{
		while ( aine != omega)
		{	
			chaineConcat(buff,aine->nom); 
			chaineConcat(buff, " "); 
			aine = getByIdent(g,aine->icadet); 
			
		}
		
	}
	
}

//PRE: None
void affiche_cousins(Genealogie g, Ident x, Chaine buff)
{
	buff[0] = '\0';
	if (x == omega || g == NULL) return; 
	Individu idx = getByIdent(g,x);

	if (idx->imere != omega) {
		Individu mere = getByIdent(g,idx->imere);

		Individu parent = NULL;
		if (mere->ipere != omega) parent = getByIdent(g,mere->ipere);
		else if (mere->imere != omega) parent = getByIdent(g,mere->imere);
		if (parent != NULL) {
			Individu aine = getByIdent(g, parent->ifaine);
			
			if (aine != NULL) {
				while (aine != omega) {
					if (aine != mere)
					{
					affiche_enfants(g, aine->id, buff); 
					}
					aine = getByIdent(g, aine->icadet); 
					
	
				}
			}
		}
	} 
	
	if (idx->ipere != omega) {
		Individu pere = getByIdent(g,idx->ipere);

		Individu parent = NULL;
		if (pere->ipere != omega) parent = getByIdent(g,pere->ipere);
		else if (pere->imere != omega) parent = getByIdent(g,pere->imere);
		if (parent != NULL) {
			Individu aine = getByIdent(g, parent->ifaine); 
			if (aine != NULL) {
				while (aine != omega) {
					if (aine != pere)
					{
					affiche_enfants(g, aine->id, buff); 
					}
					aine = getByIdent(g, aine->icadet); 
					
				}
			}
		}
	} 
}

//PRE: None
void affiche_oncles(Genealogie g, Ident x, Chaine buff){

	buff[0] = '\0';
	if (x == omega || g == NULL) return; 
	Individu idx = getByIdent(g,x); 
	if (idx->ipere != omega)
	{
		affiche_freres_soeurs(g,idx->ipere,buff); 
	
	}
	if (idx->imere != omega)
	{	
		affiche_freres_soeurs(g,idx->imere,buff); 

	}
	
}

// 
/// PARTIE 3: Cr�er Liens de parente 
/// ///////////////////////////////////////////////////////
/// 
// PRE (getByIdent(g,x)->imere != NULL || getByIdent(g,x)->ipere != NULL) && (getByIdent(g,y)->imere != NULL || getByIdent(g,y)->ipere != NULL) 
// && (getByIdent(g,x)->imere == getByIdent(g,y)->imere || getByIdent(g,x)->ipere == getByIdent(g,y)->ipere  )
void deviennent_freres_soeurs(Genealogie g, Ident x, Ident y) {
    if(g == NULL) return;

    Individu x1 = getByIdent(g, x);
    Individu y1 = getByIdent(g, y);
    if (!x1 || !y1) return;

    Ident pere_commun = (getByIdent(g, x)->ipere != omega) ? getByIdent(g, x)->ipere : getByIdent(g, y)->ipere;
    Ident mere_commun = (getByIdent(g, x)->imere != omega) ? getByIdent(g, x)->imere : getByIdent(g, y)->imere;

    Individu fratrie_x = find_oldest(g, x);
    Individu courant;
    while (fratrie_x != NULL) {
        courant = getByIdent(g, fratrie_x->icadet);
        adjFils(g, fratrie_x->id, find_oldest(g, y)->id, pere_commun, mere_commun);
        fratrie_x = courant;
    }
}
// PRE getByIdent(g,x) != NULL && !freres_soeurs(getByIdent(g,x),getByIdent(g,y)->imere)
void devient_pere(Genealogie g, Ident x, Ident y)
{
    Individu pere = getByIdent(g, x);
    Individu fils = getByIdent(g, y);
    
    if (!pere || !fils) return; // Vérification de la validité des individus

    if(pere->ifaine == omega) {
        Individu courant = find_oldest(g, y);
        pere->ifaine = find_oldest(g, y)->id;
        while(courant != NULL) {
            courant->ipere = x;
            courant = getByIdent(g, courant->icadet);
        }
    } else {
        deviennent_freres_soeurs(g, y, pere->ifaine);
    }
}

void devient_mere(Genealogie g, Ident x, Ident y)
{
    Individu mere = getByIdent(g, x);
    Individu fils = getByIdent(g, y);
    
    if (!mere || !fils) return; // Vérification de la validité des individus

    if(mere->ifaine == omega) {
        Individu courant = find_oldest(g, y);
        mere->ifaine = find_oldest(g, y)->id;
        while(courant != NULL) {
            courant->imere = x;
            courant = getByIdent(g, courant->icadet);
        }
    } else {
        deviennent_freres_soeurs(g, y, mere->ifaine);
    }
}




// 
/// PARTIE 4: Parcours de la genealogie 
/// ///////////////////////////////////////////////////////
/// 

//PRE: None
Bool estAncetre(Genealogie g, Ident x, Ident y) {
    if (g == NULL || x == omega || y == omega) return faux;

    Individu indX = getByIdent(g, x);
    Individu indY = getByIdent(g, y);
    
    // Vérification si les individus existent
    if (indX == NULL || indY == NULL) return faux;

    // Si x est le père ou la mère de y, alors x est un ancêtre
    if (x == indY->ipere || x == indY->imere) {
        return vrai;
    }

    // Si y n'a pas de père et de mère, x ne peut pas être un ancêtre
    if (indY->ipere == omega && indY->imere == omega) {
        return faux;
    }

    // Récursion : vérifier si l'un des parents de y est un ancêtre de x
    Bool pere_est_ancetre = (indY->ipere != omega) ? estAncetre(g, x, indY->ipere) : faux;
    Bool mere_est_ancetre = (indY->imere != omega) ? estAncetre(g, x, indY->imere) : faux;

    return pere_est_ancetre || mere_est_ancetre;
}

//PRE: None
Bool ontAncetreCommun(Genealogie g, Ident x, Ident y) {
    if(g == NULL || x == omega || y == omega) return faux;
    Individu indX = getByIdent(g, x);
    Individu indY = getByIdent(g, y);

    return estAncetre(g, indX->id, y) || estAncetre(g, indY->id, x) 
        || ontAncetreCommun(g, indX->ipere, y) || ontAncetreCommun(g, indX->imere, y)
        || ontAncetreCommun(g, indY->ipere, x) || ontAncetreCommun(g, indY->imere, x);
}



//PRE: None
Ident plus_ancien(Genealogie g, Ident x) {
    if(g == NULL) return omega;
    Individu perso = getByIdent(g, x);
    if (!perso) {
        return omega;
    }
    // Si l'individu n'a pas de parents, il est le plus ancien
    if (perso->ipere == omega && perso->imere == omega) {
        return perso->id;
    }
    // Chercher le plus ancien ancêtre du père et de la mère
    Ident ancetre_pere = (perso->ipere != omega) ? plus_ancien(g, perso->ipere) : omega;
    Ident ancetre_mere = (perso->imere != omega) ? plus_ancien(g, perso->imere) : omega;

    Individu pAncien = getByIdent(g, ancetre_pere);
    Individu mAncien = getByIdent(g, ancetre_mere);

    // Trouver et retourner le plus ancien entre le père et la mère
    if (!pAncien) return ancetre_mere;
    if (!mAncien) return ancetre_pere;

    Ident plusAncien = (compDate(pAncien->naiss, mAncien->naiss) <= 0) ? ancetre_pere : ancetre_mere;
    return plusAncien;
}


//PRE: None
void affiche_parente(Genealogie g, Ident x, Chaine buff) {
	buff[0] = '\0';
	if (g == NULL || g->nb_individus < 1 || x == omega) { buff[0] = '\0'; return; }
	
	// file
	Individu file[g->nb_individus];
	Nat head = 0;
	Nat cardinal = 0;

	Nat fileAux[g->nb_individus];
	Nat headAux = 0;
	Nat cardinalAux = 0;

	Individu ind = NULL;
	
	Nat max_level = 0;
	Nat pre_level = 0;

	file[(head + cardinal) % g->nb_individus] = getByIdent(g, x);
	cardinal++;
	fileAux[(headAux + cardinalAux) % g->nb_individus] = max_level;
	cardinalAux++;
	
	while (cardinal != 0) {
		ind = file[head];
		head = (head + 1) % g->nb_individus;
		cardinal--;
		max_level = fileAux[headAux];
		headAux = (headAux + 1) % g->nb_individus;
		cardinalAux--;
		
		if (max_level > pre_level && ind->id != x) {
			if (pre_level != 0) chaineConcat(buff, "\n- ");
			else chaineConcat(buff, "- ");

			buff += strlen(buff);			// - n :
			NatToChaine(max_level, buff);	//
			chaineConcat(buff, " :\n");		//
			
			chaineConcat(buff, ind->nom);
			chaineConcat(buff, " ");
			buff += strlen(buff);
			affiche_freres_soeurs(g, ind->id, buff);

			pre_level++;
		} else if (ind->id != x) {
			chaineConcat(buff, ind->nom);
			chaineConcat(buff, " ");
			buff += strlen(buff);
			affiche_freres_soeurs(g, ind->id, buff);
		}



		if (ind->ipere != omega || ind->imere != omega) {
			if (ind->ipere != omega) {
				file[(head + cardinal) % g->nb_individus] = getByIdent(g, ind->ipere);
				cardinal++;
				fileAux[(headAux + cardinalAux) % g->nb_individus] = max_level+1;
				cardinalAux++;
			}

			if (ind->imere != omega) {
				file[(head + cardinal) % g->nb_individus] = getByIdent(g, ind->imere);
				cardinal++;
				fileAux[(headAux + cardinalAux) % g->nb_individus] = max_level+1;
				cardinalAux++;
			}
		}
	}
}

//PRE: None
void affiche_descendance(Genealogie g, Ident x, Chaine buff) {
	buff[0] = '\0';
	if (g == NULL || x == omega) return;

	// Fifo
	Individu file[g->nb_individus];
	Nat tete = 0;
	Nat n_elem = 0;

	Nat fileAux[g->nb_individus];
	Nat teteAux = 0;
	Nat n_elemAux = 0;

	Individu ind = NULL;
	Individu courant = NULL;

	Nat max_level = 0;
	Nat pre_level = 0;

	file[(tete + n_elem) % g->nb_individus] = getByIdent(g, x);
	n_elem++;
	fileAux[(teteAux + n_elemAux) % g->nb_individus] = max_level;
	n_elemAux++;

	while (n_elem != 0) {
		ind = file[tete];
		tete = (tete + 1) % g->nb_individus;
		n_elem--;
		max_level = fileAux[teteAux];
		teteAux = (teteAux + 1) % g->nb_individus;
		n_elemAux--;

		if (max_level > pre_level && ind->id != x) {
			if (pre_level != 0) chaineConcat(buff, "\n- ");
			else chaineConcat(buff, "- ");

			buff += strlen(buff);			// Affiche !
			NatToChaine(max_level, buff);	// - n :
			chaineConcat(buff, " :\n");		//

			chaineConcat(buff, ind->nom);
			chaineConcat(buff, " ");

			pre_level++;
		} else if (ind->id != x) {
			chaineConcat(buff, ind->nom);
			chaineConcat(buff, " ");
		}


		if (ind->ifaine != omega) {
			courant = getByIdent(g, ind->ifaine);

			while (courant != NULL) {
				// Enfiler
				file[(tete + n_elem) % g->nb_individus] = getByIdent(g, courant->id);
				n_elem++;
				fileAux[(teteAux + n_elemAux) % g->nb_individus] = max_level+1;
				n_elemAux++;

				courant = getByIdent(g, courant->icadet);
			}
		}
	}
}

// 
/// VOS FONCTIONS AUXILIAIRES 
/// ///////////////////////////////////////////////////////
//


void afficheArbre(Genealogie g) {
 Nat i;

 printf("\n-------------------------------------------------------------\n\n");
 printf("- Tab :\nnom[LG_MAX] naiss    deces     id  ipere  imere  icadet ifaine\n");
 for (i = 0; i < g->nb_individus; i++) {
 printf("%11.10s %.4d/%.2d/%.2d %.4d/%.2d/%.2d %2.2d %5.2d %5.2d %6.2d %6.2d\n", g->tab[i]->nom, g->tab[i]->naiss.annee, g->tab[i]->naiss.mois, g->tab[i]->naiss.jour, g->tab[i]->deces.annee, g->tab[i]->deces.mois, g->tab[i]->deces.jour, g->tab[i]->id, g->tab[i]->ipere, g->tab[i]->imere, g->tab[i]->icadet, g->tab[i]->ifaine);
 }

 printf("\n\n- Rang :\n");
 for (i = 0; i < g->nb_individus; i++) {
 printf("%d ", g->rang[i]);
 }

 printf("\n\n-------------------------------------------------------------\n\n");
}
/// 

Bool est_freres_soeurs(Genealogie g, Ident x, Ident y){

	Individu idx = getByIdent(g,x); 
	Individu idy = getByIdent(g,y); 
	return idx->imere == idy->ipere || idx->ipere == idy->ipere; 
	
}

Individu getByDate(Genealogie g, Date d){
	Ent droite = g->nb_individus; 
	Ent gauche = 0; 
	Ent milieu ;

	while (gauche < droite)
	{
		milieu = (gauche + droite)/2; 
		if (compDate(d,g->tab[milieu]->naiss) == 0)
		{
			return g->tab[milieu]; 
		}
		else if (compDate(d,g->tab[milieu]->naiss) < 0)
		{
			droite = milieu; 
		}
		else {
			gauche = milieu;  
		}
		
		
	}
	
}

Individu find_oldest(Genealogie g, Ident x) {
    if(g == NULL) return NULL;

    Individu x1 = getByIdent(g, x);
    if(x1 == NULL) return NULL;

    if(x1->ipere != omega && getByIdent(g, x1->ipere)->ifaine != omega) 
		return getByIdent(g, getByIdent(g, x1->ipere)->ifaine);
    if(x1->imere != omega && getByIdent(g, x1->imere)->ifaine != omega) 
		return getByIdent(g, getByIdent(g, x1->imere)->ifaine);
    return x1;
}

// 
/// MAIN
/// ///////////////////////////////////////////////////////
/// 

int main()
{
	Car buf[500];
	Genealogie g;

	printf("******* Init:\n\n");
	genealogieInit(&g);
	printf("done.\n");

	printf("\n******* adj+getByIndent:\n");
	Date dnull = { 0,0,0 };

	// Famille Potter
	Date jhen = { 16,2,1867 }; Date jhed = { 21,11,1932 };
	Ident ihep = adj(g, "Henri", 0, 0, jhen, jhed);

	Date jfn = { 30,7,1905 }; Date jfd = { 2,3,1979 };
	Ident ijfl = adj(g, "Fleamont", ihep, 0, jfn, jfd);

	Date jeu = { 12,6,1907 }; Date jed = { 14, 1,1979 };
	Ident ijm = adj(g, "Euphemia", 0, 0, jeu, jed);


	Date jpn = { 27, 3, 1960 }; Date jpd = { 29, 7, 1981 };
	Ident ijp = adj(g, "James", ijfl, ijm, jpn, jpd);


	Date lpn = { 30, 1, 1960 }; Date lpd = { 29, 7, 1981 };
	Ident ilp = adj(g, "Lily", 0, 0, lpn, lpd);

	Date hn = { 31, 7, 1980 };
	Ident ih = adj(g, "Harry", ijp, ilp, hn, dnull);

	// Famille Weasley
	Date an = { 6, 2, 1950 };
	Ident iaw = adj(g, "Arthur", 0, 0, an, dnull);


	Date dpre = { 8,4, 1910 }; Date ddpre = { 23, 10, 1968 };
	Ident ipre = adj(g, "Prewett", 0, 0, dpre, ddpre);

	Date dfab = { 12, 5, 1946 }; Date ddfab = { 21,12, 1982 };
	Ident ifab = adj(g, "Fabian", ipre, 0, dfab, ddfab);

	Date mn = { 30, 10, 1949 };
	Ident imw = adj(g, "Molly", ipre, 0, mn, dnull);


	// ajouter ici les autres individus
	Date gn = {11,8,1981}; 
	Ident ig = adj(g,"Ginny",iaw,imw,gn,d_null);  // Ginny

	Date rn = {1,3,1980};
	Ident ir = adj(g,"Ron",iaw,imw,rn,d_null); // Ron

	Date hr = {19,9,1980}; 
	Ident ihg = adj(g,"Hermione",0,0,hr,dnull); // Hermione


	Date bl = {29,11,1970}; 
	Ident ibill = adj(g,"Bill",iaw,imw,bl,d_null); // Bill


	Date rs = {6,8,2006};
	Ident irose = adj(g,"Rose",ir,ihg,rs,d_null); // Rose

	Date al = {15,8,2006}; 
	Ident ia2 = adj(g,"Albus",ih,ig,al,d_null ); // Albus

	Date js = {1,5,2005}; 
	Ident ij = adj(g,"James",ih,ig,js,dnull); // James (son of Harry)


	Date fr = {1,4,1978};
	Date fr_deces = {5,6,1998}; 
	Ident ifr = adj(g,"Fred",iaw,imw,fr,fr_deces); 

	Date gg = {1,4,1978};
	Ident igg = adj(g,"George",iaw,imw,gg,dnull); 



	Date ll = {2,5,2008};
	Ident ill = adj(g,"Lily",ih,ig,ll,dnull); // lily (son of harry)

	for (Nat i = 0; i < cardinal(g); i++) {
		printf("%s\n", nomIndividu(kieme(g,i)));
	}

	

	
	printf("nb_individus: %d\n", cardinal(g));
	printf("Identifiant de Fabian: %u (must be 9)\n", ifab);
	printf("Identifiant de Arthur: %u (must be 7)\n", iaw); 

	
	



	printf("\nAdding more people:\n");
	Date dgid = { 7, 2, 1945 }; Date ddgid = { 21,12, 1982 };
	Ident igid = adj(g, "Gideon", 0, 0, dgid, ddgid);
	printf("Linking Gideon as brother of Molly\n");
	deviennent_freres_soeurs(g, igid, imw);
	Date dhugo = { 19,4,2008 };
	Ident ihugo = adj(g, "Hugo", 0, 0, dhugo, dnull);
	printf("Linking Hugo as son of Hermione\n");
	devient_mere(g, ihg, ihugo);

	printf("\nTry to add a double Harry:\n");
	Date hu2n = { 31, 7, 1980 };
	Ident ihu2 = adj(g, "Harry", 0, 0, hu2n, dnull);
	if (ihu2 != omega) printf("oh, no! Added double Harry!\n"); else printf("No double Harry: OK!\n");
	printf("Adding more Albus...\n");
	Date ho2n = { 1,8, 1909 };
	Ident itho2 = adj(g, "Albus", 0, 0, ho2n, dnull);
	if (itho2 == omega) printf("what! Could not add 2nd Albus!\n"); else printf("2nd Albus added: OK!\n");
	Date ho3n = { 2,2, 1832 };
	Ident itho3 = adj(g, "Albus", 0, 0, ho3n, dnull);
	if (itho3 == omega) printf("what! Could not add 3rd Albus!\n"); else printf("3rd Albus added: OK!\n");
	Individu idch = getByName(g, "Albus", dnull);
	if (idch != NULL) printf("Youngest Albus, date birth %d:%d:%d\n", naissIndividu(idch).jour, naissIndividu(idch).mois, naissIndividu(idch).annee);
	else printf("what? no Albus! There is a serious problem here...\n");
	printf("Now nb_individus: %d\n", cardinal(g));


	printf("\n******* fratrie:\n");
	printf("Freres/Soeurs de %s:\n", nomIndividu(getByIdent(g, ig)));
	buf[0] = 0;  affiche_freres_soeurs(g, ig, buf);
	printf("%s\n", buf);
	printf("Freres/Soeurs de %s:\n", nomIndividu(getByIdent(g, imw)));
	buf[0] = 0;  affiche_freres_soeurs(g, imw, buf);
	printf("%s\n", buf);

	printf("\n******* les enfants:\n");
	printf("Enfants de %s:\n", nomIndividu(getByIdent(g, ipre)));
	buf[0] = 0;  affiche_enfants(g, ipre, buf);
	printf("%s\n", buf);
	printf("Enfants de %s:\n", nomIndividu(getByIdent(g, imw)));
	buf[0] = 0;  affiche_enfants(g, imw, buf);
	printf("%s\n", buf);
	printf("Enfants de %s:\n", nomIndividu(getByIdent(g, ir)));
	buf[0] = 0;  affiche_enfants(g, ir, buf);
	printf("%s\n", buf);

	printf("\n******* les cousins:\n");
	printf("Cousins de %s:\n", nomIndividu(getByIdent(g, irose)));
	buf[0] = 0;   affiche_cousins(g, irose, buf);
	printf("%s\n", buf);

	printf("\n******* les oncles et tantes:\n");
	printf("Oncles/tantes de %s:\n", nomIndividu(getByIdent(g, ij)));
	buf[0] = 0;  affiche_oncles(g, ij, buf);
	printf("%s\n", buf);
	printf("Oncles/tantes de %s:\n", nomIndividu(getByIdent(g, ir)));
	buf[0] = 0;  affiche_oncles(g, ir, buf);
	printf("%s\n", buf);




	printf("\n******* les ancetres:\n");
	printf("%s ancetre de %s: %s\n", nomIndividu(getByIdent(g, ijfl)), nomIndividu(getByIdent(g, ia2)), estAncetre(g, ijfl, ia2) ? "oui" : "non");
	printf("%s ancetre de %s: %s\n", nomIndividu(getByIdent(g, ijfl)), nomIndividu(getByIdent(g, irose)), estAncetre(g, ijfl, irose) ? "oui" : "non");

	printf("\n******* les ancetres communs:\n");
	printf("%s et %s ont ancetre commun: %s\n", nomIndividu(getByIdent(g, irose)), nomIndividu(getByIdent(g, ij)), ontAncetreCommun(g, irose, ij) ? "oui" : "non");
	printf("%s et %s ont ancetre commun: %s\n", nomIndividu(getByIdent(g, ih)), nomIndividu(getByIdent(g, ibill)), ontAncetreCommun(g, ih, ibill) ? "oui" : "non");

	printf("\n******* ancetre plus ancien:\n");
	printf("L'ancetre le plus ancien de %s est %s\n", nomIndividu(getByIdent(g, ia2)), nomIndividu(getByIdent(g, plus_ancien(g, ia2))));
	
	printf("\n******* parente:\n");
	printf("parents de %s\n", nomIndividu(getByIdent(g, ia2)));
	buf[0] = 0; affiche_parente(g, ia2, buf);
	printf("%s\n", buf);

	printf("\n******* descendence:\n");
	printf("descendence de %s\n", nomIndividu(getByIdent(g, ijm)));
	buf[0] = 0;  affiche_descendance(g, ijm, buf);
	printf("%s\n", buf); 

	printf("\n******* free:\n");
	genealogieFree(&g);
	printf("fin.(press key)\n");
	// fgets(buf, 2, stdin);

	return 0;
}

