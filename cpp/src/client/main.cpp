/// MSG PROC 2020
#include "interfaces/msg_proc_interface.h"
#include "util/buffer.h"
#include "util/encode_decode.h"
#include "util/logger.h"
#include <arpa/inet.h>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <sys/socket.h>

void print_help() {
  std::cout << "./client -n<number of messages> -c <client id> -s <server ip> "
               "-p <server port>\n";
  exit(0);
}

int main(int argc, char **argv) {
  uint32_t num_msg{0};
  uint32_t client_id = -1;
  uint32_t message_id = -1;
  std::string server_ip{""};
  auto server_port{0};
  auto opt{-1};
  auto client_fd{0};
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;

  if (argc < 5) {
    print_help();
  }

  while ((opt = getopt(argc, argv, "n:c:s:p:")) != -1) {
    switch (opt) {
    case -1:
      break;
    case 'n':
      num_msg = atoi(optarg);
      continue;
    case 'c':
      client_id = atoi(optarg);
      continue;
    case 's':
      server_ip = optarg;
      continue;
    case 'p':
      server_port = atoi(optarg);
      continue;
    default:
      print_help();
    }
  }
  if ((client_id == -1) || (num_msg == 0)) {
    print_help();
  }
  if (server_ip.empty() || (server_port == 0)) {
    print_help();
  }

  auto filelog =
      spdlog::basic_logger_mt(util_ns::logger_ns::client_logger_name,
                              util_ns::logger_ns::client_logger_file_name +
                                  std::string(".") + std::to_string(client_id));
  spdlog::set_default_logger(filelog);
  spdlog::flush_on(spdlog::level::info);
  serv_addr.sin_port = htons(server_port);
  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    filelog->error("{0:d} socket {1:s}", client_id, strerror(errno));
    exit(1);
  }
  if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
    filelog->error("{0:d} Server address {1:s}", client_id, strerror(errno));
    exit(1);
  }
  // measure connect time and log
  auto connect_begin = std::chrono::steady_clock::now();
  if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
      0) {
    filelog->error("{0:d} Connect {1:s}", client_id, strerror(errno));
    exit(1);
  }
  auto connect_end = std::chrono::steady_clock::now();
  auto connec_time = std::chrono::duration_cast<std::chrono::microseconds>(
                         connect_end - connect_begin)
                         .count();
  filelog->info("{0:d} connect time us {1:d}", client_id, connec_time);
  const char payload[] = "Message processing client message";
  uint16_t payload_len{static_cast<uint16_t>(std::strlen(payload))};
  std::unique_ptr<util_ns::Buffer> send_buffer =
      std::make_unique<util_ns::Buffer>();
  auto total_length{sizeof(interfaces_ns::MsgProcHeader) + payload_len};
  util_ns::Buffer recv_buff;
  // measure average RTT and log
  message_id = 0;
  uint32_t max_rtt{0};
  for (int i = 0; i < num_msg; i++) {
    std::memset(send_buffer->buffer_.data(), 0, util_ns::kMsgMaxSize);
    util_ns::encode_uint32(send_buffer.get(), 0, (client_id));
    util_ns::encode_uint32(send_buffer.get(), sizeof(uint32_t), (i));
    util_ns::encode_uint16(send_buffer.get(), sizeof(uint32_t) * 2,
                           static_cast<uint16_t>(payload_len));
    std::memcpy(send_buffer->buffer_.data() +
                    sizeof(interfaces_ns::MsgProcHeader),
                payload, payload_len);
    auto send_time = std::chrono::steady_clock::now();
    // filelog->info("send_time {0:d}", send_time.time_since_epoch().count());
    if (send(client_fd, send_buffer->buffer_.data(), total_length, 0) == -1) {
      filelog->error("Send failed {0:s} exiting", strerror(errno));
      // handle reattempts based on errorno
    }
    if (recv(client_fd, recv_buff.buffer_.data(), util_ns::kMsgMaxSize, 0) ==
        -1) {
      filelog->error("Recv failed {0:s} exiting", strerror(errno));
      // handle reattempts based on errorno
    }
    auto recv_time = std::chrono::steady_clock::now();
    auto rtt{std::chrono::duration_cast<std::chrono::microseconds>(recv_time -
                                                                   send_time)
                 .count()};
    if (rtt > max_rtt) {
      max_rtt = rtt;
    }
    //    filelog->info("recv_time {0:d} max_rtt {1:d}",
    //                 recv_time.time_since_epoch().count(), max_rtt);
    // filelog->info("{0:s}", recv_buff.buffer_.data());
  }
  filelog->info("{0:d} max_rtt us {1:d}", client_id, max_rtt);
  shutdown(client_id, 2);
  std::cout << client_id << ',' << connec_time << ',' << max_rtt << '\n';
  return 0;
  }
