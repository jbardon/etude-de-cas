/**
 * @file GestionEnv.c
 * @brief Gestion de l'environnement graphique et physique du jeu
 * @author J�r�my.B
 * @version 1.0
 * @date 10 novembre 2013
 *
 * Biblioth�que pour la gestion de la SDL, chipmunk, des balles et
 * du trac� des lignes pour s�lectionner des balles
 *
 */

#include "GestionEnv.h"

//-------------------------------------------------------------------------------------------------------------
//										D�claration des fonctions locales
//-------------------------------------------------------------------------------------------------------------
static cpSpace* _initChipmunk();
static SDL_Surface* _initSDL();

static void _quitChipmunk(cpSpace* espace);
static void _quitSDL(SDL_Surface* ecran);

static void _creerPanier(Environnement* envJeu);
static void _supprimerPanier(cpShape** panier);

static void _afficherProgression(Environnement* envJeu);
static void _dessinerPanier();

static void _Balle_foreach(GPtrArray* balles, Balle_Fonction fonction);
static void _creerBalles(Environnement* envJeu, int nbBalles);
static void _creerUneBalle(Environnement* envJeu);
static void _supprimerBalles(GPtrArray* balles);

//-------------------------------------------------------------------------------------------------------------
//										Variables li�es � l'environnement
//-------------------------------------------------------------------------------------------------------------

/**
 * @var cpSpace* espace
 * @brief Espace physique au sens de chipmunk
 *
 * Initialisation: @see GestionEnv_initChipmunk
 * Destruction: @see GestionEnv_quitChipmunk
 *
 */ 
//static cpSpace* espace = NULL;

/**
 * @var cpShape* panier[3]
 * @brief Limitations physique du saladier ou tombent
 * les balles
 *
 * @see GestionEnv_creerPanier
 * @see GestionEnv_supprimerPanier
 *
 * Ce tableau contient dans l'ordre le sol et les 2 murs (gauche, droite) 
 *
 */ 
//static cpShape* panier[3]; 

/**
 * @var double temps
 * @brief Temps dans le monde chipmunk
 *
 * Temps qui s'�coule dans le monde chipmunk � 
 * chaque appel de la fonction GestionEnv_evoluer
 *
 * @see GestionEnv_evoluer
 *
 */ 
//static double temps = 0;

/**
 * @var SDL_Surface* ecran
 * @brief Ecran principal de fen�tre SDL
 *
 * @see GestionEnv_initSDL
 * @see GestionEnv_quitSDL
 *
 */ 
//static SDL_Surface* ecran = NULL;

/**
 * @var GSList balles
 * @brief Tableau des balles pr�sentent dans l'environnement
 * Balles cr��es avec la fonction _creerUneBalle
 *
 * @see GestionEnv_creerBalles
 * @see _creerUneBalle
 *
 *
 */ 
//static GPtrArray* balles = NULL;

/**
 * @var int nbBallesTotal
 * @brief Taille du tableau de balles
 *
 * @see balles
 */ 
//static int nbBallesTotal = 0;  

/**
 * @var int nbBallesCrees
 * @brief Nombre de balles r��lement cr��es dans l'environnement
 *
 * @see GestionEnv_creerBalles
 */ 
//static int nbBallesCrees = 0; 

/**
 * @var int timerLancement
 * @brief Compteur de temps pour lancer les balles
 *
 * Le temps �volue avec GestionEnv_evoluer � travers
 * la variable temps. Une fois le compteur � son maximum (DELAI_APPARITION)
 * une balle est cr��e et le timer est remis � z�ro
 *
 * @see temps
 * @see GestionEnv_evoluer
 * @see DELAI_APPARITION (config.h)
 */ 
//static int timerLancement = 0;

/**
 * @var int ajouterBalles
 * @brief Sp�cifie si l'on peut ajouter des balles
 * Important dans le cas ou l'on laisse �voluer le syst�me
 * Une fois les lettres s�lectionn�es et supprim�es
 *
 * @see GestionEnv_creerBalles
 */ 
//static int ajouterBalles = 0;

//-------------------------------------------------------------------------------------------------------------
//										Initialisation de l'environnement
//-------------------------------------------------------------------------------------------------------------

Environnement* GestionEnv_creerEnvironnement(){
	
	Environnement* envJeu = malloc(sizeof(Environnement));

	if(envJeu){

		Aleatoire_init();

		envJeu->espace = _initChipmunk();
		envJeu->ecran = _initSDL();
		envJeu->balles = NULL;
		envJeu->nbBallesTotal = 0;
		envJeu->nbBallesCrees = 0;
		envJeu->ajouterBalles = 0;

		_creerPanier(envJeu);
		_creerBalles(envJeu, NB_BALLES);
	}

	return envJeu;
}

void GestionEnv_rejouer(Environnement* envJeu){
	_supprimerBalles(envJeu->balles);
	envJeu->nbBallesTotal = 0;
	envJeu->nbBallesCrees = 0;
	_creerBalles(envJeu, NB_BALLES);
}

/**
 * @fn cpSpace* _initChipmunk()
 * @brief Fonction qui initialise le monde chipmunk
 * 
 * @see espace
 *
 * @return L'espace physique cr��
 */
static cpSpace* _initChipmunk(){

	cpSpace* espace = cpSpaceNew();

	if(espace){
		cpVect gravite = cpv(0, -100);  
		cpSpaceSetGravity(espace, gravite);
	}

	return espace;
}

/**
 * @fn SDL_Surface* _initSDL()
 * @brief Fonction qui initialise la biblioth�que SDL 
 * et la fen�tre de jeu SDL
 * 
 * @see ecran
 *
 * @return La fen�tre SDL
 */
static SDL_Surface* _initSDL(){

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	SDL_Surface* ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);

	if(ecran){
		SDL_FillRect(ecran, NULL, COULEUR_FOND);
		SDL_WM_SetCaption("Petanque 2000", NULL);
	}

	return ecran;	
}

//-------------------------------------------------------------------------------------------------------------
//										Suppression de l'environnement
//-------------------------------------------------------------------------------------------------------------

void GestionEnv_supprimerEnvironnement(Environnement* envJeu){
	_supprimerBalles(envJeu->balles);
	_supprimerPanier(envJeu->panier);

	_quitChipmunk(envJeu->espace);
	_quitSDL(envJeu->ecran);

	free(envJeu);
}

/**
 * @fn void _quitChipmunk(cpSpace* espace)
 * @brief Fonction qui d�truit le monde chipmunk
 * 
 * @see _initChipmunk
 * @see espace
 */
static void _quitChipmunk(cpSpace* espace){
	cpSpaceFree(espace);
}

/**
 * @fn Svoid _quitSDL(SDL_Surface* ecran)
 * @brief Fonction qui d�truit la fen�tre de jeu SDL
 * et quitte la biblioth�que SDL 
 * 
 * @see _initSDL
 * @see ecran
 */
static void _quitSDL(SDL_Surface* ecran){
	SDL_FreeSurface(ecran);
	TTF_Quit();
	SDL_Quit();
}

//-------------------------------------------------------------------------------------------------------------
//									Evolution de l'environnement (dans le temps)
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn void GestionEnv_evoluer()
 * @brief Fonction qui fait evoluer l'environnement
 * physique et graphique du jeu avec notamment les balles 
 * qui se d�placent
 */
void GestionEnv_evoluer(Environnement* envJeu){
	
	static double temps = 0;
	static int timerLancement = 0;

	// Cr�� une balle � chaque intervalle de temps, max = nombre de balles donn�e dans GestionEnv_creerBalles
	if(envJeu->ajouterBalles && timerLancement >= DELAI_APPARITION){
		_creerUneBalle(envJeu);
		timerLancement = 0;	
	}
/*
	// Met � jour l'affichage du panneau de la fen�tre
	char* message = calloc(strlen("Projet etude de cas - Balles 100") + 1, sizeof(char));
	sprintf(message, "Projet etude de cas - Balles %d", nbBallesCrees);
	SDL_WM_SetCaption(message, NULL);
	free(message);
*/
	// Fait �voluer les balles, applique le d�placement et maj de l'affichage
	cpSpaceStep(envJeu->espace, uniteTemps);

	_Balle_foreach(envJeu->balles, Balle_deplacer); 
	_dessinerPanier(envJeu->ecran); 
	_afficherProgression(envJeu);

	SDL_Flip(envJeu->ecran);

	// Gestion du temps
	temps += uniteTemps;
	timerLancement += 1;
}

static void _afficherProgression(Environnement* envJeu){	

	boxColor(envJeu->ecran, 0, HAUTEUR_ECRAN - 5, LARGUEUR_ECRAN, HAUTEUR_ECRAN, COULEUR_FOND);

	if(envJeu->ajouterBalles){
		float longueur = ((float)envJeu->nbBallesCrees/(float)envJeu->nbBallesTotal) * LARGUEUR_ECRAN;
		boxColor(envJeu->ecran, 0, HAUTEUR_ECRAN - 5, ceil(longueur), HAUTEUR_ECRAN, 0xFF4040DD);
	}
}

//-------------------------------------------------------------------------------------------------------------
//										Gestion du panier
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn void _creerPanier(cpSpace* espace, SDL_Surface* surf)
 * @brief Fonction qui cr�� le panier au sens physique (chipmunk)
 * et affiche ces contours sur la fen�tre SDL
 *
 * @param espace Espace physique chipmunk @see espace
 * @param surf Ecran principal de la fen�tre SDL @see ecran
 */
static void _creerPanier(Environnement* envJeu){
 
	int x = OFFSET + 2;
	int y = OFFSET;

	// Cr�ation du sol qui est un �l�ment statique
	envJeu->panier[0] = cpSegmentShapeNew(envJeu->espace->staticBody, cpv(0,y), cpv(LARGUEUR_ECRAN,y), 0);
	cpShapeSetFriction(envJeu->panier[0], FRICTION);
	cpShapeSetElasticity(envJeu->panier[0], REBOND);
	cpSpaceAddShape(envJeu->espace, envJeu->panier[0]);

	// Cr�ation du mur gauche
	envJeu->panier[1] = cpSegmentShapeNew(envJeu->espace->staticBody, cpv(x,0), cpv(x, HAUTEUR_ECRAN + BALLE_RAYON_MAX * 2), 0);
	cpShapeSetFriction(envJeu->panier[1], FRICTION);
	cpShapeSetElasticity(envJeu->panier[1], REBOND);
	cpSpaceAddShape(envJeu->espace, envJeu->panier[1]);

	// Cr�ation du mur droit
	x = LARGUEUR_ECRAN - OFFSET - 2;
	envJeu->panier[2] = cpSegmentShapeNew(envJeu->espace->staticBody, cpv(x,0), cpv(x, HAUTEUR_ECRAN + BALLE_RAYON_MAX * 2), 0);
	cpShapeSetFriction(envJeu->panier[2], FRICTION);
	cpShapeSetElasticity(envJeu->panier[2], REBOND);
	cpSpaceAddShape(envJeu->espace, envJeu->panier[2]);

	// Dessine le panier sur l'�cran
	_dessinerPanier(envJeu->ecran);
}

static void _dessinerPanier(SDL_Surface* ecran){

	static const unsigned int e = 10; // Epaisseur du trait (SDL)
	static unsigned int x;
	static unsigned int y;
	static unsigned int l;
	static unsigned int h;

	// Initialisations
	x = OFFSET;
	l = LARGUEUR_ECRAN - 2*OFFSET;

	// Affichage du sol
	y = HAUTEUR_ECRAN - OFFSET + e/2;
	thickLineColor(ecran, x - e, y, x + l + e, y, e, 0x000000FF);

	// Affichage du mur gauche
	x = OFFSET - e/2;
	y = HAUTEUR_ECRAN - OFFSET;
	h = 3 * OFFSET;
	thickLineColor(ecran, x, y + e, x, h, e, 0x00000FF);

	// Affichage du mur droit
	x = LARGUEUR_ECRAN - OFFSET + e/2;
	thickLineColor(ecran, x, y + e, x, h, e, 0x000000FF);
}

/**
 * @fn void _supprimerPanier(cpShape** panier)
 * @brief Fonction qui supprime le panier au sens physique (chipmunk)
 */
static void _supprimerPanier(cpShape** panier){
	for(int i = 0; i < 3; i++){
		cpShapeFree(panier[i]);
	}
}

void GestionEnv_effacerPanier(SDL_Surface* ecran){
	boxColor(ecran, 0, 2*OFFSET, LARGUEUR_ECRAN, HAUTEUR_ECRAN, COULEUR_FOND);
}

//-------------------------------------------------------------------------------------------------------------
//										Gestion des balles
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn static void _Balle_foreach(Balle_Fonction fonction)
 * @brief Fonction qui applique une fonction sur toutes
 * les balles pr�sentes dans l'environnement
 *
 * @param fonction Fonction provenant de la biblioth�que Balle
 * ayant aucun retour et pour seul param�tre une balle
 */
static void _Balle_foreach(GPtrArray* balles, Balle_Fonction fonction){
	g_ptr_array_foreach(balles, (GFunc)fonction, NULL);
}

/**
 * @fn static void _creerUneBalle()
 * @brief Fonction qui cr�� une balle dans l'environnement 
 * avec des param�tres al�atoires :
 *
 * @see Balle_creer
 * rayon: 28 -> 36
 * centre: x (dans les panier), y (juste au dessus de la fen�tre)
 * direction: (0,-80) -> (0,80)
 * couleur: @see couleurs
 * lettre: A -> Z
 *
 * @param fonction Fonction provenant de la biblioth�que Balle
 * ayant aucun retour et pour seul param�tre une balle
 */
static void _creerUneBalle(Environnement* envJeu){

	if(envJeu->nbBallesCrees < envJeu->nbBallesTotal){

		// Caract�ritiques al�atoires pour les balles
		const int rayon = Aleatoire_MinMax(BALLE_RAYON_MIN, BALLE_RAYON_MAX);
		const cpVect centre = Aleatoire_Position(rayon);
		const cpVect direction = Aleatoire_Direction();
		const Uint32 couleur = Aleatoire_Couleur();
		const char lettre = Aleatoire_Lettre();

		// Cr�ation de la balle
		//balles[nbBallesCrees] = Balle_creer(ecran, espace, centre, direction, rayon, couleur, lettre);
		g_ptr_array_add(envJeu->balles, Balle_creer(envJeu->ecran, envJeu->espace, centre, direction, rayon, couleur, lettre));
		envJeu->nbBallesCrees++;
	}
	else {
		envJeu->ajouterBalles = 0;
	}			
}

/**
 * @fn void _creerBalles(int nbBalles)
 * @brief Fonction qui demande � l'environnement de cr�� n balles.
 *
 * A l'appel de cette fonction une seule balle est r��lement cr��e (@see nbBallesCrees)
 * Les suivantes sont cr��s toutes les n secondes (@see timerLancement, 
 * @see DELAI_APPARITION dans config.h) par la fonction @see GestionEnv_evoluer
 *
 * @param fonction Fonction provenant de la biblioth�que Balle
 * ayant aucun retour et pour seul param�tre une balle
 */
static void _creerBalles(Environnement* envJeu, int nbBalles){

	envJeu->balles = g_ptr_array_sized_new(nbBalles);
	envJeu->ajouterBalles = 1;

	if(envJeu->balles){
		envJeu->nbBallesTotal = nbBalles;	
		envJeu->nbBallesCrees = 0;	
		_creerUneBalle(envJeu);		
	}
}

/**
 * @fn void _supprimerBalles()
 * @brief Fonction qui supprime toutes les balles
 * pr�sentent dans l'environnement
 */
static void _supprimerBalles(GPtrArray* balles){

//	GestionEnv_effacerPanier(); /* VITESSE */
//	GestionEnv_viderZoneMessage(); /* VITESSE */
	_Balle_foreach(balles, Balle_supprimer);
	 g_ptr_array_free(balles, 0);
}

/**
 * @fn int GestionEnv_ballesImmobiles()
 * @brief Fonction qui retourne 1 si toutes les balles 
 * de l'environnement sont immobiles (@see Balle_estImmobile), 0 sinon
 */
int GestionEnv_ballesImmobiles(Environnement* envJeu){

	int ballesImmobiles = 1, i = 0;

	while(ballesImmobiles && i < envJeu->nbBallesCrees){
		ballesImmobiles = Balle_estImmobile(g_ptr_array_index(envJeu->balles, i++));
	}	

	return ballesImmobiles;	
}

//-------------------------------------------------------------------------------------------------------------
//								Gestion tracer ligne & r�cup�ration des caract�res
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn char* GestionEnv_donnerCaracteresLigne(int x1, int y1, int x2, int y2)
 * @brief Fonction qui affiche une ligne de (x1,y1) � (x2,y2) et renvoie
 * la chaine de caract�res form�e par les lettres des balles travers�s
 * par cette ligne
 *
 * @param x1 Abscisse du point de d�part de la ligne
 * @param y1 Ordonn�e du point de d�part de la ligne
 * @param x2 Abscisse du point final de la ligne
 * @param y2 Ordonn�e du point final de la ligne
 *
 * @return Cha�ne de caract�re form�e par les lettres des balles 
 * travers�s par la ligne
 */
typedef int (*FonctionComparer)(const void *, const void *);

#define DROITE -1
#define GAUCHE 1

static int sensLigne = GAUCHE;
static int _comparerBalles(const Balle** a, const Balle** b){
	return ((sensLigne == DROITE) ?  (*a)->cx -  (*b)->cx :  (*b)->cx -  (*a)->cx);
}

char* GestionEnv_donnerCaracteresLigne(Environnement* envJeu, int x1, int y1, int x2, int y2){

	// Dessine la ligne
	lineColor(envJeu->ecran, x1, y1, x2, y2, 0x000000FF);

	// Cherche les balles touch�es
	// 80 = diam�tre max d'une balle (voir _creerUneBalle)
	const unsigned int nbBallesMaxDiag = ceil(sqrt(pow(HAUTEUR_ECRAN,2) + 
										      pow(LARGUEUR_ECRAN,2))/(2*BALLE_RAYON_MAX)) * 2;
/* DEBUG 	printf("max: %d\n", nbBallesMaxDiag); */
	unsigned int nbBallesTouches = 0;	
	Balle** ballesTouches = calloc(nbBallesMaxDiag, sizeof(Balle*));	

	for(unsigned int i = 0; i < envJeu->nbBallesCrees; i++){
		
		// V�rifie si la balle est touch�e par la ligne
		int touche =  cpShapeSegmentQuery(
						   ((Balle*)g_ptr_array_index(envJeu->balles, i))->zoneCollision, 
						   cpv(x1, HAUTEUR_ECRAN - y1), 
						   cpv(x2, HAUTEUR_ECRAN - y2),
						   NULL
		);

		// Sauvegarde la balle touch�e
		if(touche){
			ballesTouches[nbBallesTouches] = (Balle*)g_ptr_array_index(envJeu->balles, i);
			nbBallesTouches++;
		}
	}
/* DEBUG 	printf("touches: %d\n", nbBallesTouches); */

	// Met les balles dans l'ordre suivant le trac� de la ligne
	// pour avoir les caract�res dans le bon ordre (pour version 1 & 2 de l'algo de recherche)
	sensLigne = ((x1 < x2) ? DROITE : GAUCHE);
	qsort(ballesTouches, nbBallesTouches, sizeof(Balle*), (FonctionComparer) _comparerBalles);

	// Construit la chaine de caract�res
	char* lettres = (char*) calloc(nbBallesTouches + 1, sizeof(char));

	for(unsigned int i = 0; i < nbBallesTouches; i++){
		char l [2] = { ballesTouches[i]->lettre, '\0' }; // Ajoute z�ro terminal
		strcat(lettres, l);
	}

	// Supprime les balles touch�es du tableau global
	envJeu->nbBallesCrees -= nbBallesTouches;
	for(unsigned int i = 0; i < nbBallesTouches; i++){
		g_ptr_array_remove(envJeu->balles, ballesTouches[i]);
		Balle_supprimer(ballesTouches[i]);	
	}
	free(ballesTouches);

	// Met � jour l'affichage
	SDL_Flip(envJeu->ecran);

	return lettres;
}

static int nbMessages = 0;
void GestionEnv_afficherMessage(Environnement* envJeu, char* message, int x, int y, int taille){

	if(!nbMessages++){
		boxColor(envJeu->ecran, 0, 0, LARGUEUR_ECRAN, OFFSET*2, 0xA9EAFEFF);
	}

	SDL_Color couleur = {20, 50, 50};
	
	TTF_Font* police = TTF_OpenFont("arial.ttf", taille);
	TTF_SetFontStyle(police, TTF_STYLE_BOLD);

	if(x == ALIGN_CENTRE){
		int w = 0;
		TTF_SizeText(police, message, &w, NULL);
		x = (LARGUEUR_ECRAN - w)/2;
	}

	SDL_Surface* texte = TTF_RenderText_Blended(police, message, couleur);
	SDL_Rect position = {x, y};

	SDL_BlitSurface(texte, NULL, envJeu->ecran, &position);
	SDL_FreeSurface(texte);
	TTF_CloseFont(police);	

	SDL_Flip(envJeu->ecran);

}

void GestionEnv_viderZoneMessage(Environnement* envJeu){
	nbMessages = 0;
	boxColor(envJeu->ecran, 0, 0, LARGUEUR_ECRAN, OFFSET*2, COULEUR_FOND);
	SDL_Flip(envJeu->ecran);
}

//-------------------------------------------------------------------------------------------------------------
//						Fonctions debug (laisser pour compiler testDroiteGauche & testGaucheDroite)
//-------------------------------------------------------------------------------------------------------------

/**
 * @fn cpShape** donnerSol()
 * @brief Fonction qui retourne le sol du panier
 * pour tester le comportement de balle avec chipmunk
 * 
 * @see testDroiteGauche
 * @see testGaucheDroite
 *
 * @return Pointeur sur le sol du panier
 */
cpShape** donnerSol(Environnement* envJeu){
	return envJeu->panier;
}

