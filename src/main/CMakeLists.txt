cmake_minimum_required(VERSION 2.8)

add_executable(projet projet.c)
target_link_libraries(projet GestionEnv Balle AlgoRecherche SDLmain SDL chipmunk SDL_gfx SDL_image SDL_ttf glib-2.0)

add_test(testFichier ./testFichier)
