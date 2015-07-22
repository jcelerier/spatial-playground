project(SpatialTest)

set(CMAKE_AUTOMOC ON)
set(CMAKE_BUILD_TYPE Debug)
find_package(Qt5 5.2 REQUIRED COMPONENTS Core Widgets Gui Multimedia )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(.)
add_executable(Spatial main.cpp DrawingView.hpp)

target_link_libraries(Spatial Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Multimedia)
