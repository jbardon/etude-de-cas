#include "Balle.h"

//-------------------------------------------------------------------------------------------------------------
//										D�claration des fonctions locales
//-------------------------------------------------------------------------------------------------------------
static unsigned int Balle_donneRayon(Balle* balle);
static void _Balle_dessiner(Balle* balle, Uint32 couleur);
static void _Balle_dessiner_lettre(Balle* balle);
//-------------------------------------------------------------------------------------------------------------
//										Initialisation et suppression
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// Cr�� une nouvelle balle (en 2D) et l'affiche
//	Entr�e
//  	surf: �cran de jeu
// 		espace: monde chipmunk
// 		centre: coordon�es du centre de la balle
//		direction: direction initiale de la balle
// 		rayon: rayon de la balle
// 		couleur: couleur de la balle
// 		lettre: lettre de l'alphabet attach� � cette balle
//	Sortie
//		balle cr��e
//-------------------------------------------------------------------------------------------------------------
Balle* Balle_creer(SDL_Surface* surf, cpSpace* espace, cpVect centre, cpVect direction, 
					int rayon, Uint32 couleur, char lettre)
{

	if(surf == NULL || espace == NULL){
		return NULL;
	}

	Balle* balle = malloc(sizeof(Balle));

	if(balle){

		// Cr�� la zone de dessin de la balle
		balle->canvas = SDL_CreateRGBSurface(/*SDL_HWSURFACE*/SDL_SRCALPHA, rayon*2, rayon*2, 32, 0, 0, 0, 0);

		if(balle->canvas){

			// Place la zone de dessin sur l'�cran de jeu
		    SDL_FillRect(balle->canvas, NULL, 0x0000FFFF);
			SDL_SetColorKey(balle->canvas, SDL_SRCCOLORKEY, 0x0000FFFF);

			// Caract�ristiques physiques de la balle
			cpFloat masse = rayon;
			cpFloat moment = cpMomentForCircle(masse, 0, rayon, cpvzero);

			// Cr�� une balle dont la position initiale est cx cy
			cpBody* corpsBalle = cpSpaceAddBody(espace, cpBodyNew(masse, moment));
			cpBodySetPos(corpsBalle, cpv(centre.x, HAUTEUR_ECRAN - centre.y));
			
			// Pour donner l'impression que les plus grandes balles tombent plus vite
			cpBodySetVelLimit(corpsBalle, rayon * 10); 

			cpBodyApplyImpulse(corpsBalle, direction, cpvzero);

			// Zone de zoneCollision de la balle
			balle->zoneCollision = cpSpaceAddShape(espace, cpCircleShapeNew(corpsBalle, rayon, cpvzero));
			cpShapeSetFriction(balle->zoneCollision, FRICTION);
			cpShapeSetElasticity(balle->zoneCollision, REBOND);

			// Sauvegarde les caract�ristiques de la balle
			balle->ecranJeu = surf;
			balle->espaceJeu = espace;
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

	// Supprime la balle graphiquement
	Balle_effacer(balle);
	SDL_FreeSurface(balle->canvas);
	SDL_Flip(balle->ecranJeu);

	cpBody* tmp = cpShapeGetBody(balle->zoneCollision);

	// Supprime la balle de l'espace
	cpSpaceRemoveShape(balle->espaceJeu, balle->zoneCollision);
	cpSpaceRemoveBody(balle->espaceJeu, tmp);

	// Lib�re les variables li�es � la balle
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
	float nbTours = angleRad / M_PI; // M_PI d�fini dans SDL_gfx_rotozoom.h

	// Angle en d�gr�s : partie d�cimale (enl�ve les tours complets) puis convertion en d�gr�s
	return (nbTours - (int)nbTours) * 360; 
}

cpVect Balle_donneCoordonnees(Balle* balle){
	return cpBodyGetPos(cpShapeGetBody(balle->zoneCollision));
}

int Balle_estImmobile(Balle* balle){
	cpVect vitesse = cpBodyGetVel(cpShapeGetBody(balle->zoneCollision));
	printf("%f %d\n", vitesse.y, (vitesse.y == 0.0));
	return (vitesse.y == 0.0);
}

//-------------------------------------------------------------------------------------------------------------
//										Affichage et �volution de la balle
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

	//Dessine le cercle puis la lettre
	filledCircleColor(balle->canvas, rayon , rayon, rayon, couleur);
	_Balle_dessiner_lettre(balle);

	// Effectue la rotation de la balle
	SDL_Surface* balleTourne = Balle_rotation(balle);

	// Informations pour l'affichage
	const int delta = balleTourne->w - balle->canvas->w;
	SDL_Rect masque = { delta/2, delta/2, balle->canvas->h, balle->canvas->w };

	// Affiche la balle
	SDL_BlitSurface(balleTourne, &masque, balle->ecranJeu, &position);
	//!\\ Pas de SDL_Flip pour �viter les probl�mes lorsque plusieurs balles sont
	//!\\ d�plac�es dans une m�me fen�tre

	// Calcul la surface pour supprimer
	SDL_FreeSurface(balleTourne);
}	

static void _Balle_dessiner_lettre(Balle* balle){

	// Chargement de la police et cr�ation de la lettre
	const unsigned int rayon = Balle_donneRayon(balle);
	const unsigned int taillePolice = floor((2 * rayon) * 0.65);
	SDL_Color couleurNoire = {50,50,50};
	TTF_Font* police = TTF_OpenFont("/usr/share/fonts/truetype/msttcorefonts/arial.ttf", taillePolice);

	// Ecriture de la lettre
	char lettre [2] = { balle->lettre, 0 }; // Ajoute un zero terminal
	SDL_Surface* texte = TTF_RenderText_Solid(police, lettre, couleurNoire);

	// Calcul la position de la lettre en fonction de sa taille
	int longueurPolice = 0, hauteurPolice = 0;
	TTF_SizeText(police, lettre, &longueurPolice, &hauteurPolice);
	SDL_Rect position = {rayon - longueurPolice/2, rayon - hauteurPolice/2};

	// Affichage du caract�re
	SDL_BlitSurface(texte, NULL, balle->canvas, &position);
	TTF_CloseFont(police);
}

void Balle_deplacer(Balle* balle){

	// Efface l'ancienne balle
	Balle_effacer(balle);

	// R�cup�re les nouvelles coordon�es de la balle
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

