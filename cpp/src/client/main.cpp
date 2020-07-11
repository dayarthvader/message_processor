// MSG PROC 2020
#include "interfaces/msg_proc_interface.h"
#include "util/buffer.h"
#include "util/logger.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <getopt.h>
#include <iostream>
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
                              util_ns::logger_ns::client_logger_file_name);
  spdlog::set_default_logger(filelog);
  spdlog::flush_on(spdlog::level::info);
  serv_addr.sin_port = htons(server_port);
  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    filelog->error("Socket creation failed {0:s} exiting", strerror(errno));
    exit(1);
  }
  if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
    filelog->error("Server Ip address error {0:s} exiting", strerror(errno));
    exit(1);
  }
  // measure connect time and log
  if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
      0) {
    filelog->error("Connect failed {0:s} exiting", strerror(errno));
    exit(1);
  }
  const char payload[] = "Old McDonald had a farm. eeya eeya yo!";
  auto payload_len{std::strlen(&payload[0])};
  util_ns::Buffer send_buffer;

  *reinterpret_cast<uint32_t *>(send_buffer.buffer_.data()) = htonl(client_id);
  *reinterpret_cast<uint32_t *>(send_buffer.buffer_.data() + sizeof(client_id) +
                                sizeof(message_id)) = htons(payload_len);
  std::memcpy(send_buffer.buffer_.data() + sizeof(interfaces_ns::MsgProcHeader),
              payload, payload_len);
  auto total_length{sizeof(interfaces_ns::MsgProcHeader::client_id) +
                    sizeof(interfaces_ns::MsgProcHeader::message_id) +
                    sizeof(interfaces_ns::MsgProcHeader::message_len) +
                    payload_len};
  util_ns::Buffer recv_buff;
  // measure average RTT and log
  for (int i = 0; i < num_msg; i++) {
    *reinterpret_cast<uint32_t *>(
        send_buffer.buffer_.data() +
        sizeof(interfaces_ns::MsgProcHeader::client_id)) = htonl(message_id++);
    if (send(client_fd, send_buffer.buffer_.data(), total_length, 0) == -1) {
      filelog->error("Send failed {0:s} exiting", strerror(errno));
      // handle reattempts based on errorno
    }
    if (recv(client_fd, recv_buff.buffer_.data(), util_ns::kMsgMaxSize, 0) ==
        -1) {
      filelog->error("Recv failed {0:s} exiting", strerror(errno));
      // handle reattempts based on errorno
    }
    filelog->info("{0:s}", recv_buff.buffer_.data());
  }
  return 0;
}
