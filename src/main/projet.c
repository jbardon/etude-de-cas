#include <stdio.h>
#include <unistd.h> //pause
#include <GestionEnv.h>
#include <AlgoRecherche.h>

typedef struct
{
    char key[SDLK_LAST];
    int mousex,mousey;
    int mousexrel,mouseyrel;
    char mousebuttons[8];
        char quit;
} Input;

void UpdateEvents(Input* in)
{

    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        
        case SDL_MOUSEMOTION:
            in->mousex=event.motion.x;
            in->mousey=event.motion.y;
            in->mousexrel=event.motion.xrel;
            in->mouseyrel=event.motion.yrel;
            break;
        case SDL_MOUSEBUTTONDOWN:
            in->mousebuttons[event.button.button]=1;
            break;
        case SDL_MOUSEBUTTONUP:
            in->mousebuttons[event.button.button]=0;
            break;
        case SDL_QUIT:
            in->quit = 1;
            break;
        default:
            break;
        }
    }
}

int main(void){

	SDL_Surface* ecran = GestionEnv_initSDL();
	cpSpace* espace = GestionEnv_initChipmunk();

	GestionEnv_creerPanier(espace, ecran);
	GestionEnv_creerBalles(35);

/* DEBUT TEST */

	do {		
		GestionEnv_evoluer();
	}
	while(!GestionEnv_ballesImmobiles());

	printf("Cliquez dans la fenêtre pour tracer une ligne\n");
	cpVect coord[2];
	Input in;
	int nbClic = 0;

    memset(&in, 0, sizeof(in));
    while(nbClic < 2){
        UpdateEvents(&in);
        if (in.mousebuttons[SDL_BUTTON_LEFT]){
			filledCircleColor(ecran, in.mousex, in.mousey, 5, 0x000000FF);
//printf("Clic %d: %d,%d\n", nbClic, in.mousex, in.mousey);
            in.mousebuttons[SDL_BUTTON_LEFT] = 0;
//printf("x: %d, y: %d\n", in.mousex, in.mousey);
			coord[nbClic] = cpv(in.mousex, in.mousey);
			nbClic++;
			SDL_Flip(ecran);			
        }            
    }

	char* lettres = GestionEnv_donnerCaracteresLigne(coord[0].x, coord[0].y, coord[1].x, coord[1].y);

	printf("Lettres sélectionnées: %s\n", lettres);

	GHashTable* dico = chargerDico(RES("/AlgoRecherche/dicofinal.txt"));

	for(int i = 0; i < strlen(lettres); i++){
	  lettres[i] = tolower(lettres[i]);
	}


	char* result = version1(lettres, dico);
	if(result){
		printf("Mot trouvé ! %s (%d pts)\n", result, strlen(result));
	}
	else {
		printf("Aucun mot trouvé =(\n");
	}

/* FIN TEST */

	pause();

	GestionEnv_supprimerBalles();
	GestionEnv_supprimerPanier();

	GestionEnv_quitChipmunk();
	GestionEnv_quitSDL();

	return 0;
}
