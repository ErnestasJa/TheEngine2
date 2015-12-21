cmake_minimum_required (VERSION 2.6)
project (BUILD_TESTS)

add_subdirectory("${BUILD_TESTS_SOURCE_DIR}/gtest")
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -w -Wfatal-errors -std=c++14")
set(ENGINE_PATH "${BUILD_TESTS_SOURCE_DIR}/../" )
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

add_executable(PathTest 
	filesystem/PathTest.cpp
)

target_link_libraries(PathTest
	"${ENGINE_LIB_PATH}/libengine.a"
	gtest
	gtest_main
)