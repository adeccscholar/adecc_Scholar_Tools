#pragma once

#include <chrono>
#include <string>
#include <format>

inline [[nodiscard]] std::string trim(std::string const& input) {
   auto pos = input.find_first_not_of(" ");
   return input.substr(pos, input.find_last_not_of(" ") - pos + 1);
   }

inline [[nodiscard]] std::string get_current_time_and_date(auto now) {
   auto const timew = std::chrono::current_zone()->to_local(now);
   const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
   return std::format("{:%d.%m.%Y %X},{:03d}", timew, millis.count());
   }

inline [[nodiscard]] std::string get_current_timestamp(auto now) {
   return std::format("{:%Y%m%d%H%M}", std::chrono::current_zone()->to_local(now));
   }

inline [[nodiscard]] std::string get_current_time_and_date(void) {
   return get_current_time_and_date(std::chrono::system_clock::now());
   }

inline [[nodiscard]] std::string get_current_timestamp(void) {
   return get_current_timestamp(std::chrono::system_clock::now());
   }
