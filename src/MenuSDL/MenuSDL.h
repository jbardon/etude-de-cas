/**
 * @file MenuSDL.h
 * @brief Affichage d'un menu pour les scores au algorithmes
 *
 * Construction automatique d'une surface SDL à partir 
 * de données passés en paramètre
 *
 * Utilisé pour afficher les lettres sélectionnée et les scores
 * aux 3 algorithmes de recherche de mots
 *
 */

#ifndef __MENU_SDL_H__
#define __MENU_SDL_H__

#include <SDL.h>
#include <SDL_ttf.h>
#include <config.h>

/**
 * @struct Solution
 * @brief Représente une solution 
 *
 * Une solution est le regroupement d'un mot trouvé (par les algos recherche)
 * et du score lié à longueur du mot
 *
 * @warning Le score peut ne pas être lié à la longueur du mot c'est pour cela qu'il a une variable dédiée
 *
 */
//typedef struct _Solution Solution; /* Documentation plus propre */
//struct _Solution {
typedef struct {
	char* mot; /*!< Mot trouvé par un algorithme de recherche */
	int score; /*!< Score lié au mot trouvé */
} Solution;

extern Solution* Solution_creer(char* mot, int score);
extern void Solution_supprimer(Solution* motTrouve);

extern SDL_Surface* MenuSDL_creer(char* lettres, Solution** motsTrouves, int nbMotsTrouves);

#endif /* __MENU_SDL_H__ */



