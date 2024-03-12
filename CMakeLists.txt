cmake_minimum_required(VERSION 3.27)

project(CLSSCAN)
set(CMAKE_CXX_STANDARD 17)

include_directories(Inc)

# Find the SANE library and include directories
find_library(SANE_LIBRARY sane REQUIRED)
find_path(SANE_INCLUDE_DIR sane/sane.h)
# Include the SANE headers
include_directories(${SANE_INCLUDE_DIR})

# Find OpenCV Package
find_package(OpenCV REQUIRED)
# Include OpenCV directories (Not necessary if using OpenCV targets)
#include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES "Src/*.*")

add_executable(CLSSCAN Src/main.cpp Src/scan.cpp)
target_link_libraries(CLSSCAN ${SANE_LIBRARY} ${OpenCV_LIBS})
