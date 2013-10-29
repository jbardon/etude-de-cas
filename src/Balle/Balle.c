#include "Balle.h"

Balle* Balle_creer(SDL_Surface* surf, cpSpace* espace, int cx, int cy, int rayon, Uint32 couleur, char lettre){

	if(surf == NULL || espace == NULL){
		return NULL;
	}

	Balle* balle = malloc(sizeof(Balle));

	if(balle){

		//Cr�� la zone de dessin de la balle
		balle->canvas = SDL_CreateRGBSurface(SDL_HWSURFACE, rayon*2, rayon*2, 32, 0, 0, 0, 0);

		if(balle->canvas){

			//Place la zone de dessin sur l'�cran de jeu
		    SDL_FillRect(balle->canvas, NULL, /*0x0000FFFF*/0xFFFFFFFF);

			//Caract�ristiques physiques de la balle
			cpFloat masse = 1;
			cpFloat moment = cpMomentForCircle(masse, 0, rayon, cpvzero);

			//Cr�� une balle dont la position initiale est cx cy
			cpBody* corpsBalle = cpSpaceAddBody(espace, cpBodyNew(masse, moment));
			cpBodySetPos(corpsBalle, cpv(cx, HAUTEUR_ECRAN - cy));

			//Zone de zoneCollision de la balle
			balle->zoneCollision = cpSpaceAddShape(espace, cpCircleShapeNew(corpsBalle, rayon, cpvzero));
			cpShapeSetFriction(balle->zoneCollision, 1);

			//Associe la surface de jeu
			balle->ecranJeu = surf;
			balle->cx = cx;
			balle->cy = cy;
			balle->couleur = couleur;
			balle->lettre = lettre;

			//Cr�� la balle graphique
			Balle_dessiner(balle);
		}
		else {
			free(balle);
		}
	}
	return balle;
}

static int Balle_donneRayon(Balle* balle){
	return cpCircleShapeGetRadius(balle->zoneCollision);
}

float Balle_donneAngleDeg(Balle* balle){

	float angleRad =  cpBodyGetAngle(cpShapeGetBody(balle->zoneCollision));
	float nbTours = angleRad / M_PI; //M_PI dans SDL_gfx_rotozoom.h

	//Angle en d�gr�s : partie d�cimale (enl�ve les tours complets) pui convertion en d�gr�s
	return (nbTours - (int)nbTours) * 360; 
}

void Balle_supprimer(Balle* balle){
	SDL_FreeSurface(balle->canvas);
	SDL_Flip(balle->ecranJeu);

	free(balle->zoneCollision);
	free(balle);
}

cpVect Balle_donneCoordonnees(Balle* balle){
	return cpBodyGetPos(cpShapeGetBody(balle->zoneCollision));
}

void Balle_dessiner(Balle* balle){

	int rayon = Balle_donneRayon(balle);

	//Dessin du cercle
	filledCircleColor(balle->canvas, rayon , rayon, rayon, balle->couleur);
	lineColor(balle->canvas, 0, 20, rayon*2, 20, 0x0000FFFF);

	//Dessin du caract�re
	//....

	//Met � jour l'affichage
	SDL_Rect position = { balle->cx - rayon, balle->cy - rayon };
	SDL_Surface* balleTourne = Balle_rotation(balle);
	SDL_BlitSurface(balleTourne, NULL, balle->ecranJeu, &position);
	SDL_Flip(balle->ecranJeu);
}

void Balle_effacer(Balle* balle){

	int rayon = Balle_donneRayon(balle);
	SDL_Rect position = { balle->cx - rayon, balle->cy - rayon };

	SDL_FillRect(balle->canvas, NULL, 0xFFFFFFFF);	
	SDL_BlitSurface(balle->canvas, NULL, balle->ecranJeu, &position);
//	SDL_Flip(balle->ecranJeu);
}

void Balle_deplacer(Balle* balle){

	Balle_effacer(balle);

	//Nouvelles coordon�es de la balle
	cpVect pos = Balle_donneCoordonnees(balle);
	balle->cx = ((pos.x > LARGUEUR_ECRAN/2) ? floor(pos.x) : ceil(pos.x)); 
	balle->cy =  HAUTEUR_ECRAN  - pos.y;

	Balle_dessiner(balle);
}

SDL_Surface* Balle_rotation(Balle* balle){
	
	float angle = Balle_donneAngleDeg(balle);	
	return rotozoomSurface(balle->canvas, angle, 1, 0);
}
