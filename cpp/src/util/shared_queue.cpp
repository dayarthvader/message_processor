//  MSG PROC 2020
#include "util/shared_queue.h"
using util_ns::SharedQueue;

template <typename T> void SharedQueue<T>::Push(const T &item) {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(item);
  }
  cv_.notify_one();
}

template <typename T> T &SharedQueue<T>::FrontAndPop() {
  T item;
  {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !queue_.empty(); });
    item = queue_.front();
    queue_.pop();
  }
  return item;
}
