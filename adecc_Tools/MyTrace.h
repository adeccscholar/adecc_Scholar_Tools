#pragma once

#if __has_include("MyError.h") 
   #include "MyError.h"
#else
   #include "adecc_Tools/MyError.h"
#endif


#include <iostream>
#include <string>
#include <source_location>
#include <chrono>

using namespace std::string_literals;

#if defined WITH_TRACE
  const bool ShouldTrace = true;
#else
  const bool ShouldTrace = false;
#endif	

inline std::string MyTimeStamp(std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now()) {
   auto const cz_ts = std::chrono::current_zone()->to_local(now);
   auto const millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
   return std::format("{:%d.%m.%Y %X},{:03d}", cz_ts, millis.count());
   }

inline std::string MyPosition(std::source_location loc = std::source_location::current()) {
   return std::format("[in function \"{}\" in file \"{}\" at line {}]", loc.function_name(), loc.file_name(), loc.line());
   }


inline MyErrorInfo&& AddTrace(MyErrorInfo&& err_info, std::string const& text, std::ostream& out = std::cerr,
   std::source_location loc = std::source_location::current(),
   std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now()) {
   get<2>(err_info) += "\n"s + std::format("{}: {} {}\n", MyTimeStamp(now), text, MyPosition(loc));
   return std::move(err_info);
   }

inline MyErrorInfo& AddTrace(MyErrorInfo& err_info, std::string const& text, std::ostream& out = std::cerr,
   std::source_location loc = std::source_location::current(),
   std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now()) {
   get<2>(err_info) += "\n"s + std::format("{}: {} {}\n", MyTimeStamp(now), text, MyPosition(loc));
   return err_info;
}


template <bool boTrace = ShouldTrace>
void Trace(std::string const& text, std::ostream& out = std::cerr, 
           std::source_location loc = std::source_location::current(),
           std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now()) {
	if constexpr (boTrace == true) {
      out << std::format("{}: {} {}\n", MyTimeStamp(now), text, MyPosition(loc));
      }
   }

