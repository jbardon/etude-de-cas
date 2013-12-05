/**
 * @file Aleatoire.h
 * @brief 
 * @author Jérémy.B
 * @version 1.0
 * @date 10 novembre 2013
 *
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
