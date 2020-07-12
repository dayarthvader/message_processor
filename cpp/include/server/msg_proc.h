//  MSG PROC 2020
#ifndef MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_MSG_PROC_H_
#define MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_MSG_PROC_H_
#include "interfaces/msg_proc_interface.h"
#include "util/buffer.h"
#include <memory>
#include <string>
namespace server_ns {
class MsgProc {
public:
  MsgProc(util_ns::Buffer *);
  std::string Stringize();
  uint32_t ClientId() { return msg_.header.client_id; }
  uint32_t MessageId() { return msg_.header.message_id; }

private:
  util_ns::Buffer *buffer_{nullptr};
  interfaces_ns::MsgProc msg_;
};
} //  namespace server_ns
#endif  //  MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_MSG_PROC_H_
