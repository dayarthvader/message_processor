//  MSG PROC 2020
#ifndef MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_MSG_RESP_H_
#define MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_MSG_RESP_H_
#include "interfaces/msg_proc_interface.h"
namespace server_ns {
class MsgResp {
public:
  MsgResp(uint32_t client_id, uint32_t message_id)
      : client_id_(client_id), message_id_(message_id) {}

  uint32_t ClientId() { return client_id_; }
  uint32_t MessageId() { return message_id_; }

private:
  uint32_t client_id_ = -1;
  uint32_t message_id_ = -1;
};
} // namespace server_ns
#endif //  MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_MSG_RESP_H_
