cmake_minimum_required(VERSION 2.6)

project(yohub CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CXX_FLAGS
    -g
    -Wall
    -march=native
    -rdynamic
    -std=c++0x
)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O2")

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

find_package(Boost REQUIRED)
find_package(GTest REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

file(GLOB_RECURSE SOURCE 
     ${CMAKE_CURRENT_SOURCE_DIR} 
     ${CMAKE_CURRENT_SOURCE_DIR}/share/*.cc
     ${CMAKE_CURRENT_SOURCE_DIR}/network/*.cc
)

add_library(yohub ${SOURCE})
target_link_libraries(yohub pthread rt)
set_target_properties(yohub PROPERTIES OUTPUT_NAME yohub)
install(TARGETS yohub DESTINATION lib)

add_subdirectory(test)
add_subdirectory(demo)
add_subdirectory(bench)
