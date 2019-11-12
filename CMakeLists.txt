cmake_minimum_required(VERSION 3.10)
project(boost_geometric)

set(CMAKE_CXX_STANDARD 17)

# MRJ: Replaced boost find_package (note that we need to specify BOTH FLAGS at the same time)
set(BUILD_SHARED_LIBS ON)
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost COMPONENTS  system  filesystem REQUIRED)

# Set link libraries
LIST(APPEND LINK_LIBS
        ${Boost_LIBRARIES})

add_executable(bg_quickstart main_quickstart.cxx)
target_link_libraries(bg_quickstart ${LINK_LIBS})

add_executable(bg_point_query main_point_query.cxx)
target_link_libraries(bg_point_query ${LINK_LIBS})

# 3D queries are not implemented in boost 1.68
#add_executable(bg_3d_point_query main_3d_point_query.cxx)
#target_link_libraries(bg_3d_point_query ${LINK_LIBS})