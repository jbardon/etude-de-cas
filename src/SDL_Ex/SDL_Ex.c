#include "SDL_Ex.h"

/*
 * Bibliothèque modifiée par Bardon Jérémy
 *
 * Fichiers d'origine : 
 * Fichier exemple de la 2ème partie du tutoriel SDL d'Anomaly
 * http://anomaly.developpez.com/tutoriel/sdl/partie2/
 * Ce fichier source est libre de droits.
 */

//------------------------------------------------------------------------------
//								Fonctions internes
//------------------------------------------------------------------------------
static void setPixel(SDL_Surface* surf, int x, int y, Uint32 coul){
  *((Uint32*)(surf->pixels) + x + y * surf->w) = coul;
}

static void setPixelVerif(SDL_Surface* surf, int x, int y, Uint32 coul){
  if(x >= 0 && x < surf->w && y >= 0 && y < surf->h){
    setPixel(surf, x, y, coul);
  }
}

static void echangerEntiers(int* x, int* y){
  int t = *x;
  *x = *y;
  *y = t;
}

SDL_Ex_Point point(int x, int y){
	SDL_Ex_Point p; p.x = x; p.y = y;
	return p;
}
//------------------------------------------------------------------------------
//								Fonctions de dessin
//------------------------------------------------------------------------------
void SDL_Ex_DessinerLigneHoriz(SDL_Surface* surf, int x, int y, int l, int e, Uint32 coul){
	SDL_Rect rect;

	rect.x = x; rect.y = y + e/2;
	rect.w = l; rect.h = e;	
	
	if(e > 1 && e%2 != 0){ //epaisseur impaire
		rect.h++;
	}

	SDL_FillRect(surf, &rect, coul);
}

void SDL_Ex_DessinerLigneVert(SDL_Surface* surf, int x, int y, int h, int e, Uint32 coul){
	SDL_Rect rect;

	rect.x = x - e/2; rect.y = h - y;
	rect.w = e; rect.h = h;	
	
	if(e > 1 && e%2 != 0){ //epaisseur impaire
		rect.w++;
	}

	SDL_FillRect(surf, &rect, coul);
}

void SDL_Ex_DessinerLigne(SDL_Surface* surf, SDL_Ex_Point debut, SDL_Ex_Point fin, Uint32 coul){

  int d, dx, dy, aincr, bincr, xincr, yincr, x, y;

  if(abs(fin.x - debut.x) < abs(fin.y - debut.y)){
    /* parcours par l'axe vertical */

    if(debut.y > fin.y){
      echangerEntiers(&debut.x, &fin.x);
      echangerEntiers(&debut.y, &fin.y);
    }

    xincr = fin.x > debut.x ? 1 : -1;
    dy = fin.y - debut.y;
    dx = abs(fin.x - debut.x);
    d = 2 * dx - dy;
    aincr = 2 * (dx - dy);
    bincr = 2 * dx;
    x = debut.x;
    y = debut.y;

    setPixelVerif(surf, x, y, coul);

    for(y = debut.y+1; y <= fin.y; ++y){
      if(d >= 0){
		x += xincr;
		d += aincr;
      } 
      else {
		d += bincr;
	  }
      setPixelVerif(surf, x, y, coul);
    }

  } 
  else {
    /* parcours par l'axe horizontal */
    
    if(debut.x > fin.x){
      echangerEntiers(&debut.x, &fin.x);
      echangerEntiers(&debut.y, &fin.y);
    }

    yincr = fin.y > debut.y ? 1 : -1;
    dx = fin.x - debut.x;
    dy = abs(fin.y - debut.y);
    d = 2 * dy - dx;
    aincr = 2 * (dy - dx);
    bincr = 2 * dy;
    x = debut.x;
    y = debut.y;

    setPixelVerif(surf, x, y, coul);

    for(x = debut.x+1; x <= fin.x; ++x){
      if(d >= 0){
		y += yincr;
		d += aincr;
      } 
      else {
		d += bincr;
	  }
      setPixelVerif(surf, x, y, coul);
    }
  }    
}

void SDL_Ex_DessinerCercle(SDL_Surface* surf, const SDL_Ex_Cercle* cercle){

  int d, y, x;

  d = 3 - (2 * cercle->rayon);
  x = 0;
  y = cercle->rayon;

  while(y >= x){
  	if(y == cercle->rayon - 5){
  		SDL_Ex_DessinerLigneHoriz(surf, cercle->cx - x, cercle->cy - y, 
  									        2 * x + 1, 1, SDL_Ex_Couleur(0,0,255));
  	}
  	else{
		SDL_Ex_DessinerLigneHoriz(surf, cercle->cx - x, cercle->cy - y, 
							   			    2 * x + 1, 1, cercle->couleur);
	}
    SDL_Ex_DessinerLigneHoriz(surf, cercle->cx - x, cercle->cy + y, 
    					   				2 * x + 1, 1, cercle->couleur);
    					   
    SDL_Ex_DessinerLigneHoriz(surf, cercle->cx - y, cercle->cy - x, 
    					   				2 * y + 1, 1, cercle->couleur);
    					   
    SDL_Ex_DessinerLigneHoriz(surf, cercle->cx - y, cercle->cy + x,
    					   				2 * y + 1, 1, cercle->couleur);

    if(d < 0){
      d = d + (4 * x) + 6;
    }
    else {
      d = d + 4 * (x - y) + 10;
      y--;
    }
    x++;
  }
}

void SDL_Ex_DeplacerCercle(SDL_Surface* surf, SDL_Ex_Cercle* cercle, SDL_Ex_Point c_dest, Uint32 couleurFond){
	
	//Efface le cercle
	Uint32 tmpcouleur = cercle->couleur;
	cercle->couleur = couleurFond;
	SDL_Ex_DessinerCercle(surf, cercle);
	
	//Deplace et affiche le cercle
	cercle->couleur = tmpcouleur;
	cercle->cx = c_dest.x;
	cercle->cy = c_dest.y;
	SDL_Ex_DessinerCercle(surf, cercle);
	
	//MAJ de l'affichage
	SDL_Flip(surf);
}

unsigned long SDL_Ex_Couleur(int R, int G, int B){ //Convertit RGB en long
	return 65536*R + 256*G + B;
}
