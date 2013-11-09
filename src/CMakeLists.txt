cmake_minimum_required(VERSION 2.8)

project(EtudeDeCas C)

###

###

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "EtudeDeCas")

set(PGCD_VERSION_MAJOR 0)
set(PGCD_VERSION_MINOR 0)
set(PGCD_VERSION_PATCH 1)

add_custom_target(debug
	cmake ${CMAKE_CURRENT_SOURCE_DIR} -DCMAKE_BUILD_TYPE=Debug
	COMMAND make
	COMMENT "Building debug version..."
)
set(CMAKE_C_FLAGS_DEBUG "-g -Wall -pedantic -std=c99")

add_custom_target(release
	cmake ${CMAKE_CURRENT_SOURCE_DIR} -DCMAKE_BUILD_TYPE=Release
	COMMAND make
	COMMENT "Building release version..."
)
set(CMAKE_C_FLAGS_RELEASE "-O3 -Wall -pedantic -std=c99 -DNDEBUG")

set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_PACKAGE_VENDOR "Augereau-Bardon-Hay")
set(CPACK_PACKAGE_VERSION_MAJOR "${PGCD_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PGCD_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PGCD_VERSION_PATCH}")
set(CPACK_SOURCE_IGNORE_FILES "~$;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)

enable_testing()

include_directories(
#	"/usr/include/glib-2.0"
#	"/usr/include/glib-2.0/glib"
#	"/usr/lib/i386-linux-gnu/glib-2.0/include/"
#	"/usr/include/chipmunk"
#	"/usr/include/SDL"
	"${CMAKE_CURRENT_SOURCE_DIR}/../res/chipmunk/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../res/sdl/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../res/glib/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/Balle"
	"${CMAKE_CURRENT_SOURCE_DIR}/GestionEnv"
	"${CMAKE_CURRENT_SOURCE_DIR}/main"
	"${CMAKE_CURRENT_SOURCE_DIR}/TestSDL"
	"${CMAKE_CURRENT_SOURCE_DIR}/TestChipmunk"
	"${CMAKE_CURRENT_SOURCE_DIR}/AlgoRecherche"
)

link_directories(
#	"/usr/lib/i386-linux-gnu"
	"${CMAKE_CURRENT_BINARY_DIR}/../res/chipmunk/lib"
	"${CMAKE_CURRENT_BINARY_DIR}/../res/sdl/lib"
	"${CMAKE_CURRENT_BINARY_DIR}/../res/glib/lib"
	"${CMAKE_CURRENT_BINARY_DIR}/Balle"
	"${CMAKE_CURRENT_BINARY_DIR}/GestionEnv"
	"${CMAKE_CURRENT_BINARY_DIR}/main"
	"${CMAKE_CURRENT_BINARY_DIR}/TestSDL"	
	"${CMAKE_CURRENT_BINARY_DIR}/TestChipmunk"
	"${CMAKE_CURRENT_BINARY_DIR}/AlgoRecherche"
)

add_subdirectory(Balle)
add_subdirectory(GestionEnv)
add_subdirectory(main)
add_subdirectory(TestChipmunk)
add_subdirectory(TestSDL)
add_subdirectory(AlgoRecherche)

