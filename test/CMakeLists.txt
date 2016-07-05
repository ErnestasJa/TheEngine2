cmake_minimum_required (VERSION 2.6)
project (BUILD_TESTS)

add_subdirectory("${BUILD_TESTS_SOURCE_DIR}/gtest")
enable_testing()

if("${WINDOWS_BUILD}" STREQUAL "1")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /W3 /FI EngineInc.h")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -w -Wfatal-errors -std=c++14 -include EngineInc.h")
endif()

set(ENGINE_PATH "" CACHE PATH "Set this to directory which contains 'include', 'src' directories for engine")
set(ENGINE_SRC_PATH "${ENGINE_PATH}/src" )
set(ENGINE_INC_PATH "${ENGINE_PATH}/include" )
set(ENGINE_LIB_PATH "${ENGINE_PATH}/build/lib" )

message(STATUS "ENGINE_SRC_PATH: " ${ENGINE_SRC_PATH})
message(STATUS "ENGINE_INC_PATH: " ${ENGINE_INC_PATH})
message(STATUS "ENGINE_LIB_PATH: " ${ENGINE_LIB_PATH})


include_directories(
	"${gtest_SOURCE_DIR}/include"
	"${gtest_SOURCE_DIR}"
	"${ENGINE_INC_PATH}"
)

set(TEST_SOURCES 
	"core/StringExtensionTests.cpp" 
	
	"filesystem/PathTest.cpp" 
	"filesystem/FileSystemTest.cpp" 
)

foreach(testsourcefile ${TEST_SOURCES})

	get_filename_component(test_filename ${testsourcefile} NAME_WE)
    
    add_executable(${test_filename} ${testsourcefile})

	if("${WINDOWS_BUILD}" STREQUAL "1")
    target_link_libraries(${test_filename} 
		"${ENGINE_LIB_PATH}/engine.lib"
		"${ENGINE_LIB_PATH}/physfs.lib"
		gtest
		gtest_main
	)
	else()
	target_link_libraries(${test_filename} 
		"${ENGINE_LIB_PATH}/libengine.a"
		"${ENGINE_LIB_PATH}/libphysfs.a"
		gtest
		gtest_main
	)
	endif()
endforeach(testsourcefile ${TEST_SOURCES})

