/**
 * @file Balle.c
 * @brief Gestion d'une balle d'un point de vue graphique et physique
 * @author Jérémy.B
 * @version 1.0
 * @date 10 novembre 2013
 *
 * Bibliothèque pour la gestion d'une balle avec la SDL et chipmunk
 *
 */

#include "Balle.h"

//-------------------------------------------------------------------------------------------------------------
//										Déclaration des fonctions locales
//-------------------------------------------------------------------------------------------------------------

static unsigned int Balle_donneRayon(Balle* balle);
static void _Balle_dessiner(Balle* balle, Uint32 couleur);
static void _Balle_dessiner_lettre(Balle* balle, Uint32 couleur);

//-------------------------------------------------------------------------------------------------------------
//										Initialisation et suppression
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn Balle* Balle_creer(SDL_Surface* surf, cpSpace* espace, cpVect centre, cpVect direction, 
 *						  int rayon, Uint32 couleur, char lettre)
 *
 * @brief Fonction de création d'une nouvelle balle
 *
 * @param surf Ecran principal de la fenêtre SDL
 * @param espace Espace physique (chipmunk) ou est la balle
 * @param centre Coordonnées du centre de la balle
 * @param direction Direction initiale vers laquelle la balle sera lancée
 * @param rayon Rayon de la balles
 * @param couleur Couleur de la balle sous la forme 0xRRGGBBAA
 * @param lettre Lettre liée à la balle (dessinée au centre)
 *
 * @return Nouvelle balle créée ou NULL si la création a échoué
 */
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
			cpFloat masse = rayon;
			cpFloat moment = cpMomentForCircle(masse, 0, rayon, cpvzero);

			// Créé une balle dont la position initiale est cx cy
			cpBody* corpsBalle = cpSpaceAddBody(espace, cpBodyNew(masse, moment));
			cpBodySetPos(corpsBalle, cpv(centre.x, HAUTEUR_ECRAN - centre.y));
			
			// Pour donner l'impression que les plus grandes balles tombent plus vite
			//cpBodySetVelLimit(corpsBalle, rayon * 15); 
			cpBodyApplyForce(corpsBalle, direction, cpvzero);

			// Zone de zoneCollision de la balle
			balle->zoneCollision = cpSpaceAddShape(espace, cpCircleShapeNew(corpsBalle, rayon, cpvzero));
			cpShapeSetFriction(balle->zoneCollision, FRICTION);
			cpShapeSetElasticity(balle->zoneCollision, REBOND);

			// Sauvegarde les caractéristiques de la balle
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

/**
 * @fn void Balle_supprimer(Balle* balle)
 * @brief Fonction de suppression d'une balle au niveau graphique et physique
 *
 * @param balle Balle à supprimer
 */
void Balle_supprimer(Balle* balle){

	// Supprime la balle graphiquement
	Balle_effacer(balle); 	
	SDL_FreeSurface(balle->canvas);
	SDL_Flip(balle->ecranJeu);

	cpBody* tmp = cpShapeGetBody(balle->zoneCollision);

	// Supprime la balle de l'espace
	cpSpaceRemoveShape(balle->espaceJeu, balle->zoneCollision);
	cpSpaceRemoveBody(balle->espaceJeu, tmp);

	// Libère les variables liées à la balle
	cpShapeFree(balle->zoneCollision);
	cpBodyFree(tmp);
	free(balle);
}

//-------------------------------------------------------------------------------------------------------------
//												Accesseurs 
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn unsigned int Balle_donneRayon(Balle* balle)
 * @brief Fonction qui donne le rayon d'une balle
 *
 * @param balle Balle dont on veut connaître le rayon
 *
 * @return rayon de la balle
 */
static unsigned int Balle_donneRayon(Balle* balle){
	return cpCircleShapeGetRadius(balle->zoneCollision);
}

/**
 * @fn float Balle_donneAngleDeg(Balle* balle)
 * @brief Accesseur sui renvoie l'angle de balle
 *
 * @param balle Balle dont on veut l'angle
 * @return Angle de la balle
 */
float Balle_donneAngleDeg(Balle* balle){

	float angleRad =  cpBodyGetAngle(cpShapeGetBody(balle->zoneCollision));
	float nbTours = angleRad / M_PI; // M_PI défini dans SDL_gfx_rotozoom.h

	// Angle en dégrés : partie décimale (enlève les tours complets) puis convertion en dégrés
	return (nbTours - (int)nbTours) * 360; 
}

/**
 * @fn cpVect Balle_donneCoordonnees(Balle* balle)
 * @brief Accesseur qui renvoie les coordonnées d'une balle
 *
 * @param balle Balle dont on veut les coordonnées
 * @return Coordonnées de la balle
 */
cpVect Balle_donneCoordonnees(Balle* balle){
	return cpBodyGetPos(cpShapeGetBody(balle->zoneCollision));
}

/**
 * @fn int Balle_estImmobile(Balle* balle)
 * @brief Fonction qui renvoie 0 si la balle est en mouvement, 1 sinon
 *
 * @param balle Balle dont on veut savoir si elle est immobile
 * @return 0 si la balle est en mouvement, 1 sinon
 */
int Balle_estImmobile(Balle* balle){
	cpVect vitesse = cpBodyGetVel(cpShapeGetBody(balle->zoneCollision));
	return (vitesse.y < 0.67 && vitesse.y > -0.67);
}

//-------------------------------------------------------------------------------------------------------------
//										Affichage et évolution de la balle
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn void Balle_afficher(Balle* balle)
 * @brief Fonction qui affiche une balle sur la fenêtre SDL donnée 
 * lors de la création de la balle
 *
 * @warning Pas de SDL_Flip pour éviter les problèmes lorsque plusieurs balles sont
 * déplacées dans une même fenêtre (clignotement)
 *
 * @param balle Balle que l'on veut afficher
 */
void Balle_afficher(Balle* balle){
	_Balle_dessiner(balle, balle->couleur);
}

/**
 * @fn void Balle_effacer(Balle* balle)
 * @brief Fonction qui efface une balle sur la fenêtre SDL donnée 
 * lors de la création de la balle
 *
 * @warning Pas de SDL_Flip pour éviter les problèmes lorsque plusieurs balles sont
 * déplacées dans une même fenêtre (clignotement)
 *
 * @param balle Balle que l'on veut effacer
 */
void Balle_effacer(Balle* balle){
	_Balle_dessiner(balle, 0xFFFFFFFF);
}

/**
 * @fn void _Balle_dessiner(Balle* balle, Uint32 couleur)
 * @brief Fonction qui dessine une balle sur la fenêtre SDL donnée 
 * lors de la création de la balle dans une couleur donnée
 *
 * @warning Pas de SDL_Flip pour éviter les problèmes lorsque plusieurs balles sont
 * déplacées dans une même fenêtre (clignotement)
 *
 * @param balle Balle que l'on veut dessiner
 * @param couleur Couleur de la balle
 */
static void _Balle_dessiner(Balle* balle, Uint32 couleur){

	// Informations sur la balle
	const unsigned int rayon = Balle_donneRayon(balle);
	SDL_Rect position = { balle->cx - rayon, balle->cy - rayon };

	//Dessine le cercle puis la lettre
	filledCircleColor(balle->canvas, rayon, rayon, rayon - 1, couleur);
	_Balle_dessiner_lettre(balle, couleur);

	// Affiche la balle
	SDL_BlitSurface(balle->canvas, NULL, balle->ecranJeu, &position);
}	

/**
 * @fn void _Balle_dessiner_lettre(Balle* balle, Uint32 couleur)
 * @brief Fonction qui dessine la lettre de la balle sur son centre
 *
 * @param balle Balle dont on veut dessiner la lettre
 * @param couleur Couleur de la balle
 */
static void _Balle_dessiner_lettre(Balle* balle, Uint32 couleur){

	// Chargement de la police et création de la lettre
	const unsigned int rayon = Balle_donneRayon(balle);
	const unsigned int taillePolice = floor((2 * rayon) * 0.65);
	SDL_Color couleurPolice;
	TTF_Font* police = TTF_OpenFont("arial.ttf", taillePolice);

	// Choix de la couleur
	if(couleur == 0xFFFFFFFF){
		couleurPolice.r = 255; 
		couleurPolice.g = 255; 
		couleurPolice.b = 255;
	}
	else
	{
		couleurPolice.r = 50;
		couleurPolice.g = 50;
		couleurPolice.b = 50;	
	}

	// Ecriture de la lettre
	char lettre [2] = { balle->lettre, 0 }; // Ajoute un zero terminal
	SDL_Surface* texte = TTF_RenderText_Solid(police, lettre, couleurPolice);

	// Effectue la rotation de la balle
	const float angle = Balle_donneAngleDeg(balle);	
	SDL_Surface* texteTourne = rotozoomSurface(texte, angle, 1, 0);
	SDL_FreeSurface(texte);

	// Calcul la position de la lettre en fonction de sa taille
	SDL_Rect position = {rayon - texteTourne->w/2, rayon - texteTourne->h/2};

	// Affichage du caractère
	SDL_BlitSurface(texteTourne, NULL, balle->canvas, &position);
	TTF_CloseFont(police);

	// Calcul la surface pour supprimer
	SDL_FreeSurface(texteTourne);
}

/**
 * @fn void Balle_deplacer(Balle* balle)
 * @brief Fonction qui efface la balle, met à jour les coordonnées
 *  de la balle avec chipmunk et affiche la balle
 *
 * @warning Pas de SDL_Flip pour éviter les problèmes lorsque plusieurs balles sont
 * déplacées dans une même fenêtre (clignotement)
 *
 * @param balle Balle que l'on veut deplacer
 */
void Balle_deplacer(Balle* balle){

	// Efface l'ancienne balle
	Balle_effacer(balle);

	if(!Balle_estImmobile(balle)){
		// Récupère les nouvelles coordonées de la balle
		const cpVect pos = Balle_donneCoordonnees(balle);
		balle->cx = pos.x;
		balle->cy = HAUTEUR_ECRAN - pos.y;
	}

	// Affiche la balle
	Balle_afficher(balle);
}

