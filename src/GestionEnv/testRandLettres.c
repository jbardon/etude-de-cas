
#include <stdio.h>

static const float lettres [] = {
									 9.78,
									11.16,
									14.53,
									16.87,
									31.56,
									32.89,
									34.47,
									35.62,
									45.13,
									45.31,
									45.36,
									49.35,
									51.88,
									59.32,
									65.18,
									67.51,
									68.01,
									76.69,
									86.92,
									93.80,
									97.36,
									98.31,
									98.32,
									98.57,
									98.91,
								   100.00,
							    };

int main(void){

	srand(time(NULL));
	const float proba = (rand()%10000)/100.0;
	printf("proba: %2.2f\n", proba);

	const unsigned int sizeLettres =  sizeof(lettres)/sizeof(float);
	char lettre = 0;

	unsigned int i = 0;
	while(lettre == 0 && i < sizeLettres){	
		if(proba <= lettres[i++]){
			lettre = (char)(65 + i - 1); //à cause du ++ dans le if		
		}
	}	
	
	printf("lettre: %c\n", lettre);

	return 0;
}
