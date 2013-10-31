#include "Balle.h"

//-------------------------------------------------------------------------------------------------------------
// Créé une nouvelle balle (en 2D) et l'affiche
//	Entrée
//  	surf: écran de jeu
// 		espace: monde chipmunk
// 		cx: abscisse du centre de la balle
// 		cy: ordonnée du centre de la balle
// 		rayon: rayon de la balle
// 		couleur: couleur de la balle
// 		lettre: lettre de l'alphabet attaché à cette balle
//	Sortie
//		balle créée
//-------------------------------------------------------------------------------------------------------------
Balle* Balle_creer(SDL_Surface* surf, cpSpace* espace, int cx, int cy, int rayon, Uint32 couleur, char lettre){

	if(surf == NULL || espace == NULL){
		return NULL;
	}

	Balle* balle = malloc(sizeof(Balle));

	if(balle){

		//Créé la zone de dessin de la balle
		balle->canvas = SDL_CreateRGBSurface(/*SDL_HWSURFACE*/SDL_SRCALPHA, rayon*2, rayon*2, 32, 0, 0, 0, 0);

		if(balle->canvas){

			//Place la zone de dessin sur l'écran de jeu
		    SDL_FillRect(balle->canvas, NULL, 0x0000FFFF);
			SDL_SetColorKey(balle->canvas, SDL_SRCCOLORKEY, 0x0000FFFF);
			filledCircleColor(balle->canvas, rayon , rayon, rayon, balle->couleur);

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
			balle->precedent = NULL;
			balle->cx = cx;
			balle->cy = cy;
			balle->couleur = couleur;
			balle->lettre = lettre;

			//Créé la balle graphique
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

	//Angle en dégrés : partie décimale (enlève les tours complets) pui convertion en dégrés
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

Uint32 donnePixel(SDL_Surface *s, int x, int y){
    
	Uint32 *pixels = (Uint32 *)s->pixels;
	return pixels[y * (s->pitch/sizeof(unsigned int)) + x];
}

void mettrePixel(SDL_Surface *surface, int x, int y, Uint32 pixel){

    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    
    //Set the pixel
    pixels[ (y * surface->w) + x ] = pixel;

}

void calculSurfaceSupprimer(Balle* balle){

	//Calcul la surface à supprimer pour le prochain déplacement
	// -> remplace les pixels de la couleur du cercle par la couleur du fond
	const unsigned int tailleSurface = balle->precedent->w * balle->precedent->h;	
	unsigned int x, y;
	for(unsigned int i = 0; i < tailleSurface; i++){
		x = i / balle->precedent->w;
		y = i % balle->precedent->w;	

		if(donnePixel(balle->precedent, x, y) == 0x00FF00FF){
			mettrePixel(balle->precedent, x, y, 0xFFFFFFFF);
		}
	}
}
	
void Balle_afficher(Balle* balle){

	//Informations sur la balle
	const unsigned int rayon = Balle_donneRayon(balle);
	SDL_Rect position = { balle->cx - rayon, balle->cy - rayon };

	//Effectue la rotation de la balle
	SDL_Surface* balleTourne = Balle_rotation(balle);
	balle->precedent = balleTourne;

	//Informations pour l'affichage
	const int delta = balle->precedent->w - balle->canvas->w;
	SDL_Rect masque = { delta/2, delta/2, balle->canvas->h, balle->canvas->w };

	//Affiche la balle
	SDL_BlitSurface(balleTourne, &masque, balle->ecranJeu, &position);
	SDL_Flip(balle->ecranJeu);	

	//Calcul la surface pour supprimer
	calculSurfaceSupprimer(balle);
}

void Balle_effacer(Balle* balle){

	//Calcul de la surface à supprimer
	const unsigned int rayon = Balle_donneRayon(balle);
	SDL_Rect position = { balle->cx - rayon, balle->cy - rayon };
	const int delta = balle->precedent->w - balle->canvas->w;
	SDL_Rect masque = { delta/2, delta/2, balle->canvas->h, balle->canvas->w };

	//Efface la surface
	SDL_BlitSurface(balle->precedent, &masque, balle->ecranJeu, &position);
	SDL_FreeSurface(balle->precedent);

}

void Balle_deplacer(Balle* balle){

	//Efface l'ancienne balle
	Balle_effacer(balle);

	//Récupère les nouvelles coordonées de la balle
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
