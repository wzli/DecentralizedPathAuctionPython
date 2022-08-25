from decentralized_path_auction_python import *

import sys

def test_point():
    p = Point(1,2,3)
    assert(p.x == 1)
    assert(p.y == 2)
    assert(p.z == 3)
    p.x = 4
    p.y = 5
    p.z = 6
    assert(p.x == 4)
    assert(p.y == 5)
    assert(p.z == 6)

def test_graph():
    graph = Graph()
    pathway = [graph.insertNode(Point(0,0,0))]
    for i in range(1, 10):
        pos = Point(i, 0,0)
        node = graph.insertNode(pos)
        assert(node)
        pathway[-1].edges.append(node)
        node.edges.append(pathway[-1])
        pathway.append(node)

    for node in pathway[1: -1]:
        assert(graph.containsNode(node))
        assert(len(node.edges) == 2)

    for i in range(0, 10):
        pos = Point(i, 0,0)
        assert(graph.findNode(pos).position.tup() == pos.tup())

        pos_offset = Point(i, 10, 0);
        assert(graph.findNearestNode(pos_offset, Node.State.DEFAULT).position.tup() == pos.tup())

        assert(graph.removeNode(pos))
        assert(not graph.findNode(pos))


def test_path_search():
    graph = Graph()
    nodes = [graph.insertNode(Point(0,0,0))]
    for i in range(1, 10):
        pos = Point(i, 0,0)
        node = graph.insertNode(pos)
        assert(node)
        nodes[-1].edges.append(node)
        node.edges.append(nodes[-1])
        nodes.append(node)

    config = PathSearch.Config("A")
    path_search = PathSearch(config)
    assert(path_search.getConfig().agent_id == config.agent_id)
    assert(path_search.setDestinations(Nodes([nodes[7]])) == PathSearch.Error.SUCCESS)
    assert(path_search.getDestinations().containsNode(nodes[7]))

    path = Path([Visit(nodes[3])])
    assert(path_search.iterate(path, 100) == PathSearch.Error.SUCCESS);
    assert([visit.node.position.x for visit in path] == list(range(3, 8)))

    #print("", file=sys.stderr)
    #print(node.position.tup(), node.state, file=sys.stderr)
