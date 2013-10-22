#include "CreationEnv.h"

static cpShape* panier[3];

void CreationEnv_creerPanier(cpSpace* espace, SDL_Surface* surf, int dec){
  
  int x = dec;
  int y = dec;
  int l = LARGUEUR_ECRAN - 2*dec;
  int h = 0;
  int e = 4; //epaisseur du trait (SDL)
  
  //Création du sol qui est un élément statique---------------------------------
  panier[0] = cpSegmentShapeNew(espace->staticBody, cpv(x,y), 
  													cpv(l,y), 0);
  cpShapeSetFriction(panier[0], 1);
  cpSpaceAddShape(espace, panier[0]);
  
  y = HAUTEUR_ECRAN - y;
  SDL_Ex_DessinerLigneHoriz(surf, x, y, l - dec, e, SDL_Ex_Couleur(0, 0, 0));
  
  //Création du mur gauche------------------------------------------------------
  x = dec;
  y = dec;
  h = HAUTEUR_ECRAN - 2*dec;
  panier[1] = cpSegmentShapeNew(espace->staticBody, cpv(x,y), 
  													cpv(x,h), 0);
  cpShapeSetFriction(panier[1], 1);
  cpSpaceAddShape(espace, panier[1]);
  
  y = HAUTEUR_ECRAN - y;
  SDL_Ex_DessinerLigneVert(surf, x - e/2, y, h - dec, e, SDL_Ex_Couleur(0, 0, 0));

  //Création du mur droit-------------------------------------------------------
  x = LARGUEUR_ECRAN - dec;
  y = dec;
  h = HAUTEUR_ECRAN - 2*dec;
  panier[2] = cpSegmentShapeNew(espace->staticBody, cpv(x,y), 
  													cpv(x,h), 0);
  cpShapeSetFriction(panier[2], 1);
  cpSpaceAddShape(espace, panier[2]);

  y = HAUTEUR_ECRAN - y;
  SDL_Ex_DessinerLigneVert(surf, x + e/2, y, h - dec, e, SDL_Ex_Couleur(0, 0, 0));

}

void CreationEnv_supprimerPanier(){
	for(int i = 0; i < sizeof(panier); i++){
		cpShapeFree(panier[i]);
	}
}
