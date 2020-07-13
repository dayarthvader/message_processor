// MSG PROC 2020
#ifndef MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_FILE_ACCUMULATOR_H_
#define MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_FILE_ACCUMULATOR_H_
#include "spdlog/logger.h"
#include "util/logger.h"
#include "util/shared_queue.h"
#include <fstream>
#include <ios>
#include <memory>
#include <string>
namespace server_ns {
class FileAccumulator {
public:
  FileAccumulator(util_ns::SharedQueue<std::string> *writers_q,
                  std::shared_ptr<spdlog::logger>);

private:
  void run();
  void accumulate(const std::string &);
  void cleanup(const std::string &);
  util_ns::SharedQueue<std::string> *writers_queue_{nullptr};
  std::string logging_dir_{""};
  std::ofstream ostream_;
  static const char output[];
  std::shared_ptr<spdlog::logger> logger_{nullptr};
};
} //  namespace server_ns
#endif //  MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_FILE_ACCUMULATOR_H_
