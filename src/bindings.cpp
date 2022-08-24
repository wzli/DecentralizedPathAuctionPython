#include <decentralized_path_auction/graph.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace decentralized_path_auction;

PYBIND11_MAKE_OPAQUE(std::vector<Visit>);
PYBIND11_MAKE_OPAQUE(std::vector<std::shared_ptr<Node>>);

PYBIND11_MODULE(bindings, dpa) {
    // graph.hpp
    py::class_<Point>(dpa, "Point")
        .def(py::init<float const &, float const &, float const &>())
        .def_property("x", &Point::get<0>, &Point::set<0>)
        .def_property("y", &Point::get<1>, &Point::set<1>)
        .def_property("z", &Point::get<2>, &Point::set<2>)
    ;

    py::class_<Node> node(dpa, "Node");

    py::enum_<Node::State>(node, "State")
        .value("DEFAULT", Node::DEFAULT)
        .value("NO_PARKING", Node::NO_PARKING)
        .value("NO_STOPPING", Node::NO_STOPPING)
        .value("DISABLED", Node::DISABLED)
        .value("DELETED", Node::DELETED)
        .export_values();

    node
        .def(py::init<Point>())
        .def_readonly("position", &Node::position)
        .def_readwrite("state", &Node::state)
        .def_readwrite("edges", &Node::edges)
        // .def_readwrite("auction", &Node::auction)
        .def_readwrite("custom_data", &Node::custom_data)
    ;
}
