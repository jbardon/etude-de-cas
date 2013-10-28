cmake_minimum_required(VERSION 2.8)

add_executable(testSDL_gfx testSDL_gfx.c)
target_link_libraries(testSDL_gfx SDLmain SDL SDL_gfx)

add_executable(testSDL_rotozoom testSDL_rotozoom.c)
target_link_libraries(testSDL_rotozoom SDLmain SDL SDL_gfx)

add_test(testSDL_gfx ./testSDL_gfx)
add_test(testSDL_rotozoom ./testSDL_rotozoom)
