#include <stdio.h>
#include <stdlib.h>

int main(void){

	if(fopen("fichier.txt", "r" ) == NULL ){
		printf("Perdu\n");
	}
	else {
		printf("Gagne\n");
	}

	return EXIT_SUCCESS;
}
