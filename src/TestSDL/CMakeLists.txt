cmake_minimum_required(VERSION 2.8)

add_executable(testTranslation testTranslation.c)
target_link_libraries(testTranslation SDLmain SDL SDL_gfx)

add_executable(testRotation testRotation.c)
target_link_libraries(testRotation SDLmain SDL SDL_gfx)

add_executable(testRotationTranslation testRotationTranslation.c)
target_link_libraries(testRotationTranslation SDLmain SDL SDL_gfx)

add_test(testTranslation ./testTranslation)
add_test(testRotation ./testRotation)
add_test(testRotationTranslation ./testRotationTranslation)
