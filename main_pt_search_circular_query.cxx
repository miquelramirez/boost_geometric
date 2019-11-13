//
// Created by Miquel Ramirez on 13/11/2019.
//

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Kd_tree.h>
#include <CGAL/Search_traits_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/algorithm.h>
#include <CGAL/Fuzzy_sphere.h>

#include <boost/format.hpp>

#include <chrono>

typedef CGAL::Simple_cartesian<double>  K;
typedef K::Point_2                      Point;
typedef CGAL::Random_points_in_square_2<Point> Random_points_iterator;
typedef CGAL::Counting_iterator<Random_points_iterator> N_Random_points_iterator;
typedef CGAL::Search_traits_2<K> Traits;
typedef CGAL::Fuzzy_sphere<Traits> QuerySphere;
typedef CGAL::Kd_tree<Traits> Tree;
typedef CGAL::Random Random;

using boost::format;

int main() {
    const int N = 1000;

    auto t0 = std::chrono::high_resolution_clock::now();

    // generator for random data points in the square ( (-1,-1), (1,1) )
    Random rnd(0);
    Random_points_iterator rpit(1.0, rnd);

    // Insert also the N points in the tree
    Tree tree(N_Random_points_iterator(rpit,0),
              N_Random_points_iterator(N));
    auto tf = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();
    std::cout << str(format("kd-tree construction: %1% us") % elapsed) << std::endl;


    // define default circular range query
    Point center(0.2, 0.2);
    QuerySphere default_range(center, 0.2);

    t0 = std::chrono::high_resolution_clock::now();
    boost::optional<Point> any = tree.search_any_point(default_range);
    tf = std::chrono::high_resolution_clock::now();

    if(any){
        std::cout << *any << " is in the query circle\n";
    }else{
        std::cout << "Empty query circle\n";
    }
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();
    std::cout << str(format("Query time: %1% us") % elapsed) << std::endl;

    std::list<Point> result;
    t0 = std::chrono::high_resolution_clock::now();
    tree.search(std::back_inserter(result), default_range);
    tf = std::chrono::high_resolution_clock::now();

    std::cout << "\nPoints in circle with center " << center << " and radius 0.2" << std::endl;
    std::list<Point>::iterator it;
    for (it=result.begin(); (it != result.end()); ++it) {
        std::cout << *it << std::endl;
    }
    result.clear();
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();
    std::cout << str(format("Query time: %1% us") % elapsed) << std::endl;

    // approximate range searching using value 0.1 for fuzziness parameter
    QuerySphere approximate_range(center, 0.2, 0.1);

    t0 = std::chrono::high_resolution_clock::now();
    tree.search(std::back_inserter( result ), approximate_range);
    tf = std::chrono::high_resolution_clock::now();

    std::cout << "\n\nPoints in cirle with center " << center << " and fuzzy radius [0.1,0.3]" << std::endl;
    for (it=result.begin(); (it != result.end()); ++it) {
        std::cout << *it << std::endl;
    }
    std::cout << "\ndone" << std::endl;
    elapsed = std::chrono::duration_cast<std::chrono::microseconds>(tf - t0).count();
    std::cout << str(format("Query time: %1% us") % elapsed) << std::endl;

    return 0;
}