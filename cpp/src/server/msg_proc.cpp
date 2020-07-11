//  MSG PROC 2020
#include "server/msg_proc.h"
#include "util/buffer.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <sstream>

using server_ns::MsgProc;

MsgProc::MsgProc(util_ns::Buffer in_msg) {
  auto parse_offset{0};
  msg_.header.client_id = ntohl(
      *(reinterpret_cast<uint32_t *>(in_msg.buffer_.data() + parse_offset)));
  parse_offset += sizeof(msg_.header.client_id);
  msg_.header.message_id = ntohl(
      *reinterpret_cast<uint32_t *>((in_msg.buffer_.data() + parse_offset)));
  parse_offset += sizeof(msg_.header.message_id);
  msg_.header.message_len = ntohl(
      *reinterpret_cast<uint16_t *>((in_msg.buffer_.data() + parse_offset)));
  parse_offset += sizeof(msg_.header.message_len);
  msg_.payload = in_msg.buffer_.data() + parse_offset;
}

std::string MsgProc::Stringize() {
  std::stringstream ss;
  ss << msg_.header.client_id << " : " << msg_.header.message_id << " : "
     << std::string(reinterpret_cast<char *>(msg_.payload),
                    msg_.header.message_len)
     << '\n';
  std::cout << msg_.header.client_id << " : " << msg_.header.message_id << '\n';
  return ss.str();
}
