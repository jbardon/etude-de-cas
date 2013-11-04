cmake_minimum_required(VERSION 2.8)

add_executable(testGaucheDroite testGaucheDroite.c)
target_link_libraries(testGaucheDroite GestionEnv Balle SDLmain SDL chipmunk SDL_gfx SDL_image SDL_ttf)

add_executable(testDroiteGauche testDroiteGauche.c)
target_link_libraries(testDroiteGauche GestionEnv Balle SDLmain SDL chipmunk SDL_gfx SDL_image SDL_ttf)

add_test(testGaucheDroite ./testGaucheDroite)
add_test(testDroiteGauche ./testDroiteGauche)
