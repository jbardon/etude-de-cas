cmake_minimum_required(VERSION 2.8)

configure_file(arial.ttf arial.ttf COPYONLY)

add_library(MenuSDLShared SHARED MenuSDL.c)
add_library(MenuSDLStatic STATIC MenuSDL.c)

set_target_properties(
	MenuSDLShared MenuSDLStatic
	PROPERTIES OUTPUT_NAME MenuSDL
)

add_executable(testMenuSDL testMenuSDL.c)
target_link_libraries(testMenuSDL MenuSDL SDL SDLmain SDL_ttf)

add_test(testMenuSDL ./testMenuSDL)
