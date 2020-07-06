add_library(server STATIC tcp_server.cpp worker.cpp msg_proc.cpp )
target_link_libraries(server util)
