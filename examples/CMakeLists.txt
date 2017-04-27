include_directories ("${PROJECT_SOURCE_DIR}/src")

add_executable(example1 example1.c)
target_link_libraries(example1 lqueue)

add_executable(example2 example2.c)
target_link_libraries(example2 lqueue)
