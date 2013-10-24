#include <stdio.h>
#include <math.h>

#include <unistd.h> //pause
#include <CreationEnv.h>

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
  
  CreationEnv_creerPanier(space, ecran, 50);
 
  //Caractéristiques physiques de la balle
  cpFloat radius = 50;
  cpFloat mass = 1;
  cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);
  
  //Créé une balle dont la position initiale est -150, 50
  cpBody* ballBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
  cpBodySetPos(ballBody, cpv(500, HAUTEUR_ECRAN));
  
  //Zone de collision de la balle
  cpShape* ballShape = cpSpaceAddShape(space, cpCircleShapeNew(ballBody, radius, cpvzero));
  cpShapeSetFriction(ballShape, 1);
  
  //Créé la balle graphique
  SDL_Ex_Cercle cercle = {500, 0, 50, SDL_Ex_Couleur(255,50,50)};
  SDL_Ex_DessinerCercle(ecran, &cercle);
  
  SDL_Flip(ecran);
    
  //Evolution des objets de l'espace dans le temps
  cpFloat timeStep = 1.0/60.0;
  for(cpFloat time = 0; time < 20; time += timeStep){
    cpVect pos = cpBodyGetPos(ballBody);
    cpVect vel = cpBodyGetVel(ballBody);
	
	cpFloat ang = cpBodyGetAngle(ballBody);
    printf(
      "Time is %5.2f. ballBody is at (%5.2f, %5.2f). It's velocity is (%5.2f, %5.2f)\n",
      time, pos.x, pos.y, vel.x, vel.y
    );
    printf("angle: %f\n",ang);
    cpSpaceStep(space, timeStep);
    SDL_Ex_DeplacerCercle(ecran, &cercle, point(pos.x, HAUTEUR_ECRAN - pos.y), COULEUR_FOND);
  }
 
  pause();
  
  //Détruit la fenêtre
  SDL_FreeSurface(ecran);
  SDL_Quit();
  
  //Détruit l'espace physique
  cpShapeFree(ballShape);
  cpBodyFree(ballBody);
  CreationEnv_supprimerPanier();
  cpSpaceFree(space);

  return 0;
}
