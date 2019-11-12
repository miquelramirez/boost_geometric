//
// Created by Miquel Ramirez on 12/11/2019.
//

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

    typedef bg::model::point<float, 3, bg::cs::cartesian> point;
    typedef bg::model::box<point> box;
    typedef std::pair<point, unsigned> value;

    // creating r-tree with maximum of 16 elements in each node
    bgi::rtree<value, bgi::quadratic<16>> rtree;


    point p1(1.5, 1.0, 0.2);
    // insert the new element
    rtree.insert(std::make_pair(p1, 0));

    point p2(2.51, 1.0, -0.1);
    // insert the new element
    rtree.insert(std::make_pair(p2, 1));

    point p3(-1.0, -1.0, 0.75);
    // insert the new element
    rtree.insert(std::make_pair(p3, 2));


    // find values intersecting some area defined by a box
    box query_box(point(-2.5, -2.5, -0.5), point(2.5,2.5, 0.5));
    std::vector<value> result_s;
    rtree.query(bgi::intersects(query_box), std::back_inserter(result_s));

    std::cout << "Spatial query box: " << bg::wkt(query_box) << std::endl;
    std::cout << "Spatial query result:" << std::endl;
    for (const auto& r: result_s) {
        auto [b, i] = r;
        std::cout << bg::wkt<point>(b) << " -> " << i << std::endl;
    }
}