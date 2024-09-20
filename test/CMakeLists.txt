# Find Boost
find_package(Boost 1.76 REQUIRED COMPONENTS unit_test_framework)

# Add test executable
add_executable(stable_distribution_test stable_distribution_test.cpp ../src/stable_distribution.cpp)
add_executable(performance_test_stable performance_test_stable.cpp ../src/stable_distribution.cpp 
../src/stable_simulation.cpp)
add_executable(stable_simulation_test stable_simulation_test.cpp ../src/stable_simulation.cpp ../src/stable_distribution.cpp)
add_executable(stable_estimation_test stable_estimation_test.cpp ../src/stable_estimation.cpp ../src/stable_distribution.cpp)

# Link Boost libraries
target_link_libraries(stable_distribution_test Boost::unit_test_framework)
target_link_libraries(stable_simulation_test Boost::unit_test_framework)
target_link_libraries(stable_estimation_test Boost::unit_test_framework)