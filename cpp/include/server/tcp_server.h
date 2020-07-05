//  MSG PROC 2020
#ifndef MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_TCP_SERVER_H_
#define MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_TCP_SERVER_H_
#include "spdlog/logger.h"
#include "util/connection_info.h"
#include "util/logger.h"
#include "util/shared_queue.h"
#include <memory>
#include <queue>
#include <string>
#include <thread>
namespace server_ns {
class TcpSever {
public:
  TcpSever(const std::string &port,
           util_ns::SharedQueue<util_ns::ConnectionInfo> *job_queue);
  ~TcpSever();
  // TcpSever(TcpSever &) = delete;
  // TcpSever(TcpSever &&) = delete;
  // TcpSever &operator=(const TcpSever &) = delete;
  // TcpSever &operator=(TcpSever &&) = delete;
  void Run();

private:
  void exit_failure(const std::string &);
  void receive_jobs();
  void run();
  void setup();
  void stop();
  std::shared_ptr<spdlog::logger> logger_{nullptr};
  std::thread accept_thread_;
  bool stop_{false};
  std::string port_{""};
  util_ns::SharedQueue<util_ns::ConnectionInfo> *job_queue_;
  struct sockaddr_in server_address_;
  int server_sock_fd_{-1};
};
} //  namespace server_ns
#endif  //  MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_TCP_SERVER_H_
