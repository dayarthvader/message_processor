//  MSG PROC 2020
#include "server/worker.h"
#include "server/msg_proc.h"
#include "util/buffer.h"
#include "util/connection_info.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
using server_ns::MsgProc;
using server_ns::MsgResp;
using server_ns::Worker;

void Worker::run() {
  while (true) {
    std::stringstream ss;
    ss << std::this_thread::get_id();

    util_ns::ConnectionInfo client_conn;
    job_queue_->FrontAndPop(client_conn);
    std::string file_name{logging_dir_ + ss.str() + "_" +
                          std::to_string(session_id_++)};

    auto msg_count{0};
    temp_stream_.open(file_name, std::ofstream::binary);
    while (true) {
      auto len = recv(client_conn.socket_fd, buffer_.buffer_.data(),
                      util_ns::kMsgMaxSize, 0);
      if (len == -1 || len == 0) { // end of stream or timeout
        if (temp_stream_.is_open()) {
          temp_stream_.close();
        }
        // enque into the writers queue
        writers_queue_->Push(file_name);
        break;
      }
      buffer_.BuffeLen(len);
      MsgProc req{buffer_};
      temp_stream_.write(
          req.Stringize().c_str(),
          len); //  Might be a bit of overhead to pretty print the message
      msg_count++;
      MsgResp res(req.ClientId(), req.MessageId());
      send_response(client_conn, res);
      if (msg_count >=
          kMsgCountPerSchedule) { // Co-operative yielding if the client is
                                  // greedy. Client will be re-scheduled
        job_queue_->Push(client_conn);
        break;
      }
    }
  }
}

void Worker::send_response(const util_ns::ConnectionInfo &conn_info,
                           MsgResp &resp) {
  util_ns::Buffer send_buffer_; //  Wastage of space, use send specific buffers?
  (*reinterpret_cast<uint32_t *>(send_buffer_.buffer_.data())) =
      htonl(resp.ClientId());
  (*reinterpret_cast<uint32_t *>(send_buffer_.buffer_.data() +
                                 sizeof(uint32_t))) = htonl(resp.MessageId());
  send(conn_info.socket_fd,
       reinterpret_cast<unsigned char *>(send_buffer_.buffer_.data()),
       sizeof(resp), 0); // Todo (Daya) error check and log
}
