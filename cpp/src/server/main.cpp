// MSG PROC 2020
#include "server/tcp_server.h"
#include "util/connection_info.h"
#include "util/logger.h"
#include "util/shared_queue.h"
#include <memory>

using server_ns::TcpSever;
using util_ns::ConnectionInfo;
using util_ns::SharedQueue;

int main() {
  auto filelog =
      spdlog::basic_logger_mt(util_ns::logger_ns::server_logger_name,
                              util_ns::logger_ns::server_logger_file_name);
  auto logger{spdlog::get(util_ns::logger_ns::server_logger_name)};
  auto job_queue{std::make_unique<SharedQueue<ConnectionInfo>>()};
  auto serving_port{"14000"};
  TcpSever(serving_port, job_queue.get()).Run();
  return 0;
}
