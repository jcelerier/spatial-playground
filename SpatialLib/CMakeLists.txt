project(SpatialLib)

cmake_minimum_required(VERSION 3.2)

set(CMAKE_AUTOMOC ON)
set(CMAKE_BUILD_TYPE Debug)
find_package(Qt5 5.2 REQUIRED COMPONENTS Core Widgets Gui Multimedia )

include_directories(.)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

file(GLOB_RECURSE SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*")
add_executable(firstTest tests/test.cpp ${SRCS})
target_link_libraries(firstTest cln ginac)
target_link_libraries(firstTest Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Multimedia)



