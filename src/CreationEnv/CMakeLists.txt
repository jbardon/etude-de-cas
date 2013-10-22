cmake_minimum_required(VERSION 2.8)

add_library(CreationEnvShared SHARED CreationEnv.c)
add_library(CreationEnvStatic STATIC CreationEnv.c)

set_target_properties(
	CreationEnvShared CreationEnvStatic
	PROPERTIES OUTPUT_NAME CreationEnv
)

