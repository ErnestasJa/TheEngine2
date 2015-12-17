cmake_minimum_required (VERSION 2.6)
project (ENGINE)

MESSAGE(STATUS "CMAKE_CXX_COMPILER_VERSION: " "${CMAKE_CXX_COMPILER_VERSION}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -w -Wfatal-errors -std=c++14")

set(ENGINE_PATH "${ENGINE_SOURCE_DIR}/../../../" )
set(ENGINE_SRC_PATH "${ENGINE_PATH}/src" )
set(ENGINE_INC_PATH "${ENGINE_PATH}/include" )
set(LIB_PATH "${ENGINE_PATH}/third_party" )
set(LIBBIN_PATH "${LIB_PATH}/third_patry/bin/linux" )


include_directories("${ENGINE_INC_PATH}/"
	"${ENGINE_INC_PATH}/filesystem"
)

add_library(engine STATIC 
	"${ENGINE_SRC_PATH}/filesystem/PathExt.cpp"
)
