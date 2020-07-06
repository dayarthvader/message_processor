//  MSG PROC 2020
#ifndef MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_BUFFER_H_
#define MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_BUFFER_H_
#include <array>
#include <iostream>
namespace util_ns {
static const int kMsgMaxSize =
    256; //  Todo (Daya) dimension and make it tunable
struct Buffer {
  std::array<unsigned char, kMsgMaxSize> buffer_;
  int buff_len_{0};
  void BuffeLen(int len) {
    if (len > kMsgMaxSize || len < 1) {
      std::cout << "Bad buffer len! " << len << " exiting\n";
      exit(1);
    }
      buff_len_ = len;
  }
};
} //  namespace util_ns
#endif  //  MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_BUFFER_H_
