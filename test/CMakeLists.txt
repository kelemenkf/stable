# Find Boost
find_package(Boost 1.76 REQUIRED COMPONENTS unit_test_framework)

# Add test executable
add_executable(stable_test stable_test.cpp ../src/stable.cpp)
add_executable(performance_test_stable performance_test_stable.cpp ../src/stable.cpp)

# Link Boost libraries
target_link_libraries(stable_test Boost::unit_test_framework)


# Optionally, link with your source files
# target_link_libraries(test_program my_program)
