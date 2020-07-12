//  MSG PROC 2020
#include "util/file_handler.h"
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

namespace util_ns {
namespace file_handler_ns {
bool DirExists(const std::string &filname) {
  return (fs::exists(filname) && fs::is_directory(filname));
}

bool CreateDir(const std::string &filname) {
  return fs::create_directory(filname);
}

void Cleanup(const std::string &filename) {
  try {
    if (fs::remove(filename)) {
    } else {
      // log error Todo
    }
  } catch (const fs::filesystem_error &err) {
    // log exception
  }
}
} //  namespace file_handler_ns
} //  namespace util_ns
