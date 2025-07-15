cmake_minimum_required(VERSION 3.14)
project(GeometriaComputacional CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(geometria STATIC
  src/Point/Point.cpp
  src/Vector/Vector.cpp
  src/Poligon/Poligon.cpp
  src/ConvexHullStrategy/GiftWrappingAlgorithm/GiftWrappingAlgorithm.cpp
  src/ConvexHullStrategy/DivideAndConquerAlgorithm/DivideAndConquerAlgorithm.cpp
  src/PointGenerationStrategy/HullPercentageStrategy/HullPercentageStrategy.cpp
  src/PointGenerationStrategy/RandomPointStrategy/RandomPointStrategy.cpp
)

target_include_directories(geometria PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(main_app src/main.cpp)

target_link_libraries(main_app PRIVATE geometria)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(test)
