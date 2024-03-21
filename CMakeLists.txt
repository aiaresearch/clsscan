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

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

#add_executable(CLSSCAN Src/main_scan.cpp Src/scan.cpp)
#target_link_libraries(CLSSCAN ${SANE_LIBRARY} ${OpenCV_LIBS})

# add_executable(recognize Src/main_recognize.cpp Src/recognize.cpp Src/config.cpp)
# target_link_libraries(recognize ${OpenCV_LIBS} nlohmann_json::nlohmann_json)

# add_executable(testing Src/testing.cpp)
# target_link_libraries(testing ${OpenCV_LIBS} )

#add_executable(test Src/test.cpp)
#target_link_libraries(test ${OpenCV_LIBS})
# add_executable(ocr Src/ocr.cpp)
# target_link_libraries(ocr ${CURL_LIBRARIES})

add_executable(base Src/base64.cpp)
target_link_libraries(base ${CURL_LIBRARIES})
