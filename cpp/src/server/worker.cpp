//  MSG PROC 2020
#include "server/worker.h"
#include "util/buffer.h"
#include "util/connection_info.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
using server_ns::Worker;

void Worker::run() {
  while (true) {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    std::string file_name{logging_dir_ + ss.str()};

    util_ns::ConnectionInfo client_conn;
    job_queue_->FrontAndPop(client_conn);

    auto msg_count{0};
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
      msg_count++;
      if (msg_count >=
          kMsgCountPerSchedule) { // Co-operative yielding if the client is
                                  // greedy. Client will be re-scheduled
        job_queue_->Push(client_conn);
        break;
      }
    }
  }
}
