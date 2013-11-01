cmake_minimum_required(VERSION 2.8)

add_library(GestionEnvShared SHARED GestionEnv.c)
add_library(GestionEnvStatic STATIC GestionEnv.c)

set_target_properties(
	GestionEnvShared GestionEnvStatic
	PROPERTIES OUTPUT_NAME GestionEnv
)

add_executable(testPanier testPanier.c)
target_link_libraries(testPanier GestionEnv Balle SDLmain SDL SDL_gfx chipmunk)

add_executable(testBalles testBalles.c)
target_link_libraries(testBalles GestionEnv Balle SDL SDLmain SDL_gfx chipmunk)

add_test(testPanier ./testPanier)
add_test(testBalles ./testBalles)
