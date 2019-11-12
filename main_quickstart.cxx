#include <iostream>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>

#include <boost/geometry/index/rtree.hpp>

// to store queries results
#include <vector>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

int main() {

    typedef bg::model::point<float, 2, bg::cs::cartesian> point;
    typedef bg::model::box<point> box;
    typedef std::pair<box, unsigned> value;

    // creating r-tree with maximum of 16 elements in each node
    bgi::rtree<value, bgi::quadratic<16>> rtree;

    // create something to store into the rtree
    for (std::size_t i = 0; i < 10; i++) {
        box b(point(i + 0.0f, i + 0.0f), point(i + 0.5f, i + 0.5f));
        // insert the new element
        rtree.insert(std::make_pair(b, i));
    }

    // find values intersecting some area defined by a box
    box query_box(point(0, 0), point(5,5));
    std::vector<value> result_s;
    rtree.query(bgi::intersects(query_box), std::back_inserter(result_s));

    // find 5 nearest values to a point
    std::vector<value> result_n;
    rtree.query(bgi::nearest(point(0,0), 5), std::back_inserter(result_n));

    // display results
    std::cout << "Spatial query box: " << bg::wkt(query_box) << std::endl;
    std::cout << "Spatial query result:" << std::endl;
    for (const auto& r: result_s) {
        auto [b, i] = r;
        std::cout << bg::wkt<box>(b) << " -> " << i << std::endl;
    }

    std::cout << "k-NN query point: " << bg::wkt<point>(point(0, 0)) << std::endl;
    std::cout << "k-NN query result: " << std::endl;
    for (const auto& r: result_n) {
        auto [b, i] = r;
        std::cout << bg::wkt<box>(b) << " -> " << i << std::endl;
    }


    return 0;
}