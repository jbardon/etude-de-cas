/**
 * @file Aleatoire.h
 * @brief Gestion de l'aléatoire pour différents éléments
 *
 * Gestion de l'aléatoire pour un entier, une direction, une lettre, une position et une couleur
 *
 */

#ifndef __ALEATOIRE_H__
#define __ALEATOIRE_H__

#include <time.h>

#include <config.h>
#include <chipmunk.h>
#include <SDL_gfxPrimitives.h>

void Aleatoire_init();

unsigned int Aleatoire_MinMax(int min, int max);
Uint32 Aleatoire_Couleur();
cpVect Aleatoire_Direction();
char Aleatoire_Lettre();
cpVect Aleatoire_Position(int rayon);

#endif /* __ALEATOIRE_H__ */
