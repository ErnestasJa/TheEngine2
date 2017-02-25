cmake_minimum_required(VERSION 2.8)

project(libglad)

include_directories("include")

add_library(glad STATIC src/glad.c)