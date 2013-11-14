#ifndef __CONFIG_H__
#define __CONFIG_H__

#define HAUTEUR_ECRAN 480
#define LARGUEUR_ECRAN 640
#define SCREEN_BPP 32

#define COULEUR_FOND 0xFFFFFFFF // Fond de la fenêtre (blanche)
#define OFFSET 50               // Décalage du panier par rapport au bord de la fenêtre
#define REBOND 0.95             // Elasticité balle & murs
#define FRICTION 1
#define DELAI_APPARITION 60     // Délai d'apparition des balles (60(FPS) -> 1 seconde)

#define RES_PATH "/home/mickael/Bureau/Perso/Scolaire/L3info/S1/Etude_de_cas/etude-de-cas/res/fichiers"
#define RES(file) RES_PATH file

#endif /* __CONFIG_H__ */
