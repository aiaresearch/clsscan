cmake_minimum_required(VERSION 3.25)

project(CLSSCAN)
set(CMAKE_CXX_STANDARD 17)

include_directories(Inc)

# Find the SANE library and include directories
find_library(SANE_LIBRARY sane REQUIRED)
find_path(SANE_INCLUDE_DIR sane/sane.h)
# Include the SANE headers
include_directories(${SANE_INCLUDE_DIR})

# Find the curl library and include directories
find_library(CURL_LIBRARIES curl REQUIRED)
# find_path(CURL_INCLUDE_DIRS curl/curl.h)
include_directories(${CURL_INCLUDE_DIRS})

# Find OpenCV Package
find_package(OpenCV REQUIRED)
# Include OpenCV directories (Not necessary if using OpenCV targets)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES "Src/*.*")

add_executable(CLSSCAN Src/main.cpp Src/scan.cpp)
target_link_libraries(CLSSCAN ${SANE_LIBRARY} ${OpenCV_LIBS})

add_executable(ocr Src/ocr.cpp)
target_link_libraries(ocr ${CURL_LIBRARIES})
