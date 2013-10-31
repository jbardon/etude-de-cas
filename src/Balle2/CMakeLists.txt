cmake_minimum_required(VERSION 2.8)

add_library(BalleShared SHARED Balle.c)
add_library(BalleStatic STATIC Balle.c)

set_target_properties(
	BalleShared BalleStatic
	PROPERTIES OUTPUT_NAME Balle
)

add_executable(TestBalle TestBalle.c)
target_link_libraries(TestBalle Balle SDLmain SDL chipmunk SDL_gfx)

add_executable(TestSwapPixel TestSwapPixel.c)
target_link_libraries(TestSwapPixel SDLmain SDL)

add_test(TestBalle ./TestBalle)
add_test(TestSwapPixel ./TestSwapPixel)
