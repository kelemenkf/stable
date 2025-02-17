cmake_minimum_required(VERSION 3.10)
project(stable LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.86 REQUIRED COMPONENTS unit_test_framework timer) 
find_package(Python3 3.12 REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG REQUIRED)


include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(test)

add_library(stable_library 
    src/estimation/utils/quantile_estimator_lookup_table.cpp 
    src/estimation/stable_estimation_quantile.cpp 
    src/estimation/stable_estimation.cpp 
    src/stable_distribution.cpp 
    src/stable_simulation.cpp 
    src/utils/utils.cpp
)


target_include_directories(stable_library PUBLIC
    ${PROJECT_SOURCE_DIR}/include 
)


target_link_libraries(stable_library
    Boost::unit_test_framework
    Boost::timer
)
