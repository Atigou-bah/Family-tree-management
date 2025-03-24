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
	*g = MALLOC(struct s_genealogie); 
    (*g)->id_cur = 1; 
    (*g)->nb_individus = 0; 
    (*g)->tab = MALLOCN(Individu,TAILLE_MAX_TAB); 
    (*g)->rang = MALLOCN(Ident, TAILLE_MAX_RANG); 
    (*g)->taille_max_rang = TAILLE_MAX_RANG; 
    (*g)->taille_max_tab = TAILLE_MAX_TAB;  

}

//PRE: None
void genealogieFree(Genealogie *g)
{
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
Chaine nomIndividu(Individu ind) { if (ind != NULL) {return ind->nom;} }
Date naissIndividu(Individu ind) { if (ind != NULL){ return ind->naiss; } return d_null ;}
Nat cardinal(Genealogie g) { return g->nb_individus; }
Individu kieme(Genealogie g, Nat k) { return g->tab[k]; }

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
    // Si naissance est égale à {0,0,0}, rechercher le plus jeune individu avec le nom
    Date date_0 = {0,0,0}; 

    Nat pos = getPos(g, name);

    // Si naissance est {0,0,0}, rechercher le plus jeune individu avec ce nom
    if (compDate(date_0, naissance) == 0) {
        Individu result = NULL;  

        // Parcourir les individus à partir de la position retournée par getPos
        while (pos < g->nb_individus && chaineCompare(g->tab[pos]->nom, name) == 0) {

            if (result == NULL || compDate(g->tab[pos]->naiss, result->naiss) > 0) {
                result = g->tab[pos];  
            }
            pos++; 
        }

        return result;  
    }
    else {
     
        while (pos < g->nb_individus && chaineCompare(g->tab[pos]->nom, name) == 0) {
            if (compDate(g->tab[pos]->naiss, naissance) == 0) {
                return g->tab[pos];  
            }
            pos++; 
        }

        return NULL;  // Aucun individu trouvé
    }
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
    // Récupération des individus
    Individu enfant = getByIdent(g, idx);
    Individu pere = (p != omega) ? getByIdent(g, p) : NULL;
    Individu mere = (m != omega) ? getByIdent(g, m) : NULL;
    Individu fils_pere = (pere && pere->ifaine != omega) ? getByIdent(g, pere->ifaine) : NULL;
    Individu fils_mere = (mere && mere->ifaine != omega) ? getByIdent(g, mere->ifaine) : NULL;
    if(fils_pere) {
        if(fils_pere->imere != omega) {
            mere = getByIdent(g, fils_pere->imere);
        }
    }
    if(fils_mere) {
        if(fils_mere->ipere != omega) {
            pere = getByIdent(g, fils_mere->ipere);
        }
    }
    
    Individu aine = getByIdent(g, fils);
    
    if (fils_pere && fils_mere) {
        aine = (compDate(fils_pere->naiss, fils_mere->naiss) < 0) ? fils_pere : fils_mere;
    } else if (fils_pere) {
        aine = fils_pere;
    } else if (fils_mere) {
        aine = fils_mere;
    }
    // Si aucun premier fils, on définit directement
    if (aine == NULL || compDate(enfant->naiss, aine->naiss) < 0) {
        // Met à jour les liens de parentalité
        if (pere) {
            enfant->ipere = pere->id;
            pere->ifaine = idx;
        }
        if (mere) {
            enfant->imere = mere->id;
            mere->ifaine = idx;
        }

        // L'ancien premier fils devient le cadet du nouvel aîné
        enfant->icadet = aine ? aine->id : omega;
        return;
    }

    // Vérification des parents du frère aîné
    if (pere == NULL && aine->ipere != omega) {
        pere = getByIdent(g, aine->ipere);
    }
    
    if (mere == NULL && aine->imere != omega) {
        mere = getByIdent(g, aine->imere);
    }

    // Assigner les parents à l'enfant
    if (pere) {
        enfant->ipere = pere->id;
    }
    if (mere) {
        enfant->imere = mere->id;
    }

    // Parcours de la liste des frères/soeurs
    if (pere) aine->ipere = pere->id;
    if (mere) aine->imere = mere->id;

    Individu precedent = aine;
    Individu suivant = getByIdent(g, precedent->icadet);

    
    while (precedent->icadet != omega && suivant != NULL && compDate(enfant->naiss, suivant->naiss) >= 0) {
        // Comparaison des dates entre l'enfant et le suivant
        if (suivant == enfant) {
            return;
        }
        // Mettre à jour le précédent et suivant pour la prochaine itération
        precedent = suivant;
        suivant = getByIdent(g, precedent->icadet);
    }
    // Insérer l'enfant dans la bonne position
    enfant->icadet = precedent->icadet;
    precedent->icadet = idx;
    if(pere && aine) pere->ifaine = aine->id;
    if(mere && aine) mere->ifaine = aine->id;
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
	Individu idx = getByIdent(g,x); 
	 
	Individu parent = NULL;
	if(idx->ipere != NULL) parent = getByIdent(g,idx->ipere); 
	if (idx->imere != NULL) parent = getByIdent(g,idx->imere); 
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

	Individu idx = getByIdent(g,x); 
	if (idx->ipere != NULL)
	{
		affiche_freres_soeurs(g,idx->ipere,buff); 
	
	}
	if (idx->imere != NULL)
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
void deviennent_freres_soeurs(Genealogie g, Ident x, Ident y)
{
	Individu idx = getByIdent(g,y);
	Individu idy = getByIdent(g,x);
	Individu parentx = NULL; 
	Individu parenty = NULL; 

	if (idx->icadet != NULL && idy->icadet != NULL)
	{ 
		Individu cadetx = getByIdent(g,idx->icadet);
		Individu cadety = getByIdent(g,idy->icadet); 
		if (idx->ipere == omega) parentx = getByIdent(g,idx->imere); 
		else if (idx->imere == omega){ parentx = getByIdent(g,idx->ipere);}
		

		
		adjFils(g,idx->id,parentx->ifaine,idx->ipere, idx->imere); 
		adjFils(g,cadetx->id,parentx->ifaine,idx->ipere, idx->imere); 
		idy->ipere = idx->ipere;
		idy->imere = idx->imere;

		if (idy->ipere == omega) parenty = getByIdent(g,idx->ipere); 
		else if (idy->imere == omega){ parenty = getByIdent(g,idx->imere);}

		adjFils(g,idy->id,parenty->ifaine,idx->ipere, idx->imere); 
		adjFils(g,cadety->id,parenty->ifaine,idx->ipere, idx->imere);
		idy->ipere = idx->ipere;
		idy->imere = idx->imere; 
		cadety->ipere = idx->ipere; 
		cadety->imere = idx->imere; 
		
		
	}
	else {
		if (idx->ipere == omega) parentx = getByIdent(g,idx->imere); 
		else if (idx->imere == omega){ parentx = getByIdent(g,idx->ipere);}

			adjFils(g,idx->id,parentx->ifaine,idx->ipere, idx->imere); 



		
		if (idy->ipere == omega) parenty = getByIdent(g,idx->ipere); 
		else if (idy->imere == omega){ parenty = getByIdent(g,idx->imere);}

		adjFils(g,idy->id,parenty->ifaine,idx->ipere, idx->imere);
		idy->ipere = idx->ipere;
		idy->imere = idx->imere; 

	}
	

}

// PRE getByIdent(g,x) != NULL && !freres_soeurs(getByIdent(g,x),getByIdent(g,y)->imere)
void devient_pere(Genealogie g, Ident x, Ident y)
{
	Individu pere = getByIdent(g,x); 
	Individu fils = getByIdent(g,y); 

	if (fils->icadet != NULL)
	{
		Individu cadet = getByIdent(g,fils->icadet); 
		adjFils(g,fils->id,pere->ifaine,pere->id,fils->imere); // on ajoute d'abord le fils 
		adjFils(g,cadet->id,pere->ifaine,pere->id,cadet->imere); // ensuite le cadet 
		fils->ipere = pere->id; 
		cadet->ipere = pere->id; 
	}
	else {
		adjFils(g,fils->id,pere->ifaine,pere->id,fils->imere);
		fils->ipere = pere->id; 

	}
	

}
void devient_mere(Genealogie g, Ident x, Ident y)
{
	Individu mere = getByIdent(g,x); 
	Individu fils = getByIdent(g,y); 

	if (fils->icadet != NULL)
	{	
		Individu cadet = getByIdent(g,fils->icadet); 
		adjFils(g,fils->id,mere->ifaine,fils->ipere,mere->id); // on ajoute d'abord le fils 
		adjFils(g,cadet->id,mere->ifaine,cadet->ipere,mere->id); // ensuite le cadet 
		fils->imere = mere->id; 
		cadet->imere = mere->id;

	}
	else {
		adjFils(g,y,mere->ifaine,fils->ipere,mere->id);
		fils->imere = mere->id; 

	}
}


// 
/// PARTIE 4: Parcours de la genealogie 
/// ///////////////////////////////////////////////////////
/// 

//PRE: None
Bool estAncetre(Genealogie g, Ident x, Ident y)
{	


	Individu ancetre = getByIdent(g,x); 
	Individu idy = getByIdent(g,y); 
	Individu parentCourant = NULL; 
	if (idy->ipere != omega) 
	{
		parentCourant  = getByIdent(g,idy->ipere); 

	Individu mere = NULL; 
	while(parentCourant != NULL){
		if (parentCourant == ancetre || mere == ancetre)
		{
			return true; 
		}
		mere = getByIdent(g,parentCourant->imere); 
		parentCourant = getByIdent(g,parentCourant->ipere);
		
		
	}
	}
	if (idy->imere != omega)
	{
		parentCourant = getByIdent(g,idy->imere); 
		Individu pere = NULL; 

		while (parentCourant != NULL)
		{
			if (parentCourant == ancetre || pere == ancetre)
			{
				return true;
			}
			pere = getByIdent(g,parentCourant->ipere); 
			parentCourant = getByIdent(g,parentCourant->imere);
		}
		
	}
	



	
	
	return false;
}

//PRE: None
Bool ontAncetreCommun(Genealogie g, Ident x, Ident y)
{	

	Individu idx = getByIdent(g,x); 
	Individu idy = getByIdent(g,y); 

	if (x == omega || y == omega)
	{
		return false; 
	}

	if (est_freres_soeurs(g,x,y))
	{
		return true; 
	}
	

	Individu perex = getByIdent(g,idx->ipere); 
	Individu merex = getByIdent(g, idx->imere); 

	Individu perey = getByIdent(g,idy->ipere); 
	Individu merey = getByIdent(g,idy->imere); 

	

	if (est_freres_soeurs(g,idx->ipere,idy->imere) || est_freres_soeurs(g,idx->ipere,idy->imere) ||
	est_freres_soeurs(g,idx->imere,idy->imere) || est_freres_soeurs(g,idx->imere,idy->ipere))
	{
		return true; 
	}
	if (x == omega && y == omega)
	{
		ontAncetreCommun(g,perex->id,perey->id);
		ontAncetreCommun(g,merex->id,merey->id); 
	}
	return false; 
	

}

//PRE: None
Ident plus_ancien(Genealogie g, Ident x) {    
    Individu idx = getByIdent(g, x); 
    Individu pere = getByIdent(g, idx->ipere); 
    Individu mere = getByIdent(g, idx->imere); 
    Date dateMax = {31, 12, 9999}; 
    Individu plus_ancien = NULL;  

    if (pere != NULL) {
        while (pere != NULL) {

            Date temp;
            if (mere == NULL) {
                // Si la mère est NULL, on compare avec le père uniquement
                temp = pere->naiss;
                if (compDate(temp, dateMax) < 0) {
                    dateMax = temp;
                    plus_ancien = pere;  // Le père devient le plus ancien
                }
            } else {
                // Comparaison des dates du père et de la mère
                temp = compDate(pere->naiss, mere->naiss) < 0 ? pere->naiss : mere->naiss;
                if (compDate(temp, dateMax) < 0) {
                    dateMax = temp;
                    plus_ancien = (compDate(pere->naiss, mere->naiss) < 0) ? pere : mere;
                }
            }


            mere = getByIdent(g, pere->imere); 
            pere = getByIdent(g, pere->ipere); 
        }
    }


    if (mere != NULL) {
        while (mere != NULL) {

            Date temp;
            if (pere == NULL) {
                // Si le père est NULL, on compare avec la mère uniquement
                temp = mere->naiss;
                if (compDate(temp, dateMax) < 0) {
                    dateMax = temp;
                    plus_ancien = mere;  // La mère devient la plus ancienne
                }
            } else {
                // Comparaison des dates du père et de la mère
                temp = compDate(pere->naiss, mere->naiss) < 0 ? pere->naiss : mere->naiss;
                if (compDate(temp, dateMax) < 0) {
                    dateMax = temp;
                    plus_ancien = (compDate(pere->naiss, mere->naiss) < 0) ? pere : mere;
                }
            }

            pere = getByIdent(g, mere->ipere); 
            mere = getByIdent(g, mere->imere); 
        }
    }

        return plus_ancien->id;  

}



//PRE: None
void affiche_parente(Genealogie g, Ident x, Chaine buff)
{
	buff[0] = '\0';
}

//PRE: None
void affiche_descendance(Genealogie g, Ident x, Chaine buff)
{
	buff[0] = '\0';
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
/*
	printf("\n******* parente:\n");
	printf("parents de %s\n", nomIndividu(getByIdent(g, ia2)));
	buf[0] = 0; affiche_parente(g, ia2, buf);
	printf("%s\n", buf);

	printf("\n******* descendence:\n");
	printf("descendence de %s\n", nomIndividu(getByIdent(g, ijm)));
	buf[0] = 0;  affiche_descendance(g, ijm, buf);
	printf("%s\n", buf); 
	*/

	afficheArbre(g);
	printf("\n******* free:\n");
	genealogieFree(&g);
	printf("fin.(press key)\n");
	// fgets(buf, 2, stdin);

	return 0;
}