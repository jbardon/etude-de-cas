cmake_minimum_required(VERSION 2.8)

add_library(GestionEnvShared SHARED GestionEnv.c)
add_library(GestionEnvStatic STATIC GestionEnv.c)

set_target_properties(
	GestionEnvShared GestionEnvStatic
	PROPERTIES OUTPUT_NAME GestionEnv
)

