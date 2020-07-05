add_library(server STATIC tcp_server.cpp worker.cpp)
target_link_libraries(server util)
