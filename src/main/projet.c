#include <stdio.h>
#include <math.h>

#include <unistd.h> //pause
#include <GestionEnv.h>
#include <Balle.h>

int main(void){

	SDL_Surface* ecran = GestionEnv_initSDL();
	cpSpace* espace = GestionEnv_initChipmunk();

	GestionEnv_creerPanier(espace, ecran);

	Balle* balle1 = Balle_creer(ecran, espace, cpv(100,0), cpvzero, 50, 0x00FF00FFF, 'A');
	SDL_Flip(ecran);

	//Evolution des objets de l'espace dans le temps
	cpFloat timeStep = 1.0/60.0;
	for(cpFloat time = 0; time < 8; time += timeStep){
	cpVect pos = Balle_donneCoordonnees(balle1);
	float ang = Balle_donneAngleDeg(balle1);
	//cpVect vel = cpBodyGetVel(ballBody);
	//cpFloat ang = cpBodyGetAngle(ballBody);
	/*printf(
	"Time is %5.2f. ballBody is at (%5.2f, %5.2f). It's velocity is (%5.2f, %5.2f)\n",
	time, pos.x, pos.y, vel.x, vel.y
	);*/

	printf(
	"Temps %5.2f balle(%5.2f, %5.2f) angle %f\n",
	time, pos.x, pos.y, ang
	);

	cpSpaceStep(espace, timeStep);
	Balle_deplacer(balle1);
	}

	pause();

	//Détruit la fenêtre
	Balle_supprimer(balle1);
	//Détruit l'espace physique
	GestionEnv_supprimerPanier();
	
	GestionEnv_quitChipmunk();
	GestionEnv_quitSDL();

	return 0;
}
