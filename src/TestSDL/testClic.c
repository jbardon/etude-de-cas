#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <unistd.h> //pause


#include <stdlib.h>

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

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const Uint32 COULEUR_FOND = 0xFFFFFFFF; 

int main(void){

SDL_Init(SDL_INIT_VIDEO);


  //Créé la fenêtre de jeu
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface* ecran = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE);
  SDL_FillRect(ecran, NULL, COULEUR_FOND);
  SDL_WM_SetCaption("Projet étude de cas", NULL);
  SDL_Flip(ecran);
 
    Input in;
    // init SDL, chargement, tout ce que vous faites avant la boucle.
    memset(&in,0,sizeof(in));
    while(!in.key[SDLK_ESCAPE] && !in.quit)
    {
        UpdateEvents(&in);
        if (in.mousebuttons[SDL_BUTTON_LEFT])
        {
            in.mousebuttons[SDL_BUTTON_LEFT] = 0;
            printf("x: %d, y: %d\n", in.mousex, in.mousey);
			
        }       
        
        
    }

  pause();
  
  //Détruit la fenêtre
  SDL_FreeSurface(ecran);
  SDL_Quit();

  return 0;
}
