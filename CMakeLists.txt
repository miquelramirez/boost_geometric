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

add_executable(boost_geometric main.cpp)
target_link_libraries(boost_geometric ${LINK_LIBS})