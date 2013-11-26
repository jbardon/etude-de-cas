cmake_minimum_required(VERSION 2.8)

configure_file(arial.ttf arial.ttf COPYONLY)
configure_file(dicofinal.txt dicofinal.txt COPYONLY)

add_library(GestionEnvShared SHARED GestionEnv.c)
add_library(GestionEnvStatic STATIC GestionEnv.c)

set_target_properties(
	GestionEnvShared GestionEnvStatic
	PROPERTIES OUTPUT_NAME GestionEnv
)

add_executable(testPanier testPanier.c)
target_link_libraries(testPanier GestionEnv Balle SDL SDLmain SDL_gfx SDL_image SDL_ttf chipmunk glib-2.0)

add_executable(testBalles testBalles.c)
target_link_libraries(testBalles GestionEnv Balle SDL SDLmain SDL_gfx SDL_image SDL_ttf chipmunk glib-2.0)

add_executable(testLigne testLigne.c)
target_link_libraries(testLigne GestionEnv Balle AlgoRecherche SDL SDLmain SDL_gfx SDL_image SDL_ttf chipmunk glib-2.0)

add_executable(testCompareBalle testCompareBalle.c)
target_link_libraries(testCompareBalle GestionEnv Balle SDL SDLmain SDL_gfx SDL_image SDL_ttf chipmunk glib-2.0)

add_test(testPanier ./testPanier)
add_test(testBalles ./testBalles)
add_test(testLigne ./testLigne)
add_test(testCompareBalle ./testCompareBalle)
