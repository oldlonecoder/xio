

add_executable(devprog.app
    devprogapp.c++
)

target_link_libraries(devprog.app xio logbook chrtools)

