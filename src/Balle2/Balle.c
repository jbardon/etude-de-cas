/*
	Ce fichier n'est destiné à être compilé. Il présente seulement une autre façon    
    de gérer l'effacement et le dépassement de coordonnées d'une balle
*/

Balle* Balle_creer(SDL_Surface* surf, cpSpace* espace, int cx, int cy, int rayon, Uint32 couleur, char lettre){
			//...
			balle->precedent = NULL;
			//...
}

static void _Balle_maj_coordonees(Balle* balle){

	const cpVect pos = Balle_donneCoordonnees(balle);
	balle->cx = pos.x;
	balle->cy = HAUTEUR_ECRAN - pos.y;

	const int r = Balle_donneRayon(balle);

	//Vérifie abscisse
	if(pos.x < DEC + r)
		balle->cx = DEC + r;
	else if(pos.x > LARGUEUR_ECRAN - 2*DEC - r)
		balle->cx = LARGUEUR_ECRAN - 2*DEC - r;

	//Vérifie ordonnée
	if(HAUTEUR_ECRAN - pos.y < DEC + r)
		balle->cx = DEC + r;	
}


Uint32 donnePixel(SDL_Surface *s, int x, int y){
    Uint32 *pixels = (Uint32 *)s->pixels;
	return pixels[y * (s->pitch/sizeof(unsigned int)) + x];
}

void mettrePixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
    Uint32 *pixels = (Uint32 *)surface->pixels;
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
	//...	
	_Balle_maj_coordonees(balle);
	//...
}

