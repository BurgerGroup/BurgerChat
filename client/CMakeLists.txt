set(client_SRCS
    interface.cc
)

add_library(client ${client_SRCS})
target_link_libraries(client ${CURSES_LIBRARIES} pthread rt)

add_subdirectory(tests)
