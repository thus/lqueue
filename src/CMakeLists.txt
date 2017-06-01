# Build shared library
add_library(lqueue SHARED lqueue.c)
target_link_libraries(lqueue pthread)
install(TARGETS lqueue DESTINATION lib)

# Build static library
add_library(lqueue_static STATIC lqueue.c)
target_link_libraries(lqueue_static pthread)
set_target_properties(lqueue_static PROPERTIES OUTPUT_NAME lqueue)
install(TARGETS lqueue_static DESTINATION lib)

install(FILES "${PROJECT_SOURCE_DIR}/src/lqueue.h" DESTINATION include)
