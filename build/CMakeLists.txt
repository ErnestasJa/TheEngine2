cmake_minimum_required (VERSION 2.8)
project (ENGINE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

MESSAGE(STATUS "CMAKE_CXX_COMPILER_VERSION: " "${CMAKE_CXX_COMPILER_VERSION}")

set(CPP_GCC_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -w -std=c++14 -include EngineInc.h")
set(CPP_NMAKE_COMPILE_FLAGS  "${CMAKE_CXX_FLAGS} /Gm- /MP /O2 /W3 /FIEngineInc.h")

#Only supporting gcc on linux, mingw(gcc)/nmake on windows
if(WIN32 AND NOT MINGW)
		set(CMAKE_CXX_FLAGS "${CPP_NMAKE_COMPILE_FLAGS}")
else()
	set(CMAKE_CXX_FLAGS "${CPP_GCC_COMPILE_FLAGS}")
endif()


set(ENGINE_PATH "" CACHE PATH "Set this to directory which contains 'include', 'src' directories for engine")
set(BUILD_DIR "" CACHE PATH "Set this to directory to your build dir.")
set(ENGINE_SRC_PATH "${ENGINE_PATH}/src" )
set(ENGINE_INC_PATH "${ENGINE_PATH}/include" )
set(LIB_PATH "${ENGINE_PATH}/third_party" )
set(LIBBIN_PATH "${ENGINE_PATH}/build/lib" )

#define output library path for solutions
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIBBIN_PATH} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIBBIN_PATH} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIBBIN_PATH} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

#third party lib configuration
set(PHYSFS_INC_PATH "${ENGINE_PATH}/third_party/physfs/src" )


#Includes
include_directories("${ENGINE_INC_PATH}/"
	"${ENGINE_INC_PATH}/filesystem"
	"${LIB_PATH}"
	"${LIB_PATH}/glm"
	"${LIB_PATH}/glfw/include"
	"${LIB_PATH}/glad/include"
)


#Platform independent sources


#Platform specific
#Linux
if(UNIX) #Asume that we're compiling on some linux distro that can actually compile the project.
set(LINUX_SOURCES
	"${ENGINE_SRC_PATH}/platform/LinuxFileSystem.cpp"
)
endif()

#Windows
if(WIN32)
set(WINDOWS_SOURCES
	"${ENGINE_SRC_PATH}/platform/WindowsFileSystem.cpp"
)
endif()

#Android (maybe a bit too optimistic..)

#Cross platform sources
set(CROSSPLATFORM_SOURCES
	"${ENGINE_SRC_PATH}/core/StringUtil.cpp"

	"${ENGINE_SRC_PATH}/filesystem/PathUtil.cpp"
	"${ENGINE_SRC_PATH}/filesystem/FileReader.cpp"
	"${ENGINE_SRC_PATH}/filesystem/FileWriter.cpp"
	"${ENGINE_SRC_PATH}/filesystem/FileSystem.cpp"

	"${ENGINE_SRC_PATH}/window/GLFWWindow.cpp"
	"${ENGINE_SRC_PATH}/window/GLFWInputDevice.cpp"

	"${ENGINE_SRC_PATH}/input/InputMapping.cpp"

	"${ENGINE_SRC_PATH}/log/LogImpl.cpp"

	"${ENGINE_SRC_PATH}/render/GLExtensionLoader.cpp"
	"${ENGINE_SRC_PATH}/render/GLRendererDebugMessage.cpp"
	"${ENGINE_SRC_PATH}/render/GLRendererDebugMessageMonitor.cpp"
	"${ENGINE_SRC_PATH}/render/GLGpuShaderProgram.cpp"
	"${ENGINE_SRC_PATH}/render/GLGpuShaderProgramUniform.cpp"
	"${ENGINE_SRC_PATH}/render/GLRenderer.cpp"
	"${ENGINE_SRC_PATH}/render/GLGpuBufferObject.cpp"
	"${ENGINE_SRC_PATH}/render/GLGpuBufferArrayObject.cpp"
	"${ENGINE_SRC_PATH}/render/GLRenderContext.cpp"
	"${ENGINE_SRC_PATH}/render/GLTexture.cpp"
	"${ENGINE_SRC_PATH}/render/GLFrameBufferObject.cpp"
	"${ENGINE_SRC_PATH}/render/GLRenderBufferObject.cpp"
)

add_library(engine STATIC
	${LINUX_SOURCES}
	${WINDOWS_SOURCES}
	${CROSSPLATFORM_SOURCES}
)
