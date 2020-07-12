// MSG PROC 2020
#include "util/encode_decode.h"
#include "util/buffer.h"
namespace util_ns {
void encode_uint16(util_ns::Buffer *buffer, int offset, uint16_t value) {
  buffer->buffer_.data()[offset + 0] =
      buffer->buffer_.data()[offset + 0] | value & 0x00ff;
  buffer->buffer_.data()[offset + 1] =
      buffer->buffer_.data()[offset + 1] | value >> 8;
}

void encode_uint32(util_ns::Buffer *buffer, int offset, uint32_t value) {
  buffer->buffer_.data()[offset + 0] =
      buffer->buffer_.data()[offset + 0] | value & 0x000000ff;
  buffer->buffer_.data()[offset + 1] =
      buffer->buffer_.data()[offset + 1] | value >> 8;
  buffer->buffer_.data()[offset + 2] =
      buffer->buffer_.data()[offset + 2] | (value & 0x00ff0000) >> 16;
  buffer->buffer_.data()[offset + 3] =
      buffer->buffer_.data()[offset + 3] | (value & 0xff000000) >> 24;
}

uint16_t decode_uint16(util_ns::Buffer *buffer, int offset) {
  uint16_t ret = 0;
  ret |= buffer->buffer_.data()[offset + 0] << 8;
  ret |= buffer->buffer_.data()[offset + 1];
  return ret;
}

uint32_t decode_uint32(util_ns::Buffer *buffer, int offset) {
  uint32_t ret = 0;
  ret |= buffer->buffer_.data()[offset + 0] << 24;
  ret |= buffer->buffer_.data()[offset + 1] << 16;
  ret |= buffer->buffer_.data()[offset + 2] << 8;
  ret |= buffer->buffer_.data()[offset + 3];
  return ret;
}
} // namespace util_ns
