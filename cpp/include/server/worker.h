//  MSG PROC 2020
#ifndef MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_WORKER_H_
#define MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_WORKER_H_
#include "util/buffer.h"
#include "util/connection_info.h"
#include "util/shared_queue.h"
#include <string>
#include <thread>
namespace server_ns {
class Worker {
public:
  Worker(util_ns::SharedQueue<util_ns::ConnectionInfo> *job_queue,
         util_ns::SharedQueue<std::string> *writers_queue,
         const std::string &logging_dir)
      : job_queue_(job_queue), logging_dir_(logging_dir) {
    run();
  };

private:
  void run();
  util_ns::SharedQueue<util_ns::ConnectionInfo> *job_queue_{nullptr};
  util_ns::SharedQueue<std::string> *writers_queue_{nullptr};
  std::string logging_dir_{""};
  util_ns::Buffer buffer_;
  static const int kMsgCountPerSchedule =
      100; //  Dimention and make it configurable
};
} // namespace server_ns
#endif  //  MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_WORKER_H_
