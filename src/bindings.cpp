#include <decentralized_path_auction/graph.hpp>

#include <pybind11/pybind11.h>
namespace py = pybind11;
namespace dpa = decentralized_path_auction;


// graph.hpp

PYBIND11_MODULE(bindings, dpa) {
    py::class_<dpa::Point>(dpa, "Point")
        .def(py::init<float const &, float const &, float const &>())
        .def_property("x", &dpa::Point::get<0>, &dpa::Point::set<0>)
        .def_property("y", &dpa::Point::get<1>, &dpa::Point::set<1>)
        .def_property("z", &dpa::Point::get<2>, &dpa::Point::set<2>)
    ;
}
