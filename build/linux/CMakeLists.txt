cmake_minimum_required (VERSION 2.6)
project (ENGINE)

MESSAGE(STATUS "CMAKE_CXX_COMPILER_VERSION: " "${CMAKE_CXX_COMPILER_VERSION}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -w -Wfatal-errors -std=c++14 -include EngineInc.h")

set(ENGINE_PATH "${ENGINE_SOURCE_DIR}/../../" )
set(ENGINE_SRC_PATH "${ENGINE_PATH}/src" )
set(ENGINE_INC_PATH "${ENGINE_PATH}/include" )
set(LIB_PATH "${ENGINE_PATH}/third_party" )
set(LIBBIN_PATH "${LIB_PATH}/third_patry/bin/linux" )

#third party lib configuration
set(PHYSFS_INC_PATH "${ENGINE_PATH}/third_party/physfs/src" )

#Let it build, let it build! :)
include_directories("${ENGINE_INC_PATH}/"
	"${ENGINE_INC_PATH}/filesystem"
	"${LIB_PATH}"
)

add_library(engine STATIC 
	"${ENGINE_SRC_PATH}/filesystem/PathExt.cpp"
	"${ENGINE_SRC_PATH}/filesystem/FileReader.cpp"
	"${ENGINE_SRC_PATH}/filesystem/FileWriter.cpp"
	"${ENGINE_SRC_PATH}/filesystem/FileSystem.cpp"
)
