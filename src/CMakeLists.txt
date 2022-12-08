add_library(aoc STATIC util.cpp)
target_include_directories(aoc PUBLIC "${CMAKE_SOURCE_DIR}/include")

add_subdirectory(day7)
