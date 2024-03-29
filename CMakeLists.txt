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

find_library(WIRINGPI_LIB wiringPi)

include_directories("/home/pi/Code/ClassificationOfPapers/bcm2835-1.75/src")
# LINK_LIBRARIES("/home/pi/Code/ClassificationOfPapers/bcm2835-1.75/src/libbcm2835.a")

find_package(PiPCA9685 REQUIRED)


file(GLOB_RECURSE SOURCES "Src/*.*")

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

#add_executable(CLSSCAN Src/main_scan.cpp Src/scan.cpp)
#target_link_libraries(CLSSCAN ${SANE_LIBRARY} ${OpenCV_LIBS})

add_executable(recognize Src/main.cpp Src/recognize.cpp Src/config.cpp Src/servo.cpp Src/GPIO.cpp Src/scan.cpp)
target_link_libraries(recognize ${OpenCV_LIBS} ${WIRINGPI_LIB} ${SANE_LIBRARY} nlohmann_json::nlohmann_json)
target_link_libraries(recognize "/home/pi/Code/ClassificationOfPapers/bcm2835-1.75/src/libbcm2835.a" ${PiPCA9685_LIBRARY})

# add_executable(testing Src/testing.cpp)
# target_link_libraries(testing ${OpenCV_LIBS} )

#add_executable(test Src/test.cpp)
#target_link_libraries(test ${OpenCV_LIBS})
# add_executable(ocr Src/ocr.cpp)
# target_link_libraries(ocr ${CURL_LIBRARIES})

# add_executable(base Src/base64.cpp)
# target_link_libraries(base ${CURL_LIBRARIES})

# add_executable(config Src/config.cpp)
# target_link_libraries(config ${OpenCV_LIBS} nlohmann_json::nlohmann_json)

# add_executable(GPIO Src/servo.cpp Src/GPIO.cpp Src/delay.cpp)
# target_link_libraries(GPIO ${WIRINGPI_LIB})
# target_link_libraries(GPIO "/home/pi/Code/ClassificationOfPapers/bcm2835-1.75/src/libbcm2835.a" ${PiPCA9685_LIBRARY})

# add_executable(first Src/firsttest.cpp)
# target_link_libraries(first ${OpenCV_LIBS} nlohmann_json::nlohmann_json)
