#include "Balle.h"

//-------------------------------------------------------------------------------------------------------------
// Cr�� une nouvelle balle (en 2D) et l'affiche
//	Entr�e
//  	surf: �cran de jeu
// 		espace: monde chipmunk
// 		cx: abscisse du centre de la balle
// 		cy: ordonn�e du centre de la balle
// 		rayon: rayon de la balle
// 		couleur: couleur de la balle
// 		lettre: lettre de l'alphabet attach� � cette balle
//	Sortie
//		balle cr��e
//-------------------------------------------------------------------------------------------------------------
Balle* Balle_creer(SDL_Surface* surf, cpSpace* espace, int cx, int cy, int rayon, Uint32 couleur, char lettre){

	if(surf == NULL || espace == NULL){
		return NULL;
	}

	Balle* balle = malloc(sizeof(Balle));

	if(balle){

		//Cr�� la zone de dessin de la balle
		balle->canvas = SDL_CreateRGBSurface(/*SDL_HWSURFACE*/SDL_SRCALPHA, rayon*2, rayon*2, 32, 0, 0, 0, 0);

		if(balle->canvas){

			//Place la zone de dessin sur l'�cran de jeu
		    SDL_FillRect(balle->canvas, NULL, 0x0000FFFF);
			SDL_SetColorKey(balle->canvas, SDL_SRCCOLORKEY, 0x0000FFFF);

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
			Balle_afficher(balle);
		}
		else {
			free(balle);
		}
	}
	return balle;
}

static unsigned int Balle_donneRayon(Balle* balle){
	return cpCircleShapeGetRadius(balle->zoneCollision);
}

float Balle_donneAngleDeg(Balle* balle){

	float angleRad =  cpBodyGetAngle(cpShapeGetBody(balle->zoneCollision));
	float nbTours = angleRad / M_PI; //M_PI dans SDL_gfx_rotozoom.h

	//Angle en d�gr�s : partie d�cimale (enl�ve les tours complets) pui convertion en d�gr�s
	return (nbTours - (int)nbTours) * 360; 
}

void Balle_supprimer(Balle* balle){

	Balle_effacer(balle);
	SDL_FreeSurface(balle->canvas);
	SDL_Flip(balle->ecranJeu);

	free(balle->zoneCollision);
	free(balle);
}

cpVect Balle_donneCoordonnees(Balle* balle){
	return cpBodyGetPos(cpShapeGetBody(balle->zoneCollision));
}

static void _Balle_dessiner(Balle* balle, Uint32 couleur){
printf("couleur: %hx\n", couleur);
	//Informations sur la balle
	const unsigned int rayon = Balle_donneRayon(balle);
	SDL_Rect position = { balle->cx - rayon, balle->cy - rayon };

	//Effectue la rotation de la balle
	filledCircleColor(balle->canvas, rayon , rayon, rayon, couleur);
	SDL_Surface* balleTourne = Balle_rotation(balle);

	//Informations pour l'affichage
	const int delta = balleTourne->w - balle->canvas->w;
	SDL_Rect masque = { delta/2, delta/2, balle->canvas->h, balle->canvas->w };

	//Affiche la balle
	SDL_BlitSurface(balleTourne, &masque, balle->ecranJeu, &position);
	if(couleur != 0xFFFFFFFF){ //N'affiche pas apr�s effacement de la balle
		SDL_Flip(balle->ecranJeu);	
	}

	//Calcul la surface pour supprimer
	SDL_FreeSurface(balleTourne);
}	

void Balle_afficher(Balle* balle){
	_Balle_dessiner(balle, balle->couleur);
}

void Balle_effacer(Balle* balle){
	_Balle_dessiner(balle, 0xFFFFFFFF);
}

void Balle_deplacer(Balle* balle){

	//Efface l'ancienne balle
	Balle_effacer(balle);

	//R�cup�re les nouvelles coordon�es de la balle
	const cpVect pos = Balle_donneCoordonnees(balle);
	balle->cx = ((pos.x > LARGUEUR_ECRAN/2) ? floor(pos.x) : ceil(pos.x)); 
	balle->cy =  HAUTEUR_ECRAN  - pos.y;

	//Affiche la balle
	Balle_afficher(balle);

}

SDL_Surface* Balle_rotation(Balle* balle){
	
	const float angle = Balle_donneAngleDeg(balle);	
	return rotozoomSurface(balle->canvas, angle, 1, 0);

}
