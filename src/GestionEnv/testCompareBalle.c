/**
 * @file testCompareBalle.c
 * @brief Test de trie d'un tableau de balle
 * @author Jérémy.B
 * @version 1.0
 *
 * Trie un tableau de balles avec qsort se basant
 * sur son abscisse et le sens de tracé de la ligne
 * qui a sélectionné les balles
 *
 * @see GestionEnv_donnerCaracteresLigne
 *
 */

#include <stdio.h>
#include <unistd.h> //pause
#include <GestionEnv.h>

typedef int (*FonctionComparer)(const void *, const void *);

#define DROITE -1
#define GAUCHE 1

static int sensLigne = GAUCHE;
static int comparerBalles(const Balle** a, const Balle** b){
	return ((sensLigne == DROITE) ?  (*a)->cx -  (*b)->cx :  (*b)->cx -  (*a)->cx);
}

int main(void){

	Environnement* envJeu = GestionEnv_creerEnvironnement();

	Balle* balles[] = {
		Balle_creer(envJeu->ecran, envJeu->espace, cpv(5,1), cpvzero, 2, 0, 'A'),
		Balle_creer(envJeu->ecran, envJeu->espace, cpv(3,1), cpvzero, 2, 0, 'A'),
		Balle_creer(envJeu->ecran, envJeu->espace, cpv(1,1), cpvzero, 2, 0, 'A'),
		Balle_creer(envJeu->ecran, envJeu->espace, cpv(2,1), cpvzero, 2, 0, 'A'),
		Balle_creer(envJeu->ecran, envJeu->espace, cpv(4,1), cpvzero, 2, 0, 'A')
	};

	printf("Test ligne Droite->Gauche\n");
	sensLigne = GAUCHE;
	qsort(balles, 5, sizeof(Balle*), (FonctionComparer) comparerBalles);
	for(unsigned int i = 0; i < 5; i++){
		printf("#%d: x(%d)\n", i, balles[i]->cx);	
	}

	printf("Test ligne Gauche->Droite\n");
	sensLigne = DROITE;
	qsort(balles, 5, sizeof(Balle*), (FonctionComparer) comparerBalles);
	for(unsigned int i = 0; i < 5; i++){
		printf("#%d: x(%d)\n", i, balles[i]->cx);	
	}

	GestionEnv_supprimerEnvironnement(envJeu);

	return 0;
}
