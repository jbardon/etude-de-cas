#include <stdio.h>
#include <math.h>

#include <unistd.h> //pause
#include <GestionEnv.h>
#include <Balle.h>

int main(void){

  //Créé la fenêtre de jeu
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface* ecran = SDL_SetVideoMode(LARGUEUR_ECRAN, HAUTEUR_ECRAN, SCREEN_BPP, SDL_HWSURFACE);
  SDL_FillRect(ecran, NULL, COULEUR_FOND);
  SDL_WM_SetCaption("Projet étude de cas", NULL);
  
  //Créé un espace vide et spécifie sa gravitée
  cpSpace* space = cpSpaceNew();
  cpVect gravity = cpv(0, -100);  
  cpSpaceSetGravity(space, gravity);

  GestionEnv_creerPanier(space, ecran, 50);

  Balle* balle1 = Balle_creer(space, ecran, 100, 0, 50, SDL_MapRGB(ecran->format, 255, 0, 255));
  SDL_Flip(ecran);
    
  //Evolution des objets de l'espace dans le temps
  cpFloat timeStep = 1.0/60.0;
  for(cpFloat time = 0; time < 6; time += timeStep){
    cpVect pos = Balle_donneCoordonnees(balle1);
    //cpVect vel = cpBodyGetVel(ballBody);
    //cpFloat ang = cpBodyGetAngle(ballBody);
    /*printf(
      "Time is %5.2f. ballBody is at (%5.2f, %5.2f). It's velocity is (%5.2f, %5.2f)\n",
      time, pos.x, pos.y, vel.x, vel.y
    );*/

    printf(
      "Time is %5.2f. ballBody is at (%5.2f, %5.2f)\n",
      time, pos.x, pos.y
    );

    cpSpaceStep(space, timeStep);
    Balle_deplacer(balle1);
  }
 
  pause();
  
  //Détruit la fenêtre
  SDL_FreeSurface(ecran);
  SDL_Quit();
  
  //Détruit l'espace physique
  Balle_supprimer(balle1);
  GestionEnv_supprimerPanier();
  cpSpaceFree(space);

  return 0;
}
