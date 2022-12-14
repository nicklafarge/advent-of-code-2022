add_executable(day13 day13.cpp)
target_include_directories(day13 PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day13 aoc)


add_executable(day13c day13Cheat.cpp)
target_include_directories(day13c PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(day13c aoc)