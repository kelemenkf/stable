set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(stable stable.cpp stable_python_wrapper.cpp)
install(TARGETS stable DESTINATION .)

add_executable(main  main.cpp stable.cpp)