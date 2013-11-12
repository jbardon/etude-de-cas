
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

	const unsigned int sizeLettres =  sizeof(lettres)/sizeof(float);
	char lettre = 0;
	unsigned int i = sizeLettres/2;
	while(lettre == 0){
		printf("%2.2f %3.2f %3.2f ", proba, lettres[i], lettres[i+1]);
		if(proba >= lettres[i] && proba < lettres[i+1]){
			lettre = (char)(65 + i);		
		}
		else if(proba <= lettres[i]){
			i /= 2;
			printf("inf\n");
		}
		else {
			i += i/2;
			printf("sup\n");
		}
	}	
	
	printf("\nlettre: %c\n", lettre);

	return 0;
}
