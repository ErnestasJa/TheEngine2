cmake_minimum_required (VERSION 2.6)
project (BUILD_SAMPLES)


if("${WINDOWS_BUILD}" STREQUAL "1")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /W3 /FI EngineInc.h")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -w -Wfatal-errors -std=c++14 -include EngineInc.h")
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
	"${LIB_PATH}/glxw/include"
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
			"${ENGINE_LIB_PATH}/libglxw_wgl.a"
			"${ENGINE_LIB_PATH}/libglxw.a"
			opengl32
			pthread
		)
	else()
		set(SAMPLE_LIBS
			"${ENGINE_LIB_PATH}/libengine.lib"
			"${ENGINE_LIB_PATH}/libphysfs.lib"
			"${ENGINE_LIB_PATH}/libglfw3.lib"
			"${ENGINE_LIB_PATH}/libglxw.lib"
			opengl32
			pthread
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
		"${ENGINE_LIB_PATH}/libglxw.a"
	)
endif()

foreach(testsourcefile ${BUILD_SOURCES})

	get_filename_component(test_filename ${testsourcefile} NAME_WE)
    
    add_executable(${test_filename} ${testsourcefile})

    target_link_libraries(${test_filename} 
    	${SAMPLE_LIBS}
	)
	
	
endforeach(testsourcefile ${BUILD_SOURCES})

