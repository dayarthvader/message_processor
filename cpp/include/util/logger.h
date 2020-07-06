// MSG PROC 2020
#ifndef MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_LOGGER_H_
#define MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_LOGGER_H_
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
namespace util_ns {
namespace logger_ns {
const char server_logger_name[] = "msg_proc_server_log";
const char server_logger_file_name[] = "/var/log/msg_proc_server.log";
const char client_logger_name[] = "msg_proc_client_log";
const char client_logger_file_name[] = "/var/log/msg_proc_client.log";
} // namespace logger_ns
}  // namespace util_ns
#endif  //  MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_LOGGER_H_
