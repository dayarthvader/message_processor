// MSG PROC 2020
#ifndef MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_ENCODE_DECODE_H_
#define MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_ENCODE_DECODE_H_
#include "buffer.h"
#include <cstdint>
#include <memory>
namespace util_ns {
void encode_uint32(util_ns::Buffer *buffer, int offset, uint32_t value);
void encode_uint16(util_ns::Buffer *buffer, int offset, uint16_t value);
uint32_t decode_uint32(util_ns::Buffer *buffer, int offset);
uint16_t decode_uint16(util_ns::Buffer *buffer, int offset);
}  //  namespace util_ns
#endif  //  MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_ENCODE_DECODE_H_ 
