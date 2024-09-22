set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(stable stable_distribution.cpp stable_simulation.cpp estimation/stable_estimation.cpp stable_python_wrapper.cpp)
install(TARGETS stable DESTINATION .)

add_executable(main  main.cpp stable_distribution.cpp)

add_library(stable_library estimation/utils/quantile_estimator_lookup_table.cpp estimation/stable_estimation_quantile.cpp estimation/stable_estimation
stable_distribution.cpp stable_simulation.cpp utils/utils.cpp)