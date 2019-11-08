cmake_minimum_required (VERSION 2.8)
project (ENGINE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

MESSAGE(STATUS "CMAKE_CXX_COMPILER_VERSION: " "${CMAKE_CXX_COMPILER_VERSION}")

set(CPP_GCC_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -w -std=c++17 -include EngineInc.h")
set(CPP_NMAKE_COMPILE_FLAGS  "${CMAKE_CXX_FLAGS} /Gm- /MP /O2 /W3 /FIEngineInc.h")



set(ENGINE_PATH "./")
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
	"${LIB_PATH}/imgui/imgui"
	"${LIB_PATH}/stb_image"
	#"${LIB_PATH}/fmt/include"
)

file(GLOB_RECURSE ENGINE_INCLUDES
    "${ENGINE_SRC_PATH}/*.h"
    "${ENGINE_SRC_PATH}/*.hpp"
    "${ENGINE_INC_PATH}/*.h"
    "${ENGINE_INC_PATH}/*.hpp"
)


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
	"${ENGINE_SRC_PATH}/render/GLTexture.cpp"
	"${ENGINE_SRC_PATH}/render/GLFrameBufferObject.cpp"
	"${ENGINE_SRC_PATH}/render/GLRenderBufferObject.cpp"
	"${ENGINE_SRC_PATH}/render/PerspectiveCamera.cpp"
	"${ENGINE_SRC_PATH}/render/BaseMaterial.cpp"
	"${ENGINE_SRC_PATH}/render/BaseMesh.cpp"
	"${ENGINE_SRC_PATH}/render/AnimatedMesh.cpp"
	"${ENGINE_SRC_PATH}/render/RenderContext.cpp"

	"${ENGINE_SRC_PATH}/render/debug/DebugLineMesh.cpp"

	"${ENGINE_SRC_PATH}/gui/GuiImpl.cpp"
	"${ENGINE_SRC_PATH}/gui/imgui_impl.cpp"
	"${ENGINE_SRC_PATH}/gui/imgui_gl3_impl.h"
	"${ENGINE_SRC_PATH}/gui/imgui_gl3_impl.cpp"
	"${ENGINE_SRC_PATH}/gui/ImGuiGlfwEventHandler.cpp"

	"${ENGINE_SRC_PATH}/resource_management/GpuProgramManager.cpp"
	"${ENGINE_SRC_PATH}/resource_management/ImageLoader.cpp"
	"${ENGINE_SRC_PATH}/resource_management/mesh/IQMLoader.cpp"
	"${ENGINE_SRC_PATH}/resource_management/mesh/MBDLoader.cpp"

	"${ENGINE_SRC_PATH}/engine/EngineContext.cpp"
	"${ENGINE_SRC_PATH}/render/AnimatedMesh.cpp"

        src/input/InputHandlerHandle.cpp)


add_subdirectory ("${LIB_PATH}/glad")
add_subdirectory ("${LIB_PATH}/glfw")
add_subdirectory ("${LIB_PATH}/imgui")
add_subdirectory ("${LIB_PATH}/physfs")
add_subdirectory ("${LIB_PATH}/fmt")

add_library(engine STATIC
	${LINUX_SOURCES}
	${WINDOWS_SOURCES}
	${CROSSPLATFORM_SOURCES}
	${ENGINE_INCLUDES}
)
set_target_properties(engine PROPERTIES COMPILE_FLAGS "${CPP_GCC_COMPILE_FLAGS}")

target_link_libraries(engine glad glfw imgui physfs-static fmt)

add_dependencies(engine glad glfw imgui physfs-static fmt)
