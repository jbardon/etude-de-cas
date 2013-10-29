#include "Balle.h"

Balle* Balle_creer(SDL_Surface* surf, cpSpace* espace, int cx, int cy, int rayon, Uint32 couleur, char lettre){

	if(surf == NULL || espace == NULL){
		return NULL;
	}

	Balle* balle = malloc(sizeof(Balle));

	if(balle){

		//Créé la zone de dessin de la balle
		balle->canvas = SDL_CreateRGBSurface(SDL_HWSURFACE, rayon*2, rayon*2, 32, 0, 0, 0, 0);

		if(balle->canvas){

			//Place la zone de dessin sur l'écran de jeu
		    SDL_FillRect(balle->canvas, NULL, 0x0000FFFF);

			//Caractéristiques physiques de la balle
			cpFloat masse = 1;
			cpFloat moment = cpMomentForCircle(masse, 0, rayon, cpvzero);

			//Créé une balle dont la position initiale est cx cy
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
			balle->angle = 0;
			balle->lettre = lettre;

			//Créé la balle graphique
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

float Balle_donneAngle(Balle* balle){
	return cpBodyGetAngle(cpShapeGetBody(balle->zoneCollision));
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

	//Dessin du caractère
	//....

	//Met à jour l'affichage
	SDL_Rect position = { balle->cx - rayon, balle->cy - rayon };

	SDL_BlitSurface(balle->canvas, NULL, balle->ecranJeu, &position);
	SDL_Flip(balle->ecranJeu);
}

void Balle_effacer(Balle* balle){

	int rayon = Balle_donneRayon(balle);
	SDL_Rect position = { balle->cx - rayon, balle->cy - rayon };

	SDL_FillRect(balle->canvas, NULL, 0x0000FFFF);	
	SDL_BlitSurface(balle->canvas, NULL, balle->ecranJeu, &position);
//	SDL_Flip(balle->ecranJeu);
}

void Balle_deplacer(Balle* balle){

	Balle_effacer(balle);

	//Nouvelles coordonées de la balle
	cpVect pos = Balle_donneCoordonnees(balle);
	balle->cx = ((pos.x > LARGUEUR_ECRAN/2) ? floor(pos.x) : ceil(pos.x)); 
	balle->cy =  HAUTEUR_ECRAN  - pos.y;

//	Balle_rotation(balle);
	Balle_dessiner(balle);
}

void Balle_rotation(Balle* balle){
	
	//float angle = Balle_donneAngle(balle);
	//float rotation = /*angle - balle->angle*/0.1;
	
	balle->canvas = rotozoomSurface(balle->canvas, 5, 1, 0);
}
