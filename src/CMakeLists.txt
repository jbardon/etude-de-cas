cmake_minimum_required(VERSION 2.8)

project(Jeu C)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Jeu")

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
	"/usr/include/chipmunk"
	"${CMAKE_CURRENT_SOURCE_DIR}/main"
	"${CMAKE_CURRENT_SOURCE_DIR}/GestionEnv"
	"${CMAKE_CURRENT_SOURCE_DIR}/Balle"
)

link_directories(
	"/usr/lib/i386-linux-gnu"
	"${CMAKE_CURRENT_BINARY_DIR}/main"
	"${CMAKE_CURRENT_BINARY_DIR}/GestionEnv"
	"${CMAKE_CURRENT_BINARY_DIR}/Balle"
)

add_subdirectory(GestionEnv)
add_subdirectory(Balle)
add_subdirectory(main)


