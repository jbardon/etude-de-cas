#include "Balle.h"

Balle* Balle_creer(cpSpace* espace, SDL_Surface* surf, int cx, int cy, int rayon, Uint32 couleur){

  if(surf == NULL || espace == NULL){
  	return NULL;
  }

  Balle* balle = malloc(sizeof(Balle));
  if(balle){
	  //Caractéristiques physiques de la balle
	  cpFloat masse = 1;
	  cpFloat moment = cpMomentForCircle(masse, 0, rayon, cpvzero);
	  
	  //Créé une balle dont la position initiale est cx cy
	  cpBody* corpsBalle = cpSpaceAddBody(espace, cpBodyNew(masse, moment));
	  cpBodySetPos(corpsBalle, cpv(cx, HAUTEUR_ECRAN - cy));
	  
	  //Zone de zoneCollision de la balle
	  balle->zoneCollision = cpSpaceAddShape(espace, cpCircleShapeNew(corpsBalle, rayon, cpvzero));
	  cpShapeSetFriction(balle->zoneCollision, 1);

	  //Associe la surface sur laquelle desinner le cercle
	  balle->ecran = surf;
      	  balle->cx = cx;
	  balle->cy = cy;
	  balle->couleur = couleur;

	  //Créé la balle graphique
	  filledCircleColor(surf, cx, cy, rayon, couleur);
  }
  return balle;
}

static int Balle_donneRayon(Balle* balle){
	return cpCircleShapeGetRadius(balle->zoneCollision);
}

void Balle_supprimer(Balle* balle){
	filledCircleColor(balle->ecran, balle->cx, balle->cy, Balle_donneRayon(balle), 0xFFFFFFFF);
	free(balle->zoneCollision);
	free(balle);
}

cpVect Balle_donneCoordonnees(Balle* balle){
	return cpBodyGetPos(cpShapeGetBody(balle->zoneCollision));
}

void Balle_deplacer(Balle* balle){

	//Nouvelles coordonées de la balle
	cpVect pos = Balle_donneCoordonnees(balle);
	int r = Balle_donneRayon(balle);

	//Efface le cercle à son ancienne position
	filledCircleColor(balle->ecran, balle->cx, balle->cy, r, 0xFFFFFFFF);

	//Met à jour les coordonées du cercle et l'affiche
	balle->cx = pos.x; balle->cy =  HAUTEUR_ECRAN - pos.y;
	filledCircleColor(balle->ecran, balle->cx, balle->cy, r, balle->couleur);

	SDL_Flip(balle->ecran);
}

