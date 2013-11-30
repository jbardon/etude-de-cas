#include "AlgoRecherche.h"

/*
* Fonctions locales
*/

static char *_str_sub (const char *s, unsigned int start, unsigned int end);
static void _supprimer_retour_ligne(char *chaine);

/*
* Définition de type de fonction pour la version 3
*/
typedef int (*FonctionComparer)(const void*, const void*); //fonction de comparaison utile pour la fonction de hashage
typedef void (*FonctionHash)(const void*); //fonction de hashage


//************************************************* ALGO VERSION 1 & 2 *************************************************//

/*
* Permet la création du dictionnaire pour les versions 1 & 2 des algos
* Paramètres :
* 	nomFichier, le fichier .txt contenant le dictionnaire
*/
GHashTable* chargerDicoV1_V2(char* nomFichier){
	
	GHashTable* DicoHash = g_hash_table_new_full(g_int_hash, g_int_equal, (GDestroyNotify)free, (GDestroyNotify)free);

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
				valhash = g_str_hash(line); //récupère la valeur de la fonction de hashage

				int* tmpval = malloc(sizeof(int));//création d'une nouvelle variable à chaque tour de boucle...
				*tmpval = valhash; //...qui contient la valeur de hashage
			
				char* tmp = g_strdup(line);
				g_hash_table_insert(DicoHash,/* GINT_TO_POINTER(valhash)*/tmpval, tmp); //Insertion dans le hashtable
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
* Permet la recherche de la plus longue sous-chaine dans une chaine en respectant l'ordre
* sous-chaine est contenue dans le dico
* Paramètres :
* 	table, hashtable contenant le dico
*	chaine, les lettres de départ
*/
char* version1(GHashTable* table, char* chaine)
{
	char* chaine_retourV1 = NULL; //variable qui contient le chaine à retourner
	char* substring; //variable qui contiendra chaque sous-chaine
	int taille=0; //contiendra la taille de la plus longue chaine

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
					chaine_retourV1=substring; //la chaine de retour est cette nouvelle chaine
				}
			}
		}
	}
	return chaine_retourV1;
}

/*
* Permet la recherche de la plus longue sous-chaine extraite dans une chaine en respectant l'ordre
* sous-chaine est contenue dans le dico
* Paramètres :
* 	table, hashtable contenant le dico
*	active, la chaine qui contiendra toutes les sous-chaine d'une chaine de caractère
*	rest, la chaine qui contient la chaine de départ
*/
char* version2_2(GHashTable* table, char* active, char* rest)
{
	if(strlen(rest)==0) //si la longueur de rest est 0
	{
		if(active == NULL) //si active vaut NULL
		{
		    return NULL; //on renvoie NULL
		}

		static char* chaine_retourV2 = ""; //chaine de retour de la fonction (variable static du à la récursivité)
		const int h = g_str_hash(active); //on recupère la valeur de hash de la sous-chaine ...
			
		if(g_hash_table_contains(table, &h)) //... on regarde si cette valeur est dans la hashtable
		{
			if(strlen(active) > strlen(chaine_retourV2)) //si la taille est plus grande que celui trouver avant
			{
				chaine_retourV2=active; //la chaine de retour est cette nouvelle chaine
		    	} 
		}
		return chaine_retourV2;
	}
	else
	{
		char* sub1 = _str_sub(rest,1,strlen(rest)); //on supprime le premier caractère de rest
		char* hash = calloc(strlen(active)+2,sizeof(char)); //on réserve un espace de la taille de active +2
		strcpy(hash,active); //on copie active dans hash

		char tab[2] = {rest[0],'\0'};
		strcat(hash, tab); //on insère le premier élément et \0 à la fin de hash
	    	version2_2(table,hash,sub1); //récursivité
	    	return version2_2(table,active,sub1); //récursivité
	}
}

/*
* Permet de cacher la version avec 3 paramètres où le deuxième est une chaine vide
*/
char* version2(GHashTable* table, char* chaine)
{
	return version2_2(table, "", chaine);
}

//*************************************************** ALGO VERSION 3 ***************************************************//

/*
* Permet la comparaison de deux caractères (ordre alphabétique)
* Paramètres :
* 	car1 et car2, les caractères à comparer
*/
int compare(const char* car1, const char* car2)
{
	return *car1-*car2;
}

/*
* Fonction de hashage pour la hashtable de la version 3
* Principe : mettre les lettres d'un mot dans l'ordre alphabétique
* Paramètres :
* 	chaine, la chaine de caractère à hacher
*/
char* hashage(char* chaine)
{
	char* hash = calloc(strlen(chaine)+1,sizeof(char)); //réserve un espace mémoire de la même dimension que chaine
	strcpy(hash,chaine); //copie de chaine dans hash
	qsort(hash, strlen(hash), sizeof(char), (FonctionComparer)compare); //range les lettres de hash dans l'ordre alphabétique
	return hash;
}

/*
* Permet l'insertion dans la table de hashage
* Paramètres :
* 	table, la table de hashage
*	data, la donnée à insérer
*	key, la clé de la donnée
*/
void insert(GHashTable* table, char* data, char* key)
{
	GSList* anag = (GSList*)g_hash_table_lookup(table, key);	
	anag = g_slist_append(anag, data);
	g_hash_table_replace(table, key, anag);
}

/*
* Permet la création du dictionnaire pour la version 3
* Paramètres :
* 	nomFichier, le fichier .txt contenant le dictionnaire
*/
GHashTable* chargerDicoV3(char* nomFichier)
{
	
	GHashTable* DicoHash = g_hash_table_new(g_str_hash, g_str_equal);

	if(DicoHash){
		char* valhash=NULL; //valeur de la fonction de hashage
	
		FILE* fichier = NULL; //definition de la variable fichier
		fichier = fopen(nomFichier,"r"); //ouverture du fichier dico.txt en mode r (lecture).

		//test pour savoir si le fichier est bien ouvert
		if (fichier != NULL)
		{
			char line[128];
			while(fgets(line, sizeof line, fichier)!=NULL) //lire une ligne et on la met dans line
			{
				_supprimer_retour_ligne(line); //suppression du retour à la ligne
				valhash = hashage(line); //récupère la valeur de la fonction de hashage
				char* tmp = g_strdup(line); //duplique line
				insert(DicoHash, tmp, valhash); //Insertion dans le hashtable
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

char* version3_2(GHashTable* table, char* active, char* rest)
{
	if(strlen(rest)==0) //si la longueur de rest est 0
	{
		if(active == NULL) //si active vaut NULL
		{
		    return NULL; //on renvoie NULL
		}

		char* anagramme = RechercheAnagramme(active,table); //on cherche un anagramme de active
		static char* chaine_retourV3 = "";

		if(anagramme) //s'il y a un anagramme
		{
		    if(strlen(anagramme) > strlen(chaine_retourV3)) //si la taille de l'anagramme est plus grande que celui précédent
		    {
			chaine_retourV3=anagramme; //la chaine de retour est ce nouveau anagramme
		    } 
		}
		return chaine_retourV3;
	}
	else
	{
		char* sub1 = _str_sub(rest,1,strlen(rest)); //on supprime le premier caractère de rest
		char* hash = calloc(strlen(active)+2,sizeof(char)); //on réserve un espace de la taille de active +2
		strcpy(hash,active); //on copie active dans hash

		char tab[2] = {rest[0],'\0'};
		strcat(hash, tab); //on insère le premier élément et \0 à la fin de hash

	    	version3_2(table,hash,sub1); //récursivité
	    	return version3_2(table,active,sub1); //récursivité
	}
}

/*
* Permet de cacher la version avec 3 paramètres où le deuxième est une chaine vide
*/
char* version3(GHashTable* table, char* chaine)
{
	return version3_2(table, "", chaine);
}

/*
* Permet la recherche de tous les anagrammes d'un mot et en renvoie un au hasard
* Paramètres :
* 	table, hashtable contenant le dico
*	chaine, le mot
*/

char* RechercheAnagramme(char *chaine, GHashTable* table)
{
	// Recherche une clé dans la table et renvoie un pointeur sur la donnée (une liste ici)
	char* hash=hashage(chaine);
	void* trouve = g_hash_table_lookup(table, hash);
	if(trouve) //s'il y a des anagrammes
	{
		GSList* anag = (GSList*)trouve; //liste contenant les anagrammes
		int lenght = g_list_length((GList*)anag); //récupère la longueur de la liste des anagrammes (nb anagrammes)

		srand(time(NULL)); //initialise le tirage aléatoire
		int aleatoire = rand()%lenght; //tirage aléatoire d'un nombre entre 0 et length
	
		return (char*)g_list_nth_data((GList*)anag,aleatoire); //retourne un des anagrammes du mot
		/*(char*)anag->data;*/
	}	
	else //s'il n'y a pas d'anagramme
	{	
		return NULL;
	}
}



//************************************************** FONCTIONS STATIC **************************************************//

/*
* Permet d'extraire la chaine de caractère de la chaine s qui est comprise entre les positions start et end.
* Si start==end, renvoie le caractère à la position de start et end.
* Paramètres :
* 	s, la chaine de départ dont on doit extraire
*	start, la position de départ de la sous chaine
* 	end, la position de fin de la sous chaine
*/
static char *_str_sub (const char *s, unsigned int start, unsigned int end)
{
	char *new_s = NULL; //initialisation de la chaine que l'on retorunera
	//si la chaine que l'on passe n'est pas NULL et que les positions sont OK
	if (s != NULL && start <= end)
	{
		new_s = malloc (sizeof(*new_s)*(end-start+2)); //allocation de la chaine que l'on retournera
		if (new_s != NULL)
		{
			int i; //initialise i pour la boucle for
			for(i=start;i<=end;i++)
			{
				new_s[i-start] = s[i]; //on remplit la chaine que l'on renverra
			}
			new_s[i-start] = '\0';//on rajoute le \0 de fin de chaine
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






