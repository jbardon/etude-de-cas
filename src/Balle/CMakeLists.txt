cmake_minimum_required(VERSION 2.8)

add_library(BalleShared SHARED Balle.c)
add_library(BalleStatic STATIC Balle.c)

set_target_properties(
	BalleShared BalleStatic
	PROPERTIES OUTPUT_NAME Balle
)

add_executable(testBalle testBalle.c)
target_link_libraries(testBalle Balle SDL SDLmain SDL_gfx chipmunk)

add_executable(testSwapPixel testSwapPixel.c)
target_link_libraries(testSwapPixel SDLmain SDL)

add_test(testBalle ./testBalle)
add_test(testSwapPixel ./testSwapPixel)
