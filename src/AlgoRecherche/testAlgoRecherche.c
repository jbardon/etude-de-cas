#include "../main/config.h"
#include "AlgoRecherche.h"

int main()
{	

	//******************************************* Cr�ation des dictionnaires *******************************************//
	//cr�ation des dictionnaire avec la table de hashage
	GHashTable* dicoV1_V2 = chargerDicoV1_V2("dicofinal.txt"); //pour la version 1 & 2
	GHashTable* dicoV3 = chargerDicoV3("dicofinal.txt"); //pour la version 3

	//********************************************** TEST DE LA VERSION 1 **********************************************//
	printf("\n--------> Test de la Version 1 <-------- \n");

	char* resultatV1 = "abaissai"; //resultat du test
	printf("Resultat attendu, value : %s, points : %d\n",resultatV1, (int)strlen(resultatV1));

	char* ver1 = version1(dicoV1_V2, "zedabaissaidgf");
	if(ver1)
	{
		printf("Chaine la plus longue : %s, vous marquez %d points \n", ver1, (int)strlen(ver1)); 
		printf("\n ===> TEST VERSION 1 .......... OK \n");
	}
	else 
	{
		printf("Aucun mot trouv� pour la version 1\n");
		printf("\n ===> TEST VERSION 1 .......... KO \n");
	}

	//********************************************** TEST DE LA VERSION 2 **********************************************//
	printf("\n--------> Test de la Version 2 <-------- \n");

	char* resultatV2 = "aluminium";
	printf("Resultat attendu, value : %s, points : %d\n",resultatV2, (int)strlen(resultatV2));

	char* ver2 = version2(dicoV1_V2,"zaluqpminiuwwm");
	if(ver2)
	{
		printf("Chaine la plus longue : %s, vous marquez %d points \n", ver2, (int)strlen(ver2)); 
		printf("\n ===> TEST VERSION 2 .......... OK \n");
	}
	else 
	{
		printf("Aucun mot trouv� pour la version 2\n");
		printf("\n ===> TEST VERSION 2 .......... KO \n");
	}

	//********************************************** TEST DE LA VERSION 3 **********************************************//
	printf("\n--------> Test de la Version 3 <-------- \n");
	
	/*
	printf("--> Test de la recherche d'anagramme\n");
	printf("Anagramme de argent : %s\n", RechercheAnagramme("argent",dicoV3));
	printf("Anagramme de resurrection : %s\n\n", RechercheAnagramme("resurrection",dicoV3));
	*/

	char* resultatV3 = "derealisant";
	printf("Resultat attendu, value : %s, points : %d\n",resultatV3, (int)strlen(resultatV3));

	char* ver3=version3(dicoV3,"rstlneaiaed");
	if(ver3)
	{
		printf("Chaine la plus longue : %s, vous marquez %d points \n", ver3, (int)strlen(ver3)); 
		printf("\n ===> TEST VERSION 3 .......... OK \n");
	}
	else 
	{
		printf("Aucun mot trouv� pour la version 3\n");
		printf("\n ===> TEST VERSION 3 .......... KO \n");
	}

	//***************************************** Destruction des dictionnaires *****************************************//
	g_hash_table_destroy(dicoV1_V2);
	supp(dicoV3);
	g_hash_table_destroy(dicoV3);

	return EXIT_SUCCESS;
}





