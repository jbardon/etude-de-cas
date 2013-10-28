cmake_minimum_required(VERSION 2.8)

add_executable(projet projet.c)
target_link_libraries(projet GestionEnv Balle SDLmain SDL chipmunk SDL_gfx)

add_executable(testGaucheDroite testGaucheDroite.c)
target_link_libraries(testGaucheDroite GestionEnv Balle SDLmain SDL chipmunk SDL_gfx)

add_executable(testDroiteGauche testDroiteGauche.c)
target_link_libraries(testDroiteGauche GestionEnv Balle SDLmain SDL chipmunk SDL_gfx)

add_test(projet ./projet)
add_test(testGaucheDroite ./testGaucheDroite)
add_test(testDroiteGauche ./testDroiteGauche)
