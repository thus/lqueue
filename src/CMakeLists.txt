add_library(lqueue SHARED lqueue.c)
target_link_libraries(lqueue pthread)

install(TARGETS lqueue DESTINATION lib)

install(FILES "${PROJECT_SOURCE_DIR}/src/lqueue.h" DESTINATION include)
