#include "stable_distribution.hpp"
#include "stable_simulation.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(stable, m) {
    py::class_<StableDistribution>(m, "StableDistribution")
        .def(py::init<>())
        .def(py::init<double, double, double, double>())
        .def("get_param", &StableDistribution::getParametrization)
        .def("set_param", &StableDistribution::setParametrization)
    ;

    py::class_<Simulator>(m, "Simulator")
        .def(py::init<>())
        .def(py::init<double, double, double, double, unsigned short, unsigned int>())
        .def("symmetric", &Simulator::simulateSymmetricZVector)
        .def("non_symmetric", &Simulator::simulateNonSymmetricZVector)
        .def("stable", &Simulator::simulateStableXVector)
    ;
}