cmake_minimum_required(VERSION 2.8)

configure_file(arial.ttf arial.ttf COPYONLY)
configure_file(dicofinal.txt dicofinal.txt COPYONLY)

add_executable(projet projet.c)
target_link_libraries(projet GestionEnv Aleatoire Balle AlgoRecherche MenuSDL SDLmain SDL chipmunk SDL_gfx SDL_ttf glib-2.0)

add_test(fichier ./fichier)
