#include <decentralized_path_auction/graph.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace decentralized_path_auction;

PYBIND11_MAKE_OPAQUE(Nodes);
PYBIND11_MAKE_OPAQUE(Path);

PYBIND11_MODULE(bindings, dpa) {
    py::bind_vector<Nodes>(dpa, "Nodes");
    py::bind_vector<Path>(dpa, "Path");

    // Point
    py::class_<Point> point(dpa, "Point");
    point.def(py::init<float const &, float const &, float const &>());
    point.def_property("x", &Point::get<0>, &Point::set<0>);
    point.def_property("y", &Point::get<1>, &Point::set<1>);
    point.def_property("z", &Point::get<2>, &Point::set<2>);
    point.def("tup",
        [](const Point &p) {
            return std::make_tuple(p.get<0>(), p.get<1>(), p.get<2>());
        }
    );

    // Node
    py::class_<Node, NodePtr> node(dpa, "Node");
    node.def(py::init<Point>());
    node.def_readonly("position", &Node::position);
    node.def_readwrite("state", &Node::state);
    node.def_readwrite("edges", &Node::edges);
    // node.def_readwrite("auction", &Node::auction);
    node.def_readwrite("custom_data", &Node::custom_data);
    // node.def("validate", &Node::validate);

    // Node::State
    py::enum_<Node::State> state(node, "State");
    state.value("DEFAULT", Node::DEFAULT);
    state.value("NO_PARKING", Node::NO_PARKING);
    state.value("NO_STOPPING", Node::NO_STOPPING);
    state.value("DISABLED", Node::DISABLED);
    state.value("DELETED", Node::DELETED);
    state.export_values();

    // Visit
    py::class_<Visit> visit(dpa, "Visit");
    visit.def(py::init<NodePtr>());
    visit.def_readwrite("price", &Visit::price);
    visit.def_readwrite("duration", &Visit::duration);
    visit.def_readwrite("base_price", &Visit::base_price);
    visit.def_readwrite("cost_estimate", &Visit::cost_estimate);
    visit.def_readwrite("time_estimate", &Visit::time_estimate);

    // NodeRTree
    py::class_<NodeRTree> node_rtree(dpa, "NodeRTree");
    node_rtree.def(py::init<>());
    node_rtree.def("insertNode", &NodeRTree::insertNode, "node"_a);
    node_rtree.def("removeNode", &NodeRTree::removeNode, "node"_a);
    node_rtree.def("clearNodes", &NodeRTree::clearNodes);

    node_rtree.def("findNode", &NodeRTree::findNode, "position"_a);
    node_rtree.def("findNearestNode", &NodeRTree::findNearestNode, "position"_a, "criterion"_a);
    node_rtree.def("findAnyNode", &NodeRTree::findAnyNode, "criterion"_a);

    node_rtree.def("containsNode", &NodeRTree::containsNode, "node"_a);

    // Graph
    py::class_<Graph, NodeRTree> graph(dpa, "Graph");
    graph.def(py::init<>());
    graph.def("clearNodes", &Graph::clearNodes);
    graph.def("removeNode", static_cast<bool (Graph::*)(NodePtr)>(&Graph::removeNode), "node"_a);
    graph.def("removeNode", static_cast<bool (Graph::*)(Point)>(&Graph::removeNode), "node"_a);
    graph.def("insertNode", &Graph::insertNode<>, "position"_a);

}
