//  MSG PROC 2020
#ifndef MESSAGE_PROCESSOR_CPP_INCLUDE_INTERFACES_MSG_PROC_INTERFACE_H_
#define MESSAGE_PROCESSOR_CPP_INCLUDE_INTERFACES_MSG_PROC_INTERFACE_H_
#include <cstdint>
namespace interfaces_ns {
typedef struct MsgProcHeader {
  uint32_t client_id;
  uint32_t message_id;
  uint16_t message_len;
} MsgProcHeader;

typedef struct MsgProc {
  MsgProcHeader header;
  unsigned char *payload; //  Server API supports max 256 bytes PDU
} MsgProc;

typedef struct MsgResp {
  uint32_t client_id;
  uint32_t message_id;
} MsgResp;
} // namespace interfaces_ns
#endif  //  MESSAGE_PROCESSOR_CPP_INCLUDE_INTERFACES_MSG_PROC_INTERFACE_H_
