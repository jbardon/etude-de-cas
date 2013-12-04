cmake_minimum_required(VERSION 2.8)

configure_file(dico.txt dico.txt COPYONLY)
configure_file(dicofinal.txt dicofinal.txt COPYONLY)

add_library(AlgoRechercheShared SHARED AlgoRecherche.c)
add_library(AlgoRechercheStatic STATIC AlgoRecherche.c)

set_target_properties(
	AlgoRechercheShared AlgoRechercheStatic
	PROPERTIES OUTPUT_NAME AlgoRecherche
)

add_executable(testAlgoRecherche testAlgoRecherche.c)
target_link_libraries(testAlgoRecherche AlgoRecherche glib-2.0)

add_executable(testGlibV1_V2 testGlibV1_V2.c)
target_link_libraries(testGlibV1_V2 glib-2.0)

add_executable(testGlibV3 testGlibV3.c)
target_link_libraries(testGlibV3 glib-2.0)

add_test(testAlgoRecherche ./testAlgoRecherche)
add_test(testGlibV1_V2 ./testGlibV1_V2)
add_test(testGlibV3 ./testGlibV3)
