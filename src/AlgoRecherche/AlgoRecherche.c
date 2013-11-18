#include "AlgoRecherche.h"

/*
* Fonctions locales
*/
static char *_str_sub (const char *s, unsigned int start, unsigned int end);
static void _supprimer_retour_ligne(char *chaine);

/*
* Permet la cr�ation du dictionnaire
* Param�tres :
* 	nomFichier, e fichier contenant le dictionnaire.txt
*/
GHashTable* chargerDico(char* nomFichier){
	
	GHashTable* DicoHash = g_hash_table_new_full(g_int_hash, g_int_equal, free, free);

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
				_supprimer_retour_ligne(line); //suppression du retour � la ligne
//printf("ligne %s, gstrh %d , taille %d \n", line, g_str_hash(line), strlen(line));
				valhash = g_str_hash(line); //r�cup�re la valeur de la fonction de hashage

				int* tmpval = malloc(sizeof(int)); //cr�ation d'une nouvelle variable � chaque tour de boucle...
				*tmpval = valhash; //...qui contient la valeur de hashage
			
				char* tmp = g_strdup(line); //cr�ation d'une variable � chaque tour de boucle...
				strcpy(tmp,line); //...qui contient la valeur de la ligne lue

				g_hash_table_insert(DicoHash, tmpval, tmp); //Insertion dans le hashtable
			}
	
			fclose(fichier); //on ferme le fichier qui � �t� ouvert		
		}
		else //si le fichier n'a pas �t� ouvert
		{
			printf("Impossible d'ouvrir le fichier dico.txt \n");
		}
	}
	return DicoHash;
}

/*
* Permet d'extraire la chaine de caract�re de la chaine s qui est comprise entre les positions start et end.
* Si start==end, renvoie le caract�re � la position de start et end.
* Param�tres :
* 	s, la chaine de d�part
*	start, la position de d�part
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

/*
* Permet la recherche de la plus longue sous-chaine dans une chaine
* sous-chaine est contenue dans le dico
* Param�tres :
* 	table, hashtable contenant le dico
*	chaine, les lettres de d�part
*/
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
	char* chaine_retour = NULL; //variable qui contient le chaine � retourner
	char* substring; //variable qui contiendra chaque sous-chaine
	int taille=0;
//printf("Chaine de depart : %s \n",chaine);
	for(int i=0; i<strlen(chaine); i++) //on parcourt la chaine
	{
		for(int j=i; j<strlen(chaine); j++) //on parcourt la chaine � partir de i
		{
			substring = _str_sub(chaine,i,j); //on extrait la sous chaine entre i et j
			const int h = g_str_hash(substring); //on recup�re la valeur de hash de la sous-chaine ...
			
			if(g_hash_table_contains(table, &h)) //... on regarde si cette valeur est dans la hashtable
			{
				if(strlen(substring) > taille) //si la taille est plus grande que celui trouver avant
				{
					taille=strlen(substring); //la taille change
					chaine_retour=substring; //la chaine de retour est cette nouvelle chaine
				}
			}
		}
	}
	return chaine_retour;
	//strcmp
}

char* version2(char* chaine, GHashTable* table)
{
	char* chaine_retour=NULL;
	char* sub2=NULL;
	for(int i=0; i<strlen(chaine); i++)
	{
		for(int j=0; j<strlen(chaine)-1; j++)
		{
			if(i!=0 && j!=0)
			{
				chaine_retour=_str_sub(chaine,0,j-1);
				sub2=_str_sub(chaine,j+1,strlen(chaine)-1);
				strcat(chaine_retour,sub2);
				printf("Chaine de retour : %s\n",chaine_retour);
			}
		}
	}
	return chaine_retour;
}

/*
* Permet la suppression du retour chariot d'une chaine de caractere
* Param�tres :
* 	chaine, la chaine de caractere � traiter
*/
static void _supprimer_retour_ligne(char *chaine)
{
	char *p = strchr(chaine, '\r'); //on cherche le retour � la ligne
	if (p)
	{
		*p = 0; //et on l'�change contre un 0 (fin de chaine)
	}
	p = strchr(chaine, '\n');
	if (p)
	{
		*p = 0;
	}
	p = strchr(chaine, '\f');
	if (p)
	{
		*p = 0;
	}
	p = strchr(chaine, '\v');
	if (p)
	{
		*p = 0;
	}
}
