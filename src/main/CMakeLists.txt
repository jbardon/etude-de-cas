cmake_minimum_required(VERSION 2.8)

add_executable(projet projet.c)
target_link_libraries(projet GestionEnv Balle SDLmain SDL chipmunk SDL_gfx)

add_test(projet ./projet)
