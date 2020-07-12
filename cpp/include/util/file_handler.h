//  MSG PROC 2020
#ifndef MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_FILE_HANDLER_H_
#define MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_FILE_HANDLER_H_
#include <string>
namespace util_ns {
namespace file_handler_ns {
bool DirExists(const std::string
                   &filname); //  ToDo (Daya) fix this with proper error codes
bool CreateDir(const std::string &filname);
void Cleanup(const std::string &filename);
} //  namespace file_handler_ns
} //  namespace util_ns
#endif  //  MESSAGE_PROCESSOR_CPP_INCLUDE_UTIL_FILE_HANDLER_H_
