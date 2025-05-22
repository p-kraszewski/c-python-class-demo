cmake_minimum_required(VERSION 3.28)
project(tm1_demo)

find_package(Python  COMPONENTS Interpreter Development)
include_directories ("${Python_INCLUDE_DIRS}")

set(CMAKE_C_STANDARD 23)

add_library(${PROJECT_NAME} SHARED tm1_demo.c)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
