set(PYBIND11_FINDPYTHON ON)

pybind11_add_module(stable_cpp stable_distribution.cpp stable_simulation.cpp estimation/stable_estimation.cpp stable_python_wrapper.cpp)
install(TARGETS stable_cpp DESTINATION .)

add_executable(main  main.cpp)
add_executable(build_lookup_tables build_lookup_tables.cpp)

target_link_libraries(main stable_library)
target_link_libraries(build_lookup_tables stable_library)
