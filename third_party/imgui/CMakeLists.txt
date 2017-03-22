cmake_minimum_required (VERSION 2.6)
project (IMGUI)

include_directories(
	"imgui"
)

set(BUILD_SOURCES
	"imgui/imgui_draw.cpp"
  "imgui/imgui.cpp"
)

add_library(imgui STATIC ${BUILD_SOURCES})
