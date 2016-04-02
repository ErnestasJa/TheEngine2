cmake_minimum_required (VERSION 2.6)
project (ENGINE)

MESSAGE(STATUS "CMAKE_CXX_COMPILER_VERSION: " "${CMAKE_CXX_COMPILER_VERSION}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -w -Wfatal-errors -std=c++14 -include EngineInc.h")

set(ENGINE_PATH "" CACHE PATH "Set this to directory which contains 'include', 'src' directories for engine")
set(ENGINE_SRC_PATH "${ENGINE_PATH}/src" )
set(ENGINE_INC_PATH "${ENGINE_PATH}/include" )
set(LIB_PATH "${ENGINE_PATH}/third_party" )
set(LIBBIN_PATH "${LIB_PATH}/third_patry/bin/linux" )

#third party lib configuration
set(PHYSFS_INC_PATH "${ENGINE_PATH}/third_party/physfs/src" )

#Let it build, let it build! :)
include_directories("${ENGINE_INC_PATH}/"
	"${ENGINE_INC_PATH}/filesystem"
	"${LIB_PATH}",
	"${LIB_PATH}/glm"
	"${LIB_PATH}/glfw/include"
	"${LIB_PATH}/glxw/include"
)

add_library(engine STATIC 
	"${ENGINE_SRC_PATH}/filesystem/PathExt.cpp"
	"${ENGINE_SRC_PATH}/filesystem/FileReader.cpp"
	"${ENGINE_SRC_PATH}/filesystem/FileWriter.cpp"
	"${ENGINE_SRC_PATH}/filesystem/FileSystem.cpp"

	"${ENGINE_SRC_PATH}/window/GLFWWindow.cpp"
	"${ENGINE_SRC_PATH}/window/DefaultWindowModule.cpp"
	"${ENGINE_SRC_PATH}/window/GLFWInputDevice.cpp"

	"${ENGINE_SRC_PATH}/input/InputMapping.cpp"

	"${ENGINE_SRC_PATH}/log/LogImpl.cpp"

	"${ENGINE_SRC_PATH}/core/StringExt.cpp"
	
	"${ENGINE_SRC_PATH}/render/GLExtensionLoader.cpp"
	"${ENGINE_SRC_PATH}/render/GLRendererDebugMessage.cpp"
	"${ENGINE_SRC_PATH}/render/GLRendererDebugMessageMonitor.cpp"
	"${ENGINE_SRC_PATH}/render/GLGpuShaderProgram.cpp"
	"${ENGINE_SRC_PATH}/render/GLRenderer.cpp"
	"${ENGINE_SRC_PATH}/render/GLGpuBufferObject.cpp"
	"${ENGINE_SRC_PATH}/render/GLGpuBufferArrayObject.cpp"
)