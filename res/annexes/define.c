#include <stdio.h>
#define RES_PATH "/var/res"
#define PATH(file) RES_PATH file

int main(void){
	printf("chemin: %s\n", PATH("/Balle/arial.ttf"));
	return 0;
}
