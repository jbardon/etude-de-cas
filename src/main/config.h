/**
 * @file config.h
 * @brief Configuration des certains éléments du jeu
 * @author Jérémy.B
 * @version 1.0
 * @date 10 novembre 2013
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

//------------------------------------------------------------------------------
//								Fenêtre SDL
//------------------------------------------------------------------------------

/** @brief Hauteur */
#define HAUTEUR_ECRAN 600

/** @brief Largueur */
#define LARGUEUR_ECRAN 700

/** @brief Nombre d'octets par pixels */
#define SCREEN_BPP 32

/** @brief Couleur de fond (blanc) */
#define COULEUR_FOND 0xFFFFFFFF 

//------------------------------------------------------------------------------
//							Affichage et physique
//------------------------------------------------------------------------------

/** @brief Décalage du panier par rapport au bord de la fenêtre */
#define OFFSET 50            

/** @brief Elasticité des murs et des balles (voir Chipmunk/cpBodySetElasticity) */
#define REBOND 0.6            

/** @brief Friction des éléments */
#define FRICTION 1

/** @brief Délai d'apparition des balles (30(FPS) -> 1 seconde) */
#define DELAI_APPARITION 5      

//------------------------------------------------------------------------------
//									Balles
//------------------------------------------------------------------------------

/** @brief Nombre de balles à générer */
#define NB_BALLES 140		    

/** @brief Rayon minimum d'une balle */
#define BALLE_RAYON_MIN 17     

/** @brief Rayon maximum d'une balle */
#define BALLE_RAYON_MAX 22  

#endif /* __CONFIG_H__ */




