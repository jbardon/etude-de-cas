#include "AlgoRecherche.h"

//Fonctions locales
static char *_str_sub (const char *s, unsigned int start, unsigned int end);
static void _supprimer_retour_ligne(char *chaine);

GHashTable* chargerDico(char* nomFichier){
	
	GHashTable* DicoHash = g_hash_table_new(g_int_hash, g_int_equal);

	if(DicoHash){
		int valhash = 0; //valeur de la fonction de hashage
	
		FILE* fichier = NULL; //definition de la variable fichier
		fichier = fopen(nomFichier,"r"); //ouverture du fichier dico.txt en mode r (lecture).

		//test pour savoir si le fichier est bien ouvert
		if (fichier != NULL)
		{
			char line[128];
			while(fgets(line, sizeof line, fichier)!=NULL) //lire une ligne
			{
				_supprimer_retour_ligne(line);
				printf("ligne %s, gstrh %d , taille %d \n", line, g_str_hash(line), strlen(line));
				valhash = g_str_hash(line); //récupère la valeur de la fonction de hashage

				int* tmpval = malloc(sizeof(int)); //création d'une nouvelle variable à chaque tour de boucle...
				*tmpval = valhash; //...qui contient la valeur de hashage
			
				char* tmp = malloc(strlen(line)+1); //création d'une variable à chaque tour de boucle...
				strcpy(tmp,line); //...qui contient la valeur de la ligne lue

				g_hash_table_insert(DicoHash, tmpval, tmp); //Insertion dans le hashtable
			}
	
			fclose(fichier); //on ferme le fichier qui à été ouvert		
		}
		else //si le fichier n'a pas été ouvert
		{
			printf("Impossible d'ouvrir le fichier dico.txt \n");
		}
	}
	return DicoHash;
}

/*
* Permet d'extraire la chaine de caractère de la chaine s qui est comprise entre les positions start et end.
* Si start==end, renvoie le caractère à la position de start et end.
* Paramètres :
* 	s, la chaine de départ
*	start, la position de départ
* 	end, la position de fin
*/
static char *_str_sub (const char *s, unsigned int start, unsigned int end)
{
	char *new_s = NULL; //initialisation de la chaine que l'on retorunera
	//si la chaine que l'on passe n'est pas NULL et que les positions sont OK
	if (s != NULL && start <= end)
	{
		new_s = malloc (sizeof (*new_s) * (end - start + 2)); //allocation de la chaine que l'on retournera
		if (new_s != NULL)
		{
			int i; //initialise i pour la boucle for
			for (i = start; i <= end; i++)
			{
				new_s[i-start] = s[i]; //on remplit la chaine que l'on renverra
			}
			new_s[i-start] = '\0';
		}
		else
		{
			fprintf (stderr, "Memoire insuffisante\n");
			exit (EXIT_FAILURE);
		}
	}
	return new_s;
}

char* version1(char* chaine, GHashTable* table)
{
	/*int valhash =0;
	//valeur du hashage de la chaine de caractere
	valhash= g_str_hash(chaine);
	//si la valeur de hashage est dans la table
	if(g_hash_table_contains(table, valhash))
	{
		g_hash_table_lookup(table, valhash);
		return chaine;
	}
	for(int i=0; i<strlen(chaine); i++)
	{
		
	}*/
	char* substring;
	printf("Chaine de depart : %s \n",chaine);
	for(int i=0; i<strlen(chaine); i++)
	{
		for(int j=i; j<strlen(chaine); j++)
		{
			substring = _str_sub(chaine,i,j);
			const int h = g_str_hash(substring);
		
			printf("Sous-chaine : %s , valeur de hash : %d , taille de la chaine %d \n",substring, h, strlen(substring));	

			if(g_hash_table_contains(table, &h))
			{
				printf("OK");
			}
			
		}
	}
	return substring;
	//strcmp
}

static void _supprimer_retour_ligne(char *chaine)
{
	char *p = strchr(chaine, '\r');
	if (p)
	{
		*p = 0;
	}
	/*p = strchr(chaine, '\r');
	if (p)
	{
		*p = '8';
	}*/
}
