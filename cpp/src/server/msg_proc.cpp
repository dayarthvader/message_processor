//  MSG PROC 2020
#include "server/msg_proc.h"
#include "interfaces/msg_proc_interface.h"
#include "util/buffer.h"
#include "util/encode_decode.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>

using server_ns::MsgProc;

MsgProc::MsgProc(util_ns::Buffer *in_msg) : buffer_(in_msg) {
  msg_.header.client_id = ntohl(util_ns::decode_uint32(buffer_, 0));
  msg_.header.message_id =
      ntohl(util_ns::decode_uint32(buffer_, sizeof(msg_.header.client_id)));
  msg_.header.message_len = ntohs(util_ns::decode_uint16(
      buffer_, sizeof(msg_.header.client_id) + sizeof(msg_.header.message_id)));
  msg_.payload = buffer_->buffer_.data() + sizeof(msg_.header);
}

std::string MsgProc::Stringize() {
  std::stringstream ss;
  ss << msg_.header.client_id << " : " << msg_.header.message_id << " : "
     << std::string(reinterpret_cast<char *>(msg_.payload),
                    msg_.header.message_len)
     << '\n';
  return ss.str();
}
