#Cmake file for lab 4

cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
project(Lab4)

file(GLOB_RECURSE lab4_sources "source/*.cpp")

add_executable(output ${lab4_sources})

target_include_directories(output PRIVATE /Users/danminik/Desktop/School/SeniorYear/Spring/CSE287/Labs/glm)
target_include_directories(output PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source/headers/)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(output ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
