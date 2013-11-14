#include "AlgoRecherche.h"

/*
* Fonctions locales
*/
static char *_str_sub (const char *s, unsigned int start, unsigned int end);
static void _supprimer_retour_ligne(char *chaine);

/*
* Permet la création du dictionnaire
* Paramètres :
* 	nomFichier, e fichier contenant le dictionnaire.txt
*/
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
				_supprimer_retour_ligne(line); //suppression du retour à la ligne
//printf("ligne %s, gstrh %d , taille %d \n", line, g_str_hash(line), strlen(line));
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

/*
* Permet la recherche de la plus longue sous-chaine dans une chaine
* sous-chaine est contenue dans le dico
* Paramètres :
* 	table, hashtable contenant le dico
*	chaine, les lettres de départ
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
	char* chaine_retour = NULL; //variable qui contient le chaine à retourner
	char* substring; //variable qui contiendra chaque sous-chaine
	int taille=0;
//printf("Chaine de depart : %s \n",chaine);
	for(int i=0; i<strlen(chaine); i++) //on parcourt la chaine
	{
		for(int j=i; j<strlen(chaine); j++) //on parcourt la chaine à partir de i
		{
			substring = _str_sub(chaine,i,j); //on extrait la sous chaine entre i et j
			const int h = g_str_hash(substring); //on recupère la valeur de hash de la sous-chaine ...
			
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

/*
* Permet la suppression du retour chariot d'une chaine de caractere
* Paramètres :
* 	chaine, la chaine de caractere à traiter
*/
static void _supprimer_retour_ligne(char *chaine)
{
	char *p = strchr(chaine, '\r'); //on cherche le retour à la ligne
	if (p)
	{
		*p = 0; //et on l'échange contre un 0 (fin de chaine)
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
