#include "GestionEnv.h"

//-------------------------------------------------------------------------------------------------------------
//										Initialisation de l'environnement
//-------------------------------------------------------------------------------------------------------------
cpSpace* GestionEnv_initChipmunk(){

	srand(time(NULL));	/* A VOIR */
	espace = cpSpaceNew();
	cpVect gravite = cpv(0, -100);  
	cpSpaceSetGravity(espace, gravite);

	return espace;
}

SDL_Surface* GestionEnv_initSDL(){

	SDL_Init(SDL_INIT_VIDEO);
	ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);
	SDL_FillRect(ecran, NULL, COULEUR_FOND);
	SDL_WM_SetCaption("Projet étude de cas", NULL);

	return ecran;	
}

//-------------------------------------------------------------------------------------------------------------
//										Suppression de l'environnement
//-------------------------------------------------------------------------------------------------------------
void GestionEnv_quitChipmunk(){
	cpSpaceFree(espace);
}

void GestionEnv_quitSDL(){
	SDL_FreeSurface(ecran);
	SDL_Quit();
}

//-------------------------------------------------------------------------------------------------------------
//										Gestion du panier
//-------------------------------------------------------------------------------------------------------------
void GestionEnv_creerPanier(cpSpace* espace, SDL_Surface* surf){
 
	int x = OFFSET;
	int y = OFFSET;
	int l = LARGUEUR_ECRAN - 2*OFFSET;
	int h = 0;
	int e = 10; //epaisseur du trait (SDL)

	//Création du sol qui est un élément statique---------------------------------
	panier[0] = cpSegmentShapeNew(espace->staticBody, cpv(0,y), cpv(LARGUEUR_ECRAN,y), 0);
	cpShapeSetFriction(panier[0], 1);
	cpShapeSetElasticity(panier[0], 1);
	cpSpaceAddShape(espace, panier[0]);

	y = HAUTEUR_ECRAN - y + e/2;
	thickLineColor(surf, x - e, y, x + l + e, y, e, 0x000000FF);

	//Création du mur gauche------------------------------------------------------
	x = OFFSET;
	panier[1] = cpSegmentShapeNew(espace->staticBody, cpv(x,0), cpv(x, HAUTEUR_ECRAN), 0);
	cpShapeSetFriction(panier[1],1);
	cpShapeSetElasticity(panier[1],1);
	cpSpaceAddShape(espace, panier[1]);

	x = OFFSET - e/2;
	y = HAUTEUR_ECRAN - OFFSET;
	h = 2 * OFFSET;
	thickLineColor(surf, x, y + e, x, h, e, 0x00000FF);

	//Création du mur droit-------------------------------------------------------
	x = LARGUEUR_ECRAN - OFFSET;
	panier[2] = cpSegmentShapeNew(espace->staticBody, cpv(x,0), cpv(x, HAUTEUR_ECRAN), 0);
	cpShapeSetFriction(panier[2],1);
	cpShapeSetElasticity(panier[2],1);
	cpSpaceAddShape(espace, panier[2]);

	x = x + e/2;
	y = HAUTEUR_ECRAN - OFFSET;
	h = 2 * OFFSET;
	thickLineColor(surf, x, y + e, x, h, e, 0x000000FF);

}

void GestionEnv_supprimerPanier(){
	for(int i = 0; i < sizeof(panier); i++){
		cpShapeFree(panier[i]);
	}
}

//-------------------------------------------------------------------------------------------------------------
//										Gestion des balles
//-------------------------------------------------------------------------------------------------------------

static unsigned int _randMinMax(int min, int max){
	return (rand() % (max-min)) + min;
}

void GestionEnv_creerBalles(int nbBalles){

	balles = calloc(nbBalles, sizeof(Balle*));

	if(balles){

		//Créé les balles
		for(int i = 0; i < nbBalles; i++){			
			//Caractéritiques aléatoires pour les balles
			const unsigned int rayon = _randMinMax(25,50);
			const cpVect centre = cpv(_randMinMax(OFFSET + rayon, LARGUEUR_ECRAN - 2*OFFSET - rayon), /*-rayon*/100);
			const cpVect direction = cpv(-_randMinMax(40,100), _randMinMax(40,100));
			const char lettre = (char) _randMinMax(65,90);

			balles[i] = Balle_creer(ecran, espace, centre, direction, rayon, 0xFF0000FF, lettre);			
printf("Balle #%d: centre(%f,%f), rayon(%d), lettre(%c)\n", i, centre.x, centre.y, rayon, lettre);
		}
	}
}

void GestionEnv_supprimerBalles(){

	unsigned int nbBalles = sizeof(balles);

	for(unsigned int i = 0; i < nbBalles; i++){
		Balle_supprimer(balles[0]);
printf("Supression balle #%d/%d\n", i, nbBalles);
	}
}
