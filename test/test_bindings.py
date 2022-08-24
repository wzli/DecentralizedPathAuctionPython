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

    #print("", file=sys.stderr)
    #print(node.position.tup(), node.state, file=sys.stderr)
