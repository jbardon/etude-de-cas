#include "GestionEnv.h"

void GestionEnv_creerPanier(cpSpace* espace, SDL_Surface* surf, int dec){
 
	int x = dec;
	int y = dec;
	int l = LARGUEUR_ECRAN - 2*dec;
	int h = 0;
	int e = 10; //epaisseur du trait (SDL)

	//Création du sol qui est un élément statique---------------------------------
	panier[0] = cpSegmentShapeNew(espace->staticBody, cpv(0,y), cpv(LARGUEUR_ECRAN,y), 0);
	cpShapeSetFriction(panier[0], 1);
	cpSpaceAddShape(espace, panier[0]);

	y = HAUTEUR_ECRAN - y + e/2;
	thickLineColor(surf, x - e, y, x + l + e, y, e, 0x000000FF);

	//Création du mur gauche------------------------------------------------------
	x = dec;
	panier[1] = cpSegmentShapeNew(espace->staticBody, cpv(x,0), cpv(x, HAUTEUR_ECRAN), 0);
	cpShapeSetFriction(panier[1], 1);
	cpSpaceAddShape(espace, panier[1]);

	x = dec - e/2;
	y = HAUTEUR_ECRAN - dec;
	h = 2 * dec;
	thickLineColor(surf, x, y + e, x, h, e, 0x00000FF);

	//Création du mur droit-------------------------------------------------------
	x = LARGUEUR_ECRAN - dec;
	panier[2] = cpSegmentShapeNew(espace->staticBody, cpv(x,0), cpv(x, HAUTEUR_ECRAN), 0);
	cpShapeSetFriction(panier[2], 1);
	cpSpaceAddShape(espace, panier[2]);

	x = x + e/2;
	y = HAUTEUR_ECRAN - dec;
	h = 2 * dec;
	thickLineColor(surf, x, y + e, x, h, e, 0x000000FF);

/* DEBUT DEBUG */
lineColor(surf, 0, HAUTEUR_ECRAN - dec, LARGUEUR_ECRAN, HAUTEUR_ECRAN - dec, 0x00FFFF);
lineColor(surf, dec, 0, dec, HAUTEUR_ECRAN, 0x00FFFF);
lineColor(surf, LARGUEUR_ECRAN - dec, 0, LARGUEUR_ECRAN - dec, HAUTEUR_ECRAN, 0x00FFFF);
/* FIN DEBUG */
}

void GestionEnv_supprimerPanier(){
	for(int i = 0; i < sizeof(panier); i++){
		cpShapeFree(panier[i]);
	}
}
