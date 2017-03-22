cmake_minimum_required (VERSION 2.6)
project (BUILD_SAMPLES)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CPP_GCC_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -w -std=c++14 -includeEngineInc.h")
set(CPP_NMAKE_COMPILE_FLAGS  "${CMAKE_CXX_FLAGS} /O2 /W3 /FIEngineInc.h")

#Only supporting gcc on linux, mingw(gcc)/nmake on windows
if(WIN32 AND NOT MINGW)
		set(CMAKE_CXX_FLAGS "${CPP_NMAKE_COMPILE_FLAGS}")
else()
	set(CMAKE_CXX_FLAGS "${CPP_GCC_COMPILE_FLAGS}")
endif()

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
	"${LIB_PATH}/glad/include"
	"${LIB_PATH}/imgui/imgui"
)

set(BUILD_SOURCES
	"WindowCreation.cpp"
	"WindowInputHandler.cpp"
	"BasicLogSample.cpp"
	"ExtensionLoadingAndDebugOutput.cpp"
	"RenderingSample/RenderingSample.cpp"
	"TemplateUtilitiesSamples.cpp"
)

if(WIN32)
	if(MINGW)
		set(SAMPLE_LIBS
			"${ENGINE_LIB_PATH}/libengine.a"
			"${ENGINE_LIB_PATH}/libphysfs.a"
			"${ENGINE_LIB_PATH}/libglfw3.a"
			"${ENGINE_LIB_PATH}/libglad.a"
			"${ENGINE_LIB_PATH}/libimgui.a"
			opengl32
			pthread
		)
	else()
		set(SAMPLE_LIBS
			"${ENGINE_LIB_PATH}/engine.lib"
			"${ENGINE_LIB_PATH}/physfs.lib"
			"${ENGINE_LIB_PATH}/glfw3.lib"
			"${ENGINE_LIB_PATH}/glad.lib"
			"${ENGINE_LIB_PATH}/imgui.lib"
			opengl32
		)
	endif()
else()
	set(SAMPLE_LIBS
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
		z
		"${ENGINE_LIB_PATH}/libengine.a"
		"${ENGINE_LIB_PATH}/libphysfs.a"
		"${ENGINE_LIB_PATH}/libglfw3.a"
		"${ENGINE_LIB_PATH}/libglad.a"
		"${ENGINE_LIB_PATH}/libimgui.a"
	)
endif()

foreach(testsourcefile ${BUILD_SOURCES})

	get_filename_component(test_filename ${testsourcefile} NAME_WE)

    add_executable(${test_filename} ${testsourcefile})

    target_link_libraries(${test_filename}
    	${SAMPLE_LIBS}
	)


endforeach(testsourcefile ${BUILD_SOURCES})
