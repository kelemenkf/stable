#include "stable_distribution.hpp"
#include "stable_simulation.hpp"
#include "stable_estimation_quantile.hpp"
#include "stable_density.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(stable_cpp, m) {
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


    py::class_<QuantileEstimator>(m, "Quantile")
        .def(py::init<std::vector<double>>())
        .def("get_v_alpha", &QuantileEstimator::getVAlpha)
        .def("get_v_beta", &QuantileEstimator::getVBeta)
        .def("get_v_gamma", &QuantileEstimator::getVGamma)
        .def("get_v_delta", &QuantileEstimator::getVDelta)
        .def("get_quantiles", &QuantileEstimator::getCorrectedQuantiles)
        ;


    py::class_<Density>(m, "Density")
        .def(py::init<std::vector<double>, double, double, double, double>())
        .def("get_pdf", &Density::getPdf)
        ;
}
