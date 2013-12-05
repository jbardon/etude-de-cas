cmake_minimum_required(VERSION 2.8)

project(EtudeDeCas C)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "EtudeDeCas")

set(PGCD_VERSION_MAJOR 0)
set(PGCD_VERSION_MINOR 0)
set(PGCD_VERSION_PATCH 1)

add_custom_target(debug
	cmake ${CMAKE_CURRENT_SOURCE_DIR} -DCMAKE_BUILD_TYPE=Debug
	COMMAND make
	COMMENT "Building debug version..."
)
set(CMAKE_C_FLAGS_DEBUG "-g -pg -Wall -pedantic -std=c99")

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

#set(RES_PATH "/home/jeremy/Documents/projet/etude-de-cas/res/lib")

include_directories(
	"/usr/include/chipmunk"
	"/usr/local/include/chipmunk" # chipmunk en local
	"/usr/include/SDL"

	"/usr/include/glib-2.0/"
	"/usr/lib/i386-linux-gnu/glib-2.0/include/" # glibconfig.h (OS 32 bits)
	"/usr/lib/x86_64-linux-gnu/glib-2.0/include/" # glibconfig.h (OS 64 bits)

	"${CMAKE_CURRENT_SOURCE_DIR}/AlgoRecherche"
	"${CMAKE_CURRENT_SOURCE_DIR}/Aleatoire"
	"${CMAKE_CURRENT_SOURCE_DIR}/Balle"
	"${CMAKE_CURRENT_SOURCE_DIR}/GestionEnv"
	"${CMAKE_CURRENT_SOURCE_DIR}/MenuSDL"

	"${CMAKE_CURRENT_SOURCE_DIR}/main"

	"${CMAKE_CURRENT_SOURCE_DIR}/TestSDL"
	"${CMAKE_CURRENT_SOURCE_DIR}/TestChipmunk"
)

link_directories(
	"/usr/lib/i386-linux-gnu/" # OS 32 bits
	"/usr/lib/x86_64-linux-gnu/" # OS 64 bits

	"${CMAKE_CURRENT_BINARY_DIR}/AlgoRecherche"
	"${CMAKE_CURRENT_BINARY_DIR}/Aleatoire"
	"${CMAKE_CURRENT_BINARY_DIR}/Balle"
	"${CMAKE_CURRENT_BINARY_DIR}/GestionEnv"
	"${CMAKE_CURRENT_BINARY_DIR}/MenuSDL"

	"${CMAKE_CURRENT_BINARY_DIR}/main"

	"${CMAKE_CURRENT_BINARY_DIR}/TestSDL"	
	"${CMAKE_CURRENT_BINARY_DIR}/TestChipmunk"
)

add_subdirectory(AlgoRecherche)
add_subdirectory(Aleatoire)
add_subdirectory(Balle)
add_subdirectory(GestionEnv)
add_subdirectory(MenuSDL)

add_subdirectory(main)

add_subdirectory(TestChipmunk)
add_subdirectory(TestSDL)

