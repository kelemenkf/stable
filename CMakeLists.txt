cmake_minimum_required(VERSION 3.10)
project(stable LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Python3 3.12 REQUIRED COMPONENTS Interpreter Development)

# Add the include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(test)