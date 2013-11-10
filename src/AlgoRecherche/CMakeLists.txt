cmake_minimum_required(VERSION 2.8)

add_library(AlgoRechercheShared SHARED AlgoRecherche.c)
add_library(AlgoRechercheStatic STATIC AlgoRecherche.c)

set_target_properties(
	AlgoRechercheShared AlgoRechercheStatic
	PROPERTIES OUTPUT_NAME AlgoRecherche
)

add_executable(testAlgoRecherche testAlgoRecherche.c)
target_link_libraries(testAlgoRecherche AlgoRecherche glib-2.0)

add_executable(testGlib testGlib.c)
target_link_libraries(testGlib glib-2.0)

add_test(testAlgoRecherche ./testAlgoRecherche)
add_test(testGlib ./testGlib)
