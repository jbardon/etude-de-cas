cmake_minimum_required(VERSION 2.8)

configure_file(arial.ttf arial.ttf COPYONLY)

add_executable(testTranslation testTranslation.c)
target_link_libraries(testTranslation SDLmain SDL SDL_gfx)

add_executable(testRotation testRotation.c)
target_link_libraries(testRotation SDLmain SDL SDL_gfx)

add_executable(testRotationTranslation testRotationTranslation.c)
target_link_libraries(testRotationTranslation SDLmain SDL SDL_gfx)

add_executable(testCaractere testCaractere.c)
target_link_libraries(testCaractere SDLmain SDL SDL_gfx SDL_ttf)

add_executable(testClic testClic.c)
target_link_libraries(testClic SDLmain SDL)

add_test(testTranslation ./testTranslation)
add_test(testRotation ./testRotation)
add_test(testRotationTranslation ./testRotationTranslation)
add_test(testCaractere ./testCaractere)
add_test(testClic ./testClic)
