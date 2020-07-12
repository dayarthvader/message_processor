// MSG PROC 2020
#ifndef MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_FILE_ACCUMULATOR_H_
#define MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_FILE_ACCUMULATOR_H_
#include "util/shared_queue.h"
#include <fstream>
#include <ios>
#include <string>
namespace server_ns {
class FileAccumulator {
public:
  FileAccumulator(util_ns::SharedQueue<std::string> *writers_q,
                  const std::string &logging_dir);

private:
  void run();
  void accumulate(const std::string &);
  void cleanup(const std::string &);
  util_ns::SharedQueue<std::string> *writers_queue_{nullptr};
  std::string logging_dir_{""};
  std::ofstream ostream_;
  static const char output[];
};
} //  namespace server_ns
#endif //  MESSAGE_PROCESSOR_CPP_INCLUDE_SERVER_FILE_ACCUMULATOR_H_
