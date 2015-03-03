cmake_minimum_required(VERSION 2.8.9)

project(number-plate-recognition)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed")
SET(PROJECT_SOURCE_DIR "${number-plate-recognition_SOURCE_DIR}/src")

INCLUDE(FindPkgConfig)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

find_package( OpenCV REQUIRED )
find_package(Boost 1.55.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(doc)