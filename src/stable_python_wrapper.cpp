#include "stable.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(stable, m) {
    py::class_<StableDistribution>(m, "StableDistribution")
        .def(py::init<>())
        .def(py::init<double, double, double, double>())
        .def("generate_symmetric_number", &StableDistribution::generateSymmetricStableNumber)
        .def("generate_nonsymmetric_number", &StableDistribution::generateNonSymmetricStableNumber)
        .def("get_uniform_bounds", &StableDistribution::getUniformBounds)
        .def("generate_uniform_number", &StableDistribution::generateUniformNumber)
        .def("generate_exponential_number", &StableDistribution::generateExponentialNumber)
        .def("generate_uniform_vector", &StableDistribution::generateUniformVector, 
        py::arg("n"))
        .def("generate_exponential_vector", &StableDistribution::generateExponentialVector,
        py::arg("n"))
        ;
}