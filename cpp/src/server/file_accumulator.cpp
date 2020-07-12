//  MSG PROC 2020
#include "server/file_accumulator.h"
#include "util/file_handler.h"
#include "util/shared_queue.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
using server_ns::FileAccumulator;
const char FileAccumulator::output[] = "/var/log/msg_proc_output.txt";
FileAccumulator::FileAccumulator(util_ns::SharedQueue<std::string> *writers_q,
                                 const std::string &logging_dir)
    : writers_queue_(writers_q), logging_dir_(logging_dir) {
  ostream_.open(output, std::ios_base::out | std::ios_base::app);
  if (!ostream_.is_open()) {
    ostream_.open(output, std::ios_base::out | std::ios_base::trunc);
  }
  ostream_.rdbuf()->pubsetbuf(0, 0);
  run();
}
void FileAccumulator::accumulate(const std::string &filename) {
  if (!ostream_.is_open()) {
    // Todo log error and exit;
  }
  std::ifstream istream(filename);
  if (istream.is_open()) {
    ostream_ << istream.rdbuf();
  }
  ostream_.flush();
}

void FileAccumulator::run() {
  while (true) {
    std::string in_file{""};
    writers_queue_->FrontAndPop(in_file);
    std::cout << "accumulating: " << in_file << '\n';
    accumulate(in_file);
    util_ns::file_handler_ns::Cleanup(in_file);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }
}

