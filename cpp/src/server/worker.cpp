//  MSG PROC 2020
#include "server/worker.h"
#include "util/buffer.h"
#include "util/connection_info.h"
#include <fstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
using server_ns::Worker;

void Worker::run() {
  while (true) {
    util_ns::ConnectionInfo client_conn;
    job_queue_->FrontAndPop(client_conn);
    //  Generate unique_file names in the namespace accomodated by uint32_t and
    //  cycle back
    std::string file_name{logging_dir_ + tid_str_ + '_' +
                          std::to_string(file_id_)};
    file_id_++;
    std::ofstream temp_stream(file_name, std::ofstream::binary);
    while (true) {
      auto len = recv(client_conn.socket_fd, buffer_.buffer_.data(),
                      util_ns::kMsgMaxSize, 0);
      if (len == -1 || len == 0) { // end of stream or timeout
        if (temp_stream.is_open()) {
          temp_stream.close();
        }
        // enque into the writers queue
        writers_queue_->Push(file_name);
        break;
      }
      buffer_.BuffeLen(len);
      temp_stream.write(reinterpret_cast<const char *>(buffer_.buffer_.data()),
                        len);
    }
  }
}
