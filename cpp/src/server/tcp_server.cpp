//  MSG PROC 2020
#include "server/tcp_server.h"
#include "spdlog/logger.h"
#include "util/connection_info.h"
#include "util/logger.h"
#include <iostream>
#include <memory>
#include <sys/socket.h>
#include <thread>

using server_ns::TcpSever;

TcpSever::TcpSever(const std::string &port,
                   util_ns::SharedQueue<util_ns::ConnectionInfo> *job_queue,
                   std::shared_ptr<spdlog::logger> logger)
    : port_(port), job_queue_(job_queue), logger_(logger) {
  if (job_queue_ == nullptr) {
    logger_->error("Job queue invalid");
    exit(1);
  }
  if (logger_ == nullptr) {
    std::cout << "Logger is fucked!\n";
  }
  server_address_.sin_family = AF_INET;
  server_address_.sin_addr.s_addr = INADDR_ANY;
  if (port.empty()) {
    logger_->error("Server Port number is invalid ");
    exit(1);
  }
  server_address_.sin_port = htonl(std::stoi(port));
  setup();
}

TcpSever::~TcpSever() { stop(); }

void TcpSever::exit_failure(const std::string &message) {
  stop();
  logger_->error("{:s}", message);
  logger_->error("{:s}", strerror(errno));
  exit(1);
}

// Called from the destructor for cleanup
// Can be made a public interface by making stop_ atomic bool. signal handling
// etc ...
void TcpSever::stop() {
  stop_ = true;
  if (accept_thread_.joinable()) {
    accept_thread_.join();
  }
}

void TcpSever::Run() {
  logger_->info("Running the server");
  accept_thread_ = std::thread([this] { receive_jobs(); });
  if (accept_thread_.joinable()) {
    accept_thread_.join();
    logger_->info("Server going down");
  }
}

void TcpSever::receive_jobs() {
  logger_->info("TCP server awaiting connections");
  int client_sock_fd{-1};
  struct sockaddr_in client_addr;
  auto client_addr_len{sizeof(client_addr)};
  while (!stop_) {
    client_sock_fd =
        accept(server_sock_fd_, reinterpret_cast<sockaddr *>(&client_addr),
               reinterpret_cast<socklen_t *>(&client_addr_len));
    if (client_sock_fd == -1) {
      logger_->error("Accept failed {:s}", strerror(errno));
      stop_ = true;
    }
    auto local{
        util_ns::ConnectionInfo(client_sock_fd, client_addr, client_addr_len)};
    job_queue_->Push(local);
  }
}

void TcpSever::setup() {
  if ((server_sock_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    exit_failure("Failed allocate server socket");
  }
  auto option{1};
  if (setsockopt(server_sock_fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                 &option, sizeof(option)) == -1) {
    exit_failure("Setting up socket");
  }

  if (bind(server_sock_fd_, reinterpret_cast<sockaddr *>(&server_address_),
           sizeof(server_address_)) == -1) {
    exit_failure("Bind failed");
  }

  if (listen(server_sock_fd_, 5) == -1) {
    exit_failure("Listen failed");
  }
  // This affects the performance, tune the
  // backlog according to deployment
}
