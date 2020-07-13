//  MSG PROC 2020
#ifndef MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_WORKER_H_
#define MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_WORKER_H_
#include "server/msg_resp.h"
#include "util/buffer.h"
#include "util/connection_info.h"
#include "util/shared_queue.h"
#include <fstream>
#include <memory>
#include <string>
#include <thread>
namespace server_ns {
class Worker {
public:
  Worker(util_ns::SharedQueue<util_ns::ConnectionInfo> *job_queue,
         util_ns::SharedQueue<std::string> *writers_queue,
         const std::string &logging_dir)
      : job_queue_(job_queue), writers_queue_(writers_queue),
        logging_dir_(logging_dir),
        buffer_(std::make_unique<util_ns::Buffer>()) {
    run();
  };

private:
  void run();
  void end_processing(const std::string &file_name);
  int send_response(const util_ns::ConnectionInfo &, server_ns::MsgResp &resp);
  util_ns::SharedQueue<util_ns::ConnectionInfo> *job_queue_{nullptr};
  util_ns::SharedQueue<std::string> *writers_queue_{nullptr};
  std::string logging_dir_{""};
  std::ofstream temp_stream_;
  std::unique_ptr<util_ns::Buffer> buffer_{nullptr};
  uint16_t session_id_{0};
  static const int kMsgCountPerSchedule =
      1000; //  Dimention and make it configurable
};
} // namespace server_ns
#endif  //  MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_WORKER_H_
