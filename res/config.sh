#!/bin/bash

# Ce script doit être executé à partir du répertoire /res du projet
# Il prend en paramètre le chemin vers le répertoire /src
#
# Dans le cas normal voici les commandes à effectuer à partir du répertoire de projet :
#	- cd res
#	- chmod u+x config.sh
#	- ./config.sh ../src
#
#	soit: cd res && chmod u+x config.sh && ./config.sh ../src
#
init_dir=$(pwd); ## Pour revenir dans le répertoire courant

src_dir="$1";
res_dir=$(pwd);

cd $src_dir;

# Modifie la ligne: set(RES_PATH "/home/jeremy/Documents/projet/etude-de-cas/res/lib")
# du fichier src/CMakeLists.txt
#sed -r -i "s:^(set\(RES_PATH \")[^\"]*:\1"$res_dir"/lib:g" CMakeLists.txt;

# Modifie la ligne: #define RES_PATH "/home/jeremy/Documents/projet/etude-de-cas/res/fichiers"
#sed -r -i "s:^(#define RES_PATH \")[^\"]*:\1"$res_dir"/fichiers:g" main/config.h;

# Indications pour modifier src/CMakeLists.txt
echo -e "A ajouter dans le fichier src/CMakeLists\n"

echo "Configuration chipmunk"
echo -e "\tinclude_directories: "$(locate chipmunk.h)
echo -e "\tlink_directories: "$(locate libchipmunk.a)

echo "Configuration SDL"
echo -e "\tinclude_directories: "$(locate SDL.h)
echo -e "\tlink_directories: "$(locate libSDL.a)

echo "Configuration glib"
echo -e "\tinclude_directories: "$(locate /glib.h)" et "$(locate glibconfig.h)
echo -e "\tlink_directories: "$(locate libglib-2.0.a)
echo

cd $init_dir; ## Pour revenir dans le répertoire courant
