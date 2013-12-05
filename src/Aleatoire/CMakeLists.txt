cmake_minimum_required(VERSION 2.8)

add_library(AleatoireShared SHARED Aleatoire.c)
add_library(AleatoireStatic STATIC Aleatoire.c)

set_target_properties(
	AleatoireShared AleatoireStatic
	PROPERTIES OUTPUT_NAME Aleatoire
)

add_executable(testRandLettres testRandLettres.c)

add_test(testRandLettres ./testRandLettres)
