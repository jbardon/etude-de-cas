#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <unistd.h> //pause
#include <config.h>

#include <stdlib.h>

int main(void){

SDL_Surface *texte = NULL;
TTF_Font *police = NULL;
SDL_Rect position;

SDL_Init(SDL_INIT_VIDEO);
TTF_Init();

//Créé la fenêtre de jeu
SDL_Init(SDL_INIT_VIDEO);
SDL_Surface* ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);
SDL_FillRect(ecran, NULL, COULEUR_FOND);
SDL_WM_SetCaption("Projet étude de cas", NULL);
 

if(TTF_Init() == -1)
{
    fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
}

SDL_Color couleurNoire = {0, 0, 0};
police = TTF_OpenFont("arial.ttf", 65);

texte = TTF_RenderText_Blended(police, "T", couleurNoire);

position.x = 0;
position.y = 0;
SDL_BlitSurface(texte, NULL, ecran, &position); 

position.x = 130;
position.y = 120;
SDL_BlitSurface(texte, NULL, ecran, &position);
SDL_Flip(ecran);

TTF_CloseFont(police);  //Doit être avant TTF_Quit() 
TTF_Quit();

  pause();
  
  //Détruit la fenêtre
  SDL_FreeSurface(ecran);
  SDL_Quit();

  return 0;
}
