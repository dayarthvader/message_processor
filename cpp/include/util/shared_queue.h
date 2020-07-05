//  MSG PROC 2020
#ifndef MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_SHARED_QUEUE_H_
#define MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_SHARED_QUEUE_H_
#include <condition_variable>
#include <mutex>
#include <queue>
namespace util_ns {
template<typename T>
class SharedQueue {
public:
  SharedQueue() = default;
  void Push(const T &item) {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      queue_.push(item);
    }
    cv_.notify_one();
  }

  void FrontAndPop(T &item) {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      cv_.wait(lock, [this] { return !queue_.empty(); });
      item = queue_.front();
      queue_.pop();
    }
  }

private:
  std::queue<T> queue_;
  mutable std::mutex mutex_;
  std::condition_variable cv_;
};
}  // namespace util_ns
#endif  //  MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_SHARED_QUEUE_H_
