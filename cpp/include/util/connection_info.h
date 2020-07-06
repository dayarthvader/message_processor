//  MSG PROC 2020
#ifndef MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_CONNECTION_INFO_H_
#define MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_CONNECTION_INFO_H_
#include <netinet/in.h>
namespace util_ns {
struct ConnectionInfo {
  ConnectionInfo(int fd, struct sockaddr_in addr, int len)
      : socket_fd(fd), address(addr), addrlen(len) {}
  ConnectionInfo() = default;
  int socket_fd;
  struct sockaddr_in address;
  int addrlen;
};
}  //  namespace util_ns
#endif  //  MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_CONNECTION_INFO_H_
