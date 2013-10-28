#include "GestionEnv.h"

void GestionEnv_creerPanier(cpSpace* espace, SDL_Surface* surf, int dec){
  
  int x = dec;
  int y = dec;
  int l = LARGUEUR_ECRAN - 2*dec;
  int h = 0;
  int e = 11; //epaisseur du trait (SDL)
  
  //Cr�ation du sol qui est un �l�ment statique---------------------------------
  panier[0] = cpSegmentShapeNew(espace->staticBody, cpv(0,y), cpv(LARGUEUR_ECRAN,y), 0);
  cpShapeSetFriction(panier[0], 1);
  cpSpaceAddShape(espace, panier[0]);
  
  y = HAUTEUR_ECRAN - y + e/2;
  thickLineColor(surf, x - e, y, x + l, y, e, SDL_MapRGB(surf->format, 0, 0, 255));

  //Cr�ation du mur gauche------------------------------------------------------
  x = dec;
  panier[1] = cpSegmentShapeNew(espace->staticBody, cpv(x,0), cpv(0, HAUTEUR_ECRAN), 0);
  cpShapeSetFriction(panier[1], 1);
  cpSpaceAddShape(espace, panier[1]);
  
  y = HAUTEUR_ECRAN - dec;
  h = 2 * dec;
  thickLineColor(surf, x - e, y + e, x - e, h, e, SDL_MapRGB(surf->format, 0, 0, 255));

  //Cr�ation du mur droit-------------------------------------------------------
  x = LARGUEUR_ECRAN - dec;
  panier[2] = cpSegmentShapeNew(espace->staticBody, cpv(x,0), cpv(x, HAUTEUR_ECRAN), 0);
  cpShapeSetFriction(panier[2], 1);
  cpSpaceAddShape(espace, panier[2]);

  y = HAUTEUR_ECRAN - dec;
  h = 2 * dec;
  thickLineColor(surf, x, y + e, x, h, e, SDL_MapRGB(surf->format, 0, 0, 255));
}

void GestionEnv_supprimerPanier(){
	for(int i = 0; i < sizeof(panier); i++){
		cpShapeFree(panier[i]);
	}
}
