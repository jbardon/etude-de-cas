cmake_minimum_required(VERSION 2.8)

configure_file(arial.ttf arial.ttf COPYONLY)

add_executable(testGaucheDroite testGaucheDroite.c)
target_link_libraries(testGaucheDroite GestionEnv Balle SDLmain SDL chipmunk SDL_gfx SDL_image SDL_ttf glib-2.0)

add_executable(testDroiteGauche testDroiteGauche.c)
target_link_libraries(testDroiteGauche GestionEnv Balle SDLmain SDL chipmunk SDL_gfx SDL_image SDL_ttf glib-2.0)

add_executable(testCollision testCollision.c)
target_link_libraries(testCollision GestionEnv Balle SDLmain SDL chipmunk SDL_gfx SDL_image SDL_ttf glib-2.0)

add_test(testGaucheDroite ./testGaucheDroite)
add_test(testDroiteGauche ./testDroiteGauche)
add_test(testCollision ./testCollision)
