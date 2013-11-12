#!/bin/bash

# Ce script permet d'afficher les proportions de chaques
# lettre de l'alphabet (minuscules) dans un fichier donné
#
# Utilisation: ./occurLettre cheminVersFichier

export LC_NUMERIC=C #printf

fichier=$1

function occurLettre(){
	nbOccur=$(cat $fichier | tr -dc $1 | wc -m)
	proba=$(echo "($nbOccur/$nbLettres)*100" | bc -l)
	sommeProba=$(echo "$sommeProba+$proba" | bc -l)

#	printf "%2.2f," $sommeProba
	echo "Nombre d'occurences de la lettre '"$lettre"': "$nbOccur	
	echo "Proportion: "$(printf "%2.2f" $proba)"% (lettre) "$(printf "%2.2f" $sommeProba)"% (somme)"
}

nbLettres=$(cat $fichier | tr -dc '[a-z]' | wc -m)
echo "Nombre de lettres dans le dictionnaire: "$nbLettres && echo

alphabet=$(echo {a..z})
sommeProba=0
for lettre in $alphabet
do
	occurLettre $lettre
	echo
done
