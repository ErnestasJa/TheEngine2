cmake_minimum_required (VERSION 2.6)
project (BUILD_SAMPLES)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -w -Wfatal-errors -std=c++14 -include EngineInc.h")
set(ENGINE_PATH "" CACHE PATH "Set this to directory which contains 'include', 'src' directories for engine")
set(ENGINE_SRC_PATH "${ENGINE_PATH}/src" )
set(ENGINE_INC_PATH "${ENGINE_PATH}/include" )
set(ENGINE_LIB_PATH "${ENGINE_PATH}/build/lib" )
set(LIB_PATH "${ENGINE_PATH}/third_party" )

message(STATUS "ENGINE_SRC_PATH: " ${ENGINE_SRC_PATH})
message(STATUS "ENGINE_INC_PATH: " ${ENGINE_INC_PATH})
message(STATUS "ENGINE_LIB_PATH: " ${ENGINE_LIB_PATH})


include_directories(
	"${ENGINE_INC_PATH}"
	"${LIB_PATH}/glm"
	"${LIB_PATH}/glfw/include"
	"${LIB_PATH}/glxw/include"
)

set(BUILD_SOURCES 
	"WindowCreation.cpp" 
	"WindowInputHandler.cpp" 
	"BasicLogSample.cpp"
	"ExtensionLoadingAndDebugOutput.cpp"
	"BasicShaderSample.cpp"
)

set(SYSTEM_LINUX_LIBS
GL
GLU
X11
Xxf86vm
Xinerama 
Xrandr
Xcursor
Xi
rt 
m
pthread
dl
z)

foreach(testsourcefile ${BUILD_SOURCES})

	get_filename_component(test_filename ${testsourcefile} NAME_WE)
    
    add_executable(${test_filename} ${testsourcefile})

    target_link_libraries(${test_filename} 
    	${SYSTEM_LINUX_LIBS}
		"${ENGINE_LIB_PATH}/libengine.a"
		"${ENGINE_LIB_PATH}/libphysfs.a"
		"${ENGINE_LIB_PATH}/libglfw3.a"
		"${ENGINE_LIB_PATH}/libglxw.a"
	)

endforeach(testsourcefile ${BUILD_SOURCES})

