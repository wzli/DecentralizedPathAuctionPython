from decentralized_path_auction_python import *

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
