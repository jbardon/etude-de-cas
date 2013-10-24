cmake_minimum_required(VERSION 2.8)

add_library(BalleShared SHARED Balle.c)
add_library(BalleStatic STATIC Balle.c)

set_target_properties(
	BalleShared BalleStatic
	PROPERTIES OUTPUT_NAME Balle
)

