//
// Created by Miquel Ramirez on 13/11/2019.
//


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Kd_tree.h>
#include <CGAL/algorithm.h>
#include <CGAL/Search_traits_3.h>
#include <CGAL/Search_traits_adapter.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/property_map.h>
#include <CGAL/Fuzzy_sphere.h>

#include <boost/format.hpp>

#include <chrono>
#include <memory>
#include <tuple>
#include <iostream>

using boost::format;

class Info {
public:
    typedef std::shared_ptr<Info>   Ptr;

    double  x{0.0};

    Info(double _x) : x(_x) {}
};

std::ostream& operator<<(std::ostream& os, const Info& i) {
    os << str(format("[x=%1%]") % i.x);
    return os;
}

typedef     CGAL::Simple_cartesian<double> K;
typedef     K::Point_3                      Point;
typedef     std::tuple<Point, Info::Ptr>    Element;

typedef     CGAL::Random_points_in_cube_3<Point>  Random_points_iterator;

typedef CGAL::Search_traits_3<K>                       Traits_base;
typedef CGAL::Search_traits_adapter<Element,
        CGAL::Nth_of_tuple_property_map<0, Element>, Traits_base>   Traits;

typedef CGAL::Fuzzy_sphere<Traits> QuerySphere;
typedef CGAL::Kd_tree<Traits> Tree;
typedef CGAL::Random Random;



int main() {
    // generator for random data points in the square ( (-1,-1), (1,1) )
    Random rnd(0);

    Random_points_iterator rpit( 1.0);

    std::vector<Point>        points;
    std::vector<Info::Ptr>      info;

    points.push_back(Point(*rpit++));
    points.push_back(Point(*rpit++));
    points.push_back(Point(*rpit++));
    points.push_back(Point(*rpit++));
    points.push_back(Point(*rpit++));
    points.push_back(Point(*rpit++));
    points.push_back(Point(*rpit++));

    info.push_back(std::make_shared<Info>(0.0));
    info.push_back(std::make_shared<Info>(1.0));
    info.push_back(std::make_shared<Info>(2.0));
    info.push_back(std::make_shared<Info>(3.0));
    info.push_back(std::make_shared<Info>(4.0));
    info.push_back(std::make_shared<Info>(5.0));
    info.push_back(std::make_shared<Info>(6.0));

    // Insert also the N points in the tree
    Tree tree;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0; i < points.size(); i++) {
        tree.insert(std::make_tuple(points[i], info[i]));
        std::cout << "Point: " << points[i] << std::endl;
    }
    auto tf = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();
    std::cout << str(format("kd-tree construction: %1% us") % elapsed) << std::endl;
    std::cout << "Statistics: ";
    tree.statistics(std::cout) << std::endl;

    // define default circular range query
    double search_radius = 0.2;
    Point center(0.0, 0.0, 0.0);


    std::list<Element> result;
    while (result.empty() && (search_radius <= 10.0)) {
        QuerySphere query(center, search_radius);
        t0 = std::chrono::high_resolution_clock::now();
        tree.search(std::back_inserter(result), query);
        tf = std::chrono::high_resolution_clock::now();

        std::cout << "\nPoints in sphere with center " << center << " and radius " << search_radius << std::endl;
        std::list<Element>::iterator it;
        for (it=result.begin(); (it != result.end()); ++it) {
            auto [pt, info] = *it;
            std::cout << "Point: " << pt << " Info: " << *info << std::endl;
        }

        elapsed = std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();
        std::cout << str(format("Query time: %1% us") % elapsed) << std::endl;

        if (result.empty()) {
            search_radius *= 1.5;
        }
    }


    return 0;
}