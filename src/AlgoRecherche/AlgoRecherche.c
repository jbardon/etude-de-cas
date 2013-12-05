#include "AlgoRecherche.h"

/*
* Variables statiques
*/

static char* chaine_retourV2 = NULL; //chaine de retour de la fonction (variable static du � la r�cursivit�)
static char* chaine_retourV3 = NULL; 


/*
* Fonctions locales
*/

static char *_str_sub (const char *s, unsigned int start, unsigned int end);
static void _supprimer_retour_ligne(char *chaine);

/*
* D�finition de type de fonction pour la version 3
*/
typedef int (*FonctionComparer)(const void*, const void*); //fonction de comparaison utile pour la fonction de hashage
typedef void (*FonctionHash)(const void*); //fonction de hashage


//************************************************* ALGO VERSION 1 & 2 *************************************************//

/*
* Permet la cr�ation du dictionnaire pour les versions 1 & 2 des algos
* Param�tres :
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
				_supprimer_retour_ligne(line); //suppression du retour � la ligne
				valhash = g_str_hash(line); //r�cup�re la valeur de la fonction de hashage

				int* tmpval = malloc(sizeof(int));//cr�ation d'une nouvelle variable � chaque tour de boucle...
				*tmpval = valhash; //...qui contient la valeur de hashage
			
				char* tmp = g_strdup(line);
				g_hash_table_insert(DicoHash,/* GINT_TO_POINTER(valhash)*/tmpval, tmp); //Insertion dans le hashtable
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
* Permet la recherche de la plus longue sous-chaine dans une chaine en respectant l'ordre
* sous-chaine est contenue dans le dico
* Param�tres :
* 	table, hashtable contenant le dico
*	chaine, les lettres de d�part
*/
char* version1(GHashTable* table, char* chaine)
{
    char* chaine_retourV1 = ""; //variable qui contient le chaine � retourner
    char* substring; //variable qui contiendra chaque sous-chaine
    int taille=0; //contiendra la taille de la plus longue chaine
 
    for(int i=0; i<strlen(chaine); i++) //on parcourt la chaine
    {
        for(int j=i; j<strlen(chaine); j++) //on parcourt la chaine � partir de i
        {
            substring = _str_sub(chaine,i,j); //on extrait la sous chaine entre i et j
			if(strlen(substring)>strlen(chaine_retourV1))
			{
		        const unsigned int h = g_str_hash(substring); //on recup�re la valeur de hash de la sous-chaine ...
		        
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
			if(strcmp(chaine_retourV1, substring)){
				free(substring);
			}
        }
    }
    return chaine_retourV1;
}


/*
* Permet la recherche de la plus longue sous-chaine extraite dans une chaine en respectant l'ordre
* sous-chaine est contenue dans le dico
* Param�tres :
* 	table, hashtable contenant le dico
*	active, la chaine qui contiendra toutes les sous-chaine d'une chaine de caract�re
*	rest, la chaine qui contient la chaine de d�part
*/
void version2_2(GHashTable* table, char* active, char* rest)
{
    if(strlen(rest)==0) //si la longueur de rest est 0
    {
        if(active == NULL) //si active vaut NULL
        {
            return; //on renvoie NULL
        }
 
        const unsigned int h = g_str_hash(active); //on recup�re la valeur de hash de la sous-chaine ...
            
        if(g_hash_table_contains(table, &h)) //... on regarde si cette valeur est dans la hashtable
        {
            char* ret=g_hash_table_lookup(table, &h);
            if(strcmp(ret,active)==0)
            {
                if(strlen(active) > strlen(chaine_retourV2)) //si la taille est plus grande que celui trouver avant
                {
                    chaine_retourV2 = realloc(chaine_retourV2, (strlen(active) + 1) * sizeof(char));//la chaine de retour est cette nouvelle chaine
					strcpy(chaine_retourV2, active);
                }
            }
        }
    }
    else
    {
        char* sub1 = _str_sub(rest,1,strlen(rest)); //on supprime le premier caract�re de rest
        char* hash = calloc(strlen(active)+2,sizeof(char)); //on r�serve un espace de la taille de active +2
        strcpy(hash,active); //on copie active dans hash
 
        char tab[2] = {rest[0],'\0'};
        strcat(hash, tab); //on ins�re le premier �l�ment et \0 � la fin de hash

		version2_2(table,hash,sub1); //r�cursivit�
        version2_2(table,active,sub1); //r�cursivit�
	
		free(hash);		
		free(sub1);
    }
}


/*
* Permet de cacher la version avec 3 param�tres o� le deuxi�me est une chaine vide
*/
char* version2(GHashTable* table, char* chaine)
{
	chaine_retourV2 = calloc(1, sizeof(char));
	strcpy(chaine_retourV2, "");
	version2_2(table, "", chaine);
	return chaine_retourV2;
}

//*************************************************** ALGO VERSION 3 ***************************************************//

/*
* Permet la comparaison de deux caract�res (ordre alphab�tique)
* Param�tres :
* 	car1 et car2, les caract�res � comparer
*/
int compare(const char* car1, const char* car2)
{
	return *car1-*car2;
}

/*
* Fonction de hashage pour la hashtable de la version 3
* Principe : mettre les lettres d'un mot dans l'ordre alphab�tique
* Param�tres :
* 	chaine, la chaine de caract�re � hacher
*/
char* hashage(char* chaine)
{
	char* hash = calloc(strlen(chaine)+1,sizeof(char)); //r�serve un espace m�moire de la m�me dimension que chaine
	strcpy(hash,chaine); //copie de chaine dans hash
	qsort(hash, strlen(hash), sizeof(char), (FonctionComparer)compare); //range les lettres de hash dans l'ordre alphab�tique
	return hash;
}

/*
* Permet l'insertion dans la table de hashage
* Param�tres :
* 	table, la table de hashage
*	data, la donn�e � ins�rer
*	key, la cl� de la donn�e
*/
void insert(GHashTable* table, char* data, char* key)
{
	GSList* anag = (GSList*)g_hash_table_lookup(table, key);	
	anag = g_slist_append(anag, data);
	g_hash_table_replace(table, key, anag);
}


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


/*
* Permet la cr�ation du dictionnaire pour la version 3
* Param�tres :
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
				_supprimer_retour_ligne(line); //suppression du retour � la ligne
				valhash = hashage(line); //r�cup�re la valeur de la fonction de hashage
				char* tmp = g_strdup(line); //duplique line
				insert(DicoHash, tmp, valhash); //Insertion dans le hashtable
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

void version3_2(GHashTable* table, char* active, char* rest)
{
	if(strlen(rest)==0) //si la longueur de rest est 0
	{
		if(active == NULL) //si active vaut NULL
		{
		    return; //on renvoie NULL
		}

		char* anagramme = RechercheAnagramme(active,table); //on cherche un anagramme de active

		if(anagramme) //s'il y a un anagramme
		{
		    if(strlen(anagramme) > strlen(chaine_retourV3)) //si la taille de l'anagramme est plus grande que celui pr�c�dent
		    {
				chaine_retourV3=realloc(chaine_retourV3, (strlen(anagramme) + 1) * sizeof(char)); //la chaine de retour est ce nouveau anagramme
              	strcpy(chaine_retourV3, anagramme);
		    } 
		}
	}
	else
	{
		char* sub1 = _str_sub(rest,1,strlen(rest)); //on supprime le premier caract�re de rest
		char* hash = calloc(strlen(active)+2,sizeof(char)); //on r�serve un espace de la taille de active +2
		strcpy(hash,active); //on copie active dans hash

		char tab[2] = {rest[0],'\0'};
		strcat(hash, tab); //on ins�re le premier �l�ment et \0 � la fin de hash

	    version3_2(table,hash,sub1); //r�cursivit�
		version3_2(table,active,sub1); //r�cursivit�

		free(hash);
		free(sub1);
	}
}

/*
* Permet de cacher la version avec 3 param�tres o� le deuxi�me est une chaine vide
*/
char* version3(GHashTable* table, char* chaine)
{
	chaine_retourV3 = calloc(1, sizeof(char));
	strcpy(chaine_retourV3, "");
	version3_2(table, "", chaine);
	return chaine_retourV3;
}

/*
* Permet la recherche de tous les anagrammes d'un mot et en renvoie un au hasard
* Param�tres :
* 	table, hashtable contenant le dico
*	chaine, le mot
*/

char* RechercheAnagramme(char *chaine, GHashTable* table)
{
	// Recherche une cl� dans la table et renvoie un pointeur sur la donn�e (une liste ici)
	char* hash=hashage(chaine);
	void* trouve = g_hash_table_lookup(table, hash);
	free(hash);

	if(trouve) //s'il y a des anagrammes
	{
		GSList* anag = (GSList*)trouve; //liste contenant les anagrammes
		int lenght = g_list_length((GList*)anag); //r�cup�re la longueur de la liste des anagrammes (nb anagrammes)

		srand(time(NULL)); //initialise le tirage al�atoire
		int aleatoire = rand()%lenght; //tirage al�atoire d'un nombre entre 0 et length
	
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
* Permet d'extraire la chaine de caract�re de la chaine s qui est comprise entre les positions start et end.
* Si start==end, renvoie le caract�re � la position de start et end.
* Param�tres :
* 	s, la chaine de d�part dont on doit extraire
*	start, la position de d�part de la sous chaine
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






