//  MSG PROC 2020
#include "server/file_accumulator.h"
#include "util/file_handler.h"
#include "util/shared_queue.h"
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
using server_ns::FileAccumulator;
const char FileAccumulator::output[] = "/var/log/msg_proc_output.txt";
FileAccumulator::FileAccumulator(util_ns::SharedQueue<std::string> *writers_q,
                                 std::shared_ptr<spdlog::logger> logger)
    : writers_queue_(writers_q), logger_(logger) {
  run();
}
void FileAccumulator::accumulate(const std::string &filename) {
  ostream_.open(output, std::ios_base::out | std::ios_base::app);
  if (!ostream_.is_open()) {
    ostream_.open(output, std::ios_base::out | std::ios_base::trunc);
  }
  ostream_.rdbuf()->pubsetbuf(0, 0);
  if (!ostream_.is_open()) {
    logger_->error("{0:s} is not present", output);
  }
  std::ifstream istream(filename);
  if (istream.is_open()) {
    ostream_ << istream.rdbuf();
  }
  ostream_.close();
}

void FileAccumulator::run() {
  while (true) {
    std::string in_file{""};
    writers_queue_->FrontAndPop(in_file);
    accumulate(in_file);
    util_ns::file_handler_ns::Cleanup(in_file);
  }
}

