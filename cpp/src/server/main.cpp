// MSG PROC 2020
#include "server/file_accumulator.h"
#include "server/tcp_server.h"
#include "server/worker.h"
#include "util/connection_info.h"
#include "util/file_handler.h"
#include "util/logger.h"
#include "util/shared_queue.h"
#include <memory>
#include <thread>
#include <vector>

using server_ns::FileAccumulator;
using server_ns::TcpSever;
using server_ns::Worker;
using util_ns::ConnectionInfo;
using util_ns::SharedQueue;

int main() {
  auto filelog =
      spdlog::basic_logger_mt(util_ns::logger_ns::server_logger_name,
                              util_ns::logger_ns::server_logger_file_name);
  spdlog::set_default_logger(filelog);
  spdlog::flush_on(spdlog::level::info);
  auto logger{spdlog::get(util_ns::logger_ns::server_logger_name)};
  std::string log_dir = "/var/log/msg_proc_temp/";

  if (!util_ns::file_handler_ns::DirExists(log_dir)) {
    logger->info("creating {0:s}", log_dir);
    if (!util_ns::file_handler_ns::CreateDir(log_dir)) {
      logger->error("couldn't create {0:s}", log_dir);
      exit(1);
    }
  } else {
    logger->info("{0:s} exists", log_dir);
  }

  logger->info("Setting up pipeline");

  auto job_queue{std::make_unique<SharedQueue<ConnectionInfo>>()};
  auto writers_queue{std::make_unique<SharedQueue<std::string>>()};
  std::thread file_accumulator_thread{
      [&] { FileAccumulator file_accumulator(writers_queue.get(), logger); }};

  auto max_threads{std::thread::hardware_concurrency()};
  logger->info("Setting up platform supported {0:d} workers", max_threads);
  std::vector<std::thread> thread_pool;
  for (auto i = 0; i < max_threads; i++) {
    thread_pool.push_back(std::thread(
        [&] { Worker(job_queue.get(), writers_queue.get(), log_dir); }));
  }

  auto serving_port{14000};
  TcpSever tcp_server(serving_port, job_queue.get(), logger);
  tcp_server.Run();
  // Todo (Daya)
  // The below thread cleanup should ideally go as signal handler for
  // graceful shutdown also add stop signal to idividual threads
  if (file_accumulator_thread.joinable()) {
    file_accumulator_thread.join();
  }
  for (auto &thread : thread_pool) {
    if (thread.joinable()) {
      thread.join();
    }
  }
  return 0;
}
