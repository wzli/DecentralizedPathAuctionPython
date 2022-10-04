#include <decentralized_path_auction/graph.hpp>
#include <decentralized_path_auction/path_search.hpp>
#include <decentralized_path_auction/path_sync.hpp>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl_bind.h>

namespace py = pybind11;
using namespace pybind11::literals;
using namespace decentralized_path_auction;

PYBIND11_MAKE_OPAQUE(Nodes);
PYBIND11_MAKE_OPAQUE(Path);
PYBIND11_MAKE_OPAQUE(PathSync::Paths);

PYBIND11_MODULE(bindings, dpa) {
    py::bind_vector<Nodes>(dpa, "Nodes");
    py::bind_vector<Path>(dpa, "Path");
    py::bind_map<PathSync::Paths>(dpa, "Paths");

    // Point
    py::class_<Point> point(dpa, "Point");
    point.def(
            py::init<float const&, float const&, float const&>(), "x"_a = 0, "y"_a = 0, "z"_a = 0);
    point.def_property("x", &Point::get<0>, &Point::set<0>);
    point.def_property("y", &Point::get<1>, &Point::set<1>);
    point.def_property("z", &Point::get<2>, &Point::set<2>);
    point.def("tup",
            [](const Point& p) { return std::make_tuple(p.get<0>(), p.get<1>(), p.get<2>()); });

    // Node
    py::class_<Node, NodePtr> node(dpa, "Node");
    py::enum_<Node::State> state(node, "State");
    state.value("DEFAULT", Node::DEFAULT);
    state.value("NO_FALLBACK", Node::NO_FALLBACK);
    state.value("NO_PARKING", Node::NO_PARKING);
    state.value("NO_STOPPING", Node::NO_STOPPING);
    state.value("DISABLED", Node::DISABLED);
    state.value("DELETED", Node::DELETED);
    state.export_values();
    node.def(py::init<Point, Node::State, Nodes>(), "position"_a, "state"_a = Node::DEFAULT,
            "edges"_a = Nodes());
    node.def_readonly("position", &Node::position);
    node.def_readwrite("state", &Node::state);
    node.def_readwrite("edges", &Node::edges);
    // node.def_readwrite("auction", &Node::auction);
    node.def_property("custom_data", [](const Node& node) -> size_t { return (size_t)node.custom_data; },
            [](Node& node, size_t val) { *(size_t*)(&node.custom_data) = val; });
    // node.def("validate", &Node::validate);

    // Visit
    py::class_<Visit> visit(dpa, "Visit");
    visit.def(py::init<NodePtr, float, float, float, float, float>(), "node"_a, "price"_a = 0,
            "duration"_a = 0, "base_price"_a = 0, "cost_estimate"_a = 0, "time_estimate"_a = 0);
    visit.def_readwrite("node", &Visit::node);
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

    // PathSearch
    py::class_<PathSearch> path_search(dpa, "PathSearch");

    py::enum_<PathSearch::Error>(path_search, "Error")
            .value("SUCCESS", PathSearch::SUCCESS)
            .value("FALLBACK_DIVERTED", PathSearch::FALLBACK_DIVERTED)
            .value("COST_LIMIT_EXCEEDED", PathSearch::COST_LIMIT_EXCEEDED)
            .value("ITERATIONS_REACHED", PathSearch::ITERATIONS_REACHED)
            .value("PATH_EXTENDED", PathSearch::PATH_EXTENDED)
            .value("PATH_CONTRACTED", PathSearch::PATH_CONTRACTED)
            .value("DESTINATION_DURATION_NEGATIVE", PathSearch::DESTINATION_DURATION_NEGATIVE)
            .value("DESTINATION_NODE_INVALID", PathSearch::DESTINATION_NODE_INVALID)
            .value("DESTINATION_NODE_NO_PARKING", PathSearch::DESTINATION_NODE_NO_PARKING)
            .value("DESTINATION_NODE_DUPLICATED", PathSearch::DESTINATION_NODE_DUPLICATED)
            .value("SOURCE_NODE_NOT_PROVIDED", PathSearch::SOURCE_NODE_NOT_PROVIDED)
            .value("SOURCE_NODE_INVALID", PathSearch::SOURCE_NODE_INVALID)
            .value("SOURCE_NODE_DISABLED", PathSearch::SOURCE_NODE_DISABLED)
            .value("SOURCE_NODE_PRICE_INFINITE", PathSearch::SOURCE_NODE_PRICE_INFINITE)
            .value("CONFIG_AGENT_ID_EMPTY", PathSearch::CONFIG_AGENT_ID_EMPTY)
            .value("CONFIG_COST_LIMIT_NON_POSITIVE", PathSearch::CONFIG_COST_LIMIT_NON_POSITIVE)
            .value("CONFIG_PRICE_INCREMENT_NON_POSITIVE",
                    PathSearch::CONFIG_PRICE_INCREMENT_NON_POSITIVE)
            .value("CONFIG_TIME_EXCHANGE_RATE_NON_POSITIVE",
                    PathSearch::CONFIG_TIME_EXCHANGE_RATE_NON_POSITIVE)
            .value("CONFIG_TRAVEL_TIME_MISSING", PathSearch::CONFIG_TRAVEL_TIME_MISSING)
            .export_values();

    py::class_<PathSearch::Config>(path_search, "Config")
            .def_readwrite("agent_id", &PathSearch::Config::agent_id)
            .def_readwrite("cost_limit", &PathSearch::Config::cost_limit)
            .def_readwrite("price_increment", &PathSearch::Config::price_increment)
            .def_readwrite("time_exchange_rate", &PathSearch::Config::time_exchange_rate)
            .def_readwrite("travel_time", &PathSearch::Config::travel_time)
            .def(py::init<std::string, float, float, float, PathSearch::TravelTime>(),
                    "agent_id"_a, "cost_limit"_a = FLT_MAX, "price_increment"_a = 1,
                    "time_exchange_rate"_a = 1,
                    "travel_time"_a = PathSearch::TravelTime(PathSearch::travelDistance))
            .def("validate", &PathSearch::Config::validate);

    path_search.def(py::init<PathSearch::Config>(), "config"_a);
    path_search.def("getConfig", static_cast<PathSearch::Config& (PathSearch::*) ()>(&PathSearch::getConfig),
            py::return_value_policy::reference);

    path_search.def("getDestinations", &PathSearch::getDestinations);
    path_search.def("setDestinations", &PathSearch::setDestinations, "destinations"_a,
            "duration"_a = FLT_MAX);

    path_search.def("selectSource", &PathSearch::selectSource, "sources"_a);
    path_search.def("iterate",
            static_cast<PathSearch::Error (PathSearch::*)(Path&, size_t)>(&PathSearch::iterate),
            "path"_a, "iterations"_a = 0);
    path_search.def("iterate",
            static_cast<PathSearch::Error (PathSearch::*)(Path&, size_t, float)>(
                    &PathSearch::iterate),
            "path"_a, "iterations"_a, "fallback_cost"_a);
    path_search.def("resetCostEstimates", &PathSearch::resetCostEstimates);

    // PathSync
    py::class_<PathSync> path_sync(dpa, "PathSync");

    py::enum_<PathSync::Error>(path_sync, "Error")
            .value("SUCCESS", PathSync::SUCCESS)
            .value("REMAINING_DURATION_INFINITE", PathSync::REMAINING_DURATION_INFINITE)
            .value("SOURCE_NODE_OUTBID", PathSync::SOURCE_NODE_OUTBID)
            .value("DESTINATION_NODE_NO_PARKING", PathSync::DESTINATION_NODE_NO_PARKING)
            .value("VISIT_NODE_INVALID", PathSync::VISIT_NODE_INVALID)
            .value("VISIT_NODE_DISABLED", PathSync::VISIT_NODE_DISABLED)
            .value("VISIT_DURATION_NEGATIVE", PathSync::VISIT_DURATION_NEGATIVE)
            .value("VISIT_PRICE_ALREADY_EXIST", PathSync::VISIT_PRICE_ALREADY_EXIST)
            .value("VISIT_PRICE_LESS_THAN_START_PRICE",
                    PathSync::VISIT_PRICE_LESS_THAN_START_PRICE)
            .value("VISIT_BID_ALREADY_REMOVED", PathSync::VISIT_BID_ALREADY_REMOVED)
            .value("PATH_EMPTY", PathSync::PATH_EMPTY)
            .value("PATH_VISIT_DUPLICATED", PathSync::PATH_VISIT_DUPLICATED)
            .value("PATH_CAUSES_CYCLE", PathSync::PATH_CAUSES_CYCLE)
            .value("PATH_ID_STALE", PathSync::PATH_ID_STALE)
            .value("PATH_ID_MISMATCH", PathSync::PATH_ID_MISMATCH)
            .value("AGENT_ID_EMPTY", PathSync::AGENT_ID_EMPTY)
            .value("AGENT_ID_NOT_FOUND", PathSync::AGENT_ID_NOT_FOUND)
            .value("PROGRESS_DECREASE_DENIED", PathSync::PROGRESS_DECREASE_DENIED)
            .value("PROGRESS_EXCEED_PATH_SIZE", PathSync::PROGRESS_EXCEED_PATH_SIZE)
            .value("PROGRESS_MIN_EXCEED_MAX", PathSync::PROGRESS_MIN_EXCEED_MAX)
            .export_values();

    py::class_<PathSync::PathInfo>(path_sync, "PathInfo")
            .def_readwrite("path", &PathSync::PathInfo::path)
            .def_readwrite("path_id", &PathSync::PathInfo::path_id)
            .def_readwrite("progress_min", &PathSync::PathInfo::progress_min)
            .def_readwrite("progress_max", &PathSync::PathInfo::progress_max)
            .def(py::init<Path, size_t, size_t, size_t>(), "path"_a, "path_id"_a = 0,
                    "progress_min"_a = 0, "progress_max"_a = 0);

    py::class_<PathSync::WaitStatus>(path_sync, "WaitStatus")
            .def_readwrite("error", &PathSync::WaitStatus::error)
            .def_readwrite("blocked_progress", &PathSync::WaitStatus::blocked_progress)
            .def_readwrite("remaining_duration", &PathSync::WaitStatus::remaining_duration)
            .def(py::init<PathSync::Error, size_t, float>(), "error"_a, "blocked_progress"_a,
                    "remaining_duration"_a);

    path_sync.def(py::init<>());
    path_sync.def("updatePath", &PathSync::updatePath, "agent_id"_a, "path"_a, "path_id"_a);
    path_sync.def("updateProgress", &PathSync::updateProgress, "agent_id"_a, "progress_min"_a,
            "progress_max"_a, "path_id"_a);
    path_sync.def("removePath", &PathSync::removePath, "agent_id"_a);
    path_sync.def("clearPaths", &PathSync::clearPaths);
    path_sync.def("getPaths", &PathSync::getPaths, py::return_value_policy::reference);
    path_sync.def("checkWaitStatus", &PathSync::checkWaitStatus, "agent_id"_a);
}
