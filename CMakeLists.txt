cmake_minimum_required(VERSION 3.10)
project(boost_geometric)

set(CMAKE_CXX_STANDARD 17)

# MRJ: Replaced boost find_package (note that we need to specify BOTH FLAGS at the same time)
set(BUILD_SHARED_LIBS ON)
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS  system  filesystem REQUIRED)
find_package(CGAL)

# Set include directories
LIST(APPEND INCLUDE_DIRS
        ${CGAL_INCLUDE_DIRS})

# Set link libraries
LIST(APPEND LINK_LIBS
        ${Boost_LIBRARIES})

include_directories(${INCLUDE_DIRS})

# compilation flags
add_definitions(
        -DBOOST_SYSTEM_NO_DEPRECATED
        -DCGAL_HEADER_ONLY
)

add_executable(bg_quickstart main_quickstart.cxx)
target_link_libraries(bg_quickstart ${LINK_LIBS})

add_executable(bg_point_query main_point_query.cxx)
target_link_libraries(bg_point_query ${LINK_LIBS})

add_executable(pt_search_circ_query main_pt_search_circular_query.cxx)
target_link_libraries(pt_search_circ_query ${LINK_LIBS})

add_executable(pt_search_circ_query_with_info main_pt_search_with_info.cxx)
target_link_libraries(pt_search_circ_query_with_info ${LINK_LIBS})

# 3D queries are not implemented in boost 1.68
#add_executable(bg_3d_point_query main_3d_point_query.cxx)
#target_link_libraries(bg_3d_point_query ${LINK_LIBS})