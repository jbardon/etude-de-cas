#ifndef __CONFIG_H__
#define __CONFIG_H__

#define HAUTEUR_ECRAN 600
#define LARGUEUR_ECRAN 700
#define SCREEN_BPP 32

#define COULEUR_FOND 0xFFFFFFFF // Fond de la fenêtre (blanche)
#define OFFSET 50               // Décalage du panier par rapport au bord de la fenêtre
#define REBOND 0.6             // Elasticité balle & murs
#define FRICTION 1
#define DELAI_APPARITION 5     // Délai d'apparition des balles (30(FPS) -> 1 seconde)

#define NB_BALLES 140		    // Nombre de balles à générer
#define BALLE_RAYON_MIN 17      // Rayon minimum d'une balle
#define BALLE_RAYON_MAX 22      // Rayon maximum d'une balle

#endif /* __CONFIG_H__ */
