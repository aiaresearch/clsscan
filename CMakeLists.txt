cmake_minimum_required(VERSION 3.0)
project(CLSSCAN)

# Set the C and C++ compilers
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Find OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

#set(CMAKE_BUILD_TYPE Debug)  
# Or Release, or any other valid build type

# Add executable
add_executable(recognize testing.cpp)

# Link against OpenCV
target_link_libraries(recognize PRIVATE ${OpenCV_LIBS})