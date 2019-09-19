set(SOURCE main.c)
find_package(METIS REQUIRED)
include_directories(${METIS_INCLUDE_DIR})
add_executable(metisPerm ${SOURCE})
target_link_libraries(metisPerm ${METIS_LIBRARIES})
target_link_libraries(metisPerm m)

