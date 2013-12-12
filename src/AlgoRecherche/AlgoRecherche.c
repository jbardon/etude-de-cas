#include "AlgoRecherche.h"

//**********************************************************************************************************************//
//******************************************* VARIABLES & METHODES STATIQUES *******************************************//
//**********************************************************************************************************************//

/**
* @var char* chaine_retourV2
* @brief Chaine de caractère qui sera renvoyé dans la version 2
* Initialisation: @see char* version2(GHashTable* table, char* chaine)
*/
static char* chaine_retourV2 = NULL;

/**
* @var char* chaine_retourV3
* @brief Chaine de caractère qui sera renvoyé dans la version 3
* Initialisation: @see char* version3(GHashTable* table, char* chaine)
*/
static char* chaine_retourV3 = NULL; 


/*
* Fonctions locales
*/
static char* _str_sub (const char *s, unsigned int start, unsigned int end);
static void _supprimer_retour_ligne(char *chaine);

/*
* Définition de type de fonction pour la version 3
*/
typedef int (*FonctionComparer)(const void*, const void*); //fonction de comparaison utile pour la fonction de hashage
typedef void (*FonctionHash)(const void*); //fonction de hashage


//**********************************************************************************************************************//
//************************************************* ALGO VERSION 1 & 2 *************************************************//
//**********************************************************************************************************************//

/**
* @fn GHashTable* chargerDicoV1_V2(char* nomFichier)
* @brief Fonction de création du dictionnaire pour les versions 1 & 2 des algos
* @param nomFichier La chaine de caractère contenant le nom du fichier.txt du dico
* @return La HashTable contenant le dictionnaire
*/
GHashTable* chargerDicoV1_V2(char* nomFichier)
{
	GHashTable* DicoHash = g_hash_table_new_full(g_int_hash, g_int_equal, (GDestroyNotify)free, (GDestroyNotify)free);
	if(DicoHash)
	{
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

/**
* @fn char* version1(GHashTable* table, char* chaine)
* @brief Permet la recherche de la plus longue sous-chaine, contenue dans le dico, d'une chaine en respectant l'ordre (Algo 1)
* @param table Le dictionnaire pour la recherche
* @param chaine La chaine qui contient les caractère de base
* @return La sous-chaine la plus longue contenue dans le dico
*/
char* version1(GHashTable* table, char* chaine)
{
	char* chaine_retourV1 = ""; //variable qui contient le chaine à retourner
	char* substring; //variable qui contiendra chaque sous-chaine
	int taille=0; //contiendra la taille de la plus longue chaine

	for(int i=0; i<strlen(chaine); i++) //on parcourt la chaine
	{
		for(int j=i; j<strlen(chaine); j++) //on parcourt la chaine à partir de i
		{
			substring = _str_sub(chaine,i,j); //on extrait la sous chaine entre i et j
			if(strlen(substring)>strlen(chaine_retourV1))
			{
				const unsigned int h = g_str_hash(substring); //on recupère la valeur de hash de la sous-chaine ...

				if(g_hash_table_contains(table, &h)) //... on regarde si cette valeur est dans la hashtable
				{
				    char* ret=g_hash_table_lookup(table, &h);
				    if(strcmp(ret,substring)==0)
				    {
						if(strlen(substring) > taille) //si la taille est plus grande que celui trouver avant
						{
							taille=strlen(substring); //la taille change
							chaine_retourV1=substring; //la chaine de retour est cette nouvelle chaine
						}
				    }
				}
			}
			if(strcmp(chaine_retourV1, substring))
			{
				free(substring);
			}
		}
	}
   	return chaine_retourV1;
}

/**
* @fn version2_2(GHashTable* table, char* active, char* rest)
* @brief Appeler lors de l'appel à la fonction version2(GHashTable* table, char* chaine)
* @param table Le dictionnaire pour la recherche
* @param active La chaine qui contiendra toutes les sous-chaine d'une chaine de caractère
* @param rest La chaine qui contient la chaine de départ
* @return La sous-chaine la plus longue contenue dans le dico
*/
void version2_2(GHashTable* table, char* active, char* rest)
{
    if(strlen(rest)==0) //si la longueur de rest est 0
    {
        if(active == NULL) //si active vaut NULL
        {
            return; //on renvoie NULL
        }

		if(strlen(active) > strlen(chaine_retourV2)) //si la taille est plus grande que celui trouver avant
        {
			const unsigned int h = g_str_hash(active); //on recupère la valeur de hash de la sous-chaine ...
				
			if(g_hash_table_contains(table, &h)) //... on regarde si cette valeur est dans la hashtable
			{
				char* ret=g_hash_table_lookup(table, &h);
				if(strcmp(ret,active)==0)
				{
					chaine_retourV2 = realloc(chaine_retourV2, (strlen(active) + 1) * sizeof(char));//la chaine de retour est cette nouvelle chaine
					strcpy(chaine_retourV2, active);
				}
			}
		}
    }
    else
    {
        char* sub1 = _str_sub(rest,1,strlen(rest)); //on supprime le premier caractère de rest
        char* hash = calloc(strlen(active)+2,sizeof(char)); //on réserve un espace de la taille de active +2
        strcpy(hash,active); //on copie active dans hash
 
        char tab[2] = {rest[0],'\0'};
        strcat(hash, tab); //on insère le premier élément et \0 à la fin de hash

		version2_2(table,hash,sub1); //récursivité
        version2_2(table,active,sub1); //récursivité
	
		free(hash);		
		free(sub1);
    }
}


/**
* @fn char* version2(GHashTable* table, char* chaine)
* @brief Permet la recherche de la plus longue sous-chaine extraite, contenue dans le dico, dans une chaine (Algo 2)
* Permet de cacher la version avec 3 paramètres dont le 2ème est une chaine vide au départ
* @param table Le dictionnaire pour la recherche
* @param chaine La chaine qui contient les caractère de base
* @return La sous-chaine extraite la plus longue contenue dans le dico
*/
char* version2(GHashTable* table, char* chaine)
{
	chaine_retourV2 = calloc(1, sizeof(char));
	strcpy(chaine_retourV2, "");
	version2_2(table, "", chaine);
	return chaine_retourV2;
}


//**********************************************************************************************************************//
//*************************************************** ALGO VERSION 3 ***************************************************//
//**********************************************************************************************************************//

/**
* @fn int compare(const char* car1, const char* car2)
* @brief Permet la comparaison de deux caractères (ordre alphabétique)
* @param car1 Le premier caractère à comparer
* @param car2 Le second
* @return 0 si égal, >0 si car2 est avant car1 dans l'alphabet, <0 si car2 est après car1 dans l'alphabet
*/
int compare(const char* car1, const char* car2)
{
	return *car1-*car2;
}

/**
* @fn char* hashage(char* chaine)
* @brief Fonction de hashage pour la hashtable de la version 3
* Principe : mettre les lettres d'un mot dans l'ordre alphabétique
* @param chaine Le mot à hacher
* @return La chaine dans l'ordre alphabétique (la clé du mot)
*/
char* hashage(char* chaine)
{
	char* hash = calloc(strlen(chaine)+1,sizeof(char)); //réserve un espace mémoire de la même dimension que chaine
	strcpy(hash,chaine); //copie de chaine dans hash
	qsort(hash, strlen(hash), sizeof(char), (FonctionComparer)compare); //range les lettres de hash dans l'ordre alphabétique
	return hash;
}

/**
* @fn void insert(GHashTable* table, char* data, char* key)
* @brief Permet l'insertion dans la table de hashage pour la version 3
* Principe : ranger dans des listes pour les mots qui auront la même clé
* @param table Le dictionnaire
* @param data La donnée à insérer
* @param key La clé de la donnée
* @return void
*/
void insert(GHashTable* table, char* data, char* key)
{
	GSList* anag = (GSList*)g_hash_table_lookup(table, key);	
	anag = g_slist_append(anag, data);
	g_hash_table_replace(table, key, anag);
}

/**
* @fn supprimerDicoV3(GHashTable* table)
* @brief Permet la suppression et la libération de l'espace mémoire du dico pour la version 3
* @param table Le dictionnaire
* @return void
*/
void supprimerDicoV3(GHashTable* table)
{
	GHashTableIter iter;
	GSList *list;
	char* key;

	g_hash_table_iter_init(&iter, table); //iterator pour afficher le hashtable
	while(g_hash_table_iter_next(&iter, (gpointer)&key, (gpointer)&list))
	{
		g_slist_free_full(list,(GDestroyNotify)free); 
	}

	g_hash_table_destroy(table);
}


/**
* @fn GHashTable* chargerDicoV3(char* nomFichier)
* @brief Fonction de création du dictionnaire pour la version 3 des algos
* @param nomFichier La chaine de caractère contenant le nom du fichier.txt du dico
* @return La HashTable contenant le dictionnaire
*/
GHashTable* chargerDicoV3(char* nomFichier)
{
	
	GHashTable* DicoHash = g_hash_table_new(g_str_hash, g_str_equal);

	if(DicoHash)
	{
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

/**
* @fn version3_2(GHashTable* table, char* active, char* rest)
* @brief Appeler lors de l'appel à la fonction version3(GHashTable* table, char* chaine)
* @param table Le dictionnaire pour la recherche
* @param active La chaine qui contiendra toutes les sous-chaine d'une chaine de caractère
* @param rest La chaine qui contient la chaine de départ
* @return L'anagramme le plus long contenu dans le dico
*/
void version3_2(GHashTable* table, char* active, char* rest)
{

	if(strlen(rest)==0) //si la longueur de rest est 0
	{
		if(active == NULL) //si active vaut NULL
		{
		    return; //on renvoie NULL
		}

		if(strlen(active)>strlen(chaine_retourV3))
		{
			char* anagramme = RechercheAnagramme(table,active); //on cherche un anagramme de active

			if(anagramme) //s'il y a un anagramme
			{
				if(strlen(anagramme) > strlen(chaine_retourV3)) //si la taille de l'anagramme est plus grande que celui précédent
		    	{
					chaine_retourV3=realloc(chaine_retourV3, (strlen(anagramme) + 1) * sizeof(char)); 
	      			strcpy(chaine_retourV3, anagramme);//la chaine de retour est ce nouveau anagramme
		    	} 
			}
		}
	}
	else
	{
		char* sub1 = _str_sub(rest,1,strlen(rest)); //on supprime le premier caractère de rest
		char* hash = calloc(strlen(active)+2,sizeof(char)); //on réserve un espace de la taille de active +2
		strcpy(hash,active); //on copie active dans hash

		char tab[2] = {rest[0],'\0'};
		strcat(hash, tab); //on insère le premier élément et \0 à la fin de hash

	    version3_2(table,hash,sub1); //récursivité
		version3_2(table,active,sub1); //récursivité

		free(hash);
		free(sub1);
	}
}

/**
* @fn char* version3(GHashTable* table, char* chaine)
* @brief Permet la recherche du plus grand anagramme, contenue dans le dico, dans une chaine (Algo 3)
* Permet de cacher la version avec 3 paramètres dont le 2ème est une chaine vide au départ
* @param table Le dictionnaire pour la recherche
* @param chaine La chaine qui contient les caractère de base
* @return L'anagramme le plus long contenue dans le dico
*/
char* version3(GHashTable* table, char* chaine)
{
	chaine_retourV3 = calloc(1, sizeof(char));
	strcpy(chaine_retourV3, "");
	version3_2(table, "", chaine);
	return chaine_retourV3;
}

/**
* @fn char* RechercheAnagramme(GHashTable* table, char *chaine)
* @brief Permet la recherche de tous les anagrammes d'un mot
* @param table Le dictionnaire pour la recherche
* @param chaine Le mot de départ
* @return Un des anagrammes du mot au hasard (le mot lui même si aucun anagramme)
*/
char* RechercheAnagramme(GHashTable* table, char *chaine)
{
	// Recherche une clé dans la table et renvoie un pointeur sur la donnée (une liste ici)
	char* hash=hashage(chaine);
	void* trouve = g_hash_table_lookup(table, hash);
	free(hash);

	if(trouve) //s'il y a des anagrammes
	{
		GSList* anag = (GSList*)trouve; //liste contenant les anagrammes
		int lenght = g_list_length((GList*)anag); //récupère la longueur de la liste des anagrammes (nb anagrammes)

		srand(time(NULL)); //initialise le tirage aléatoire
		int aleatoire = rand()%lenght; //tirage aléatoire d'un nombre entre 0 et length
	
		return (char*)g_list_nth_data((GList*)anag,aleatoire); //retourne un des anagrammes du mot
	}	
	else //s'il n'y a pas d'anagramme
	{	
		return NULL;
	}
}


//**********************************************************************************************************************//
//************************************************** FONCTIONS STATIC **************************************************//
//**********************************************************************************************************************//

/**
* @fn static char* _str_sub (const char *s, unsigned int start, unsigned int end)
* @brief Permet d'extraire la sous chaine de la chaine s qui est comprise entre les positions start et end.
* Si start==end, renvoie le caractère à la position de start et end.
* @param s La chaine entière
* @param start L'indice de départ
* @param end L'indice de fin
* @return La chaine de caractère extraite entre les positions start et end de s
*/
static char* _str_sub (const char *s, unsigned int start, unsigned int end)
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

/**
* @fn static void _supprimer_retour_ligne(char *chaine)
* @brief Permet la suppression du retour chariot d'une chaine de caractere
* @param chaine La chaine de caractère
* @return void
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






