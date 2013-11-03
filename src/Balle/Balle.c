#include "Balle.h"

//-------------------------------------------------------------------------------------------------------------
//										Déclaration des fonctions locales
//-------------------------------------------------------------------------------------------------------------
static unsigned int Balle_donneRayon(Balle* balle);
static void _Balle_dessiner(Balle* balle, Uint32 couleur);

//-------------------------------------------------------------------------------------------------------------
//										Initialisation et suppression
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// Créé une nouvelle balle (en 2D) et l'affiche
//	Entrée
//  	surf: écran de jeu
// 		espace: monde chipmunk
// 		centre: coordonées du centre de la balle
//		direction: direction initiale de la balle
// 		rayon: rayon de la balle
// 		couleur: couleur de la balle
// 		lettre: lettre de l'alphabet attaché à cette balle
//	Sortie
//		balle créée
//-------------------------------------------------------------------------------------------------------------
Balle* Balle_creer(SDL_Surface* surf, cpSpace* espace, cpVect centre, cpVect direction, 
					int rayon, Uint32 couleur, char lettre)
{

	if(surf == NULL || espace == NULL){
		return NULL;
	}

	Balle* balle = malloc(sizeof(Balle));

	if(balle){

		// Créé la zone de dessin de la balle
		balle->canvas = SDL_CreateRGBSurface(/*SDL_HWSURFACE*/SDL_SRCALPHA, rayon*2, rayon*2, 32, 0, 0, 0, 0);

		if(balle->canvas){

			// Place la zone de dessin sur l'écran de jeu
		    SDL_FillRect(balle->canvas, NULL, 0x0000FFFF);
			SDL_SetColorKey(balle->canvas, SDL_SRCCOLORKEY, 0x0000FFFF);

			// Caractéristiques physiques de la balle
			cpFloat masse = 1;
			cpFloat moment = cpMomentForCircle(masse, 0, rayon, cpvzero);

			// Créé une balle dont la position initiale est cx cy
			cpBody* corpsBalle = cpSpaceAddBody(espace, cpBodyNew(masse, moment));
			cpBodySetPos(corpsBalle, cpv(centre.x, HAUTEUR_ECRAN - centre.y));
			cpBodySetVelLimit(corpsBalle, 100);
			//cpBodyApplyImpulse(corpsBalle, direction, cpvzero);
			//cpBodySetForce(corpsBalle, direction);
			cpBodyApplyForce(corpsBalle, direction, cpvzero);

			// Zone de zoneCollision de la balle
			balle->zoneCollision = cpSpaceAddShape(espace, cpCircleShapeNew(corpsBalle, rayon, cpvzero));
			cpShapeSetFriction(balle->zoneCollision, FRICTION);
			cpShapeSetElasticity(balle->zoneCollision, REBOND);

			// Sauvegarde les caractéristiques de la balle
			balle->ecranJeu = surf;
			balle->cx = centre.x;
			balle->cy = centre.y;
			balle->couleur = couleur;
			balle->lettre = lettre;

			// Affiche la balle
			Balle_afficher(balle);
		}
		else {
			free(balle);
		}
	}
	return balle;
}

void Balle_supprimer(Balle* balle){

	Balle_effacer(balle);
	SDL_FreeSurface(balle->canvas);
	SDL_Flip(balle->ecranJeu);

	cpBody* tmp = cpShapeGetBody(balle->zoneCollision);
	cpShapeFree(balle->zoneCollision);
	cpBodyFree(tmp);
	free(balle);
}

//-------------------------------------------------------------------------------------------------------------
//												Accesseurs 
//-------------------------------------------------------------------------------------------------------------

static unsigned int Balle_donneRayon(Balle* balle){
	return cpCircleShapeGetRadius(balle->zoneCollision);
}

float Balle_donneAngleDeg(Balle* balle){

	float angleRad =  cpBodyGetAngle(cpShapeGetBody(balle->zoneCollision));
	float nbTours = angleRad / M_PI; // M_PI défini dans SDL_gfx_rotozoom.h

	// Angle en dégrés : partie décimale (enlève les tours complets) puis convertion en dégrés
	return (nbTours - (int)nbTours) * 360; 
}

cpVect Balle_donneCoordonnees(Balle* balle){
	return cpBodyGetPos(cpShapeGetBody(balle->zoneCollision));
}

//-------------------------------------------------------------------------------------------------------------
//											Affichage et évolution de la balle
//-------------------------------------------------------------------------------------------------------------

void Balle_afficher(Balle* balle){
	_Balle_dessiner(balle, balle->couleur);
}

void Balle_effacer(Balle* balle){
	_Balle_dessiner(balle, 0xFFFFFFFF);
}

static void _Balle_dessiner(Balle* balle, Uint32 couleur){

	// Informations sur la balle
	const unsigned int rayon = Balle_donneRayon(balle);
	SDL_Rect position = { balle->cx - rayon, balle->cy - rayon };

	// Effectue la rotation de la balle
	filledCircleColor(balle->canvas, rayon , rayon, rayon, couleur);
	SDL_Surface* balleTourne = Balle_rotation(balle);

	// Informations pour l'affichage
	const int delta = balleTourne->w - balle->canvas->w;
	SDL_Rect masque = { delta/2, delta/2, balle->canvas->h, balle->canvas->w };

	// Affiche la balle
	SDL_BlitSurface(balleTourne, &masque, balle->ecranJeu, &position);
	//!\\ Pas de SDL_Flip pour éviter les problèmes lorsque plusieurs balles sont
	//!\\ déplacées dans une même fenêtre

	// Calcul la surface pour supprimer
	SDL_FreeSurface(balleTourne);
}	

void Balle_deplacer(Balle* balle){

	// Efface l'ancienne balle
	Balle_effacer(balle);

	// Récupère les nouvelles coordonées de la balle
	const cpVect pos = Balle_donneCoordonnees(balle);
	balle->cx = pos.x;
	balle->cy = HAUTEUR_ECRAN - pos.y;

	// Affiche la balle
	Balle_afficher(balle);
}

SDL_Surface* Balle_rotation(Balle* balle){
	const float angle = Balle_donneAngleDeg(balle);	
	return rotozoomSurface(balle->canvas, angle, 1, 0);
}

