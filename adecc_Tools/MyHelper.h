#pragma once

#include <chrono>
#include <string>
#include <string_view>
#include <format>

[[nodiscard]] inline std::string CompilerVersion() {
   #ifdef _MSC_VER
      return std::format("Visual Studio Version {}", _MSC_VER);
   #elif defined __GNUC__
      return std::format("GCC Version {}.{}.{}", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
   #elif defined __clang__
      return std::format("Clang Version {}.{}.{}", __clang_major__, __clang_minor__, __clang_patchlevel__);
   #else
      return "Unknown compiler";
   #endif
   }

[[nodiscard]] inline std::string trim(std::string const& input) {
   auto pos = input.find_first_not_of(" ");
   return input.substr(pos, input.find_last_not_of(" ") - pos + 1);
   }

[[nodiscard]] inline std::string get_current_time_and_date(auto now) {
   auto const timew = std::chrono::current_zone()->to_local(now);
   const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
   return std::format("{:%d.%m.%Y %X},{:03d}", timew, millis.count());
   }

[[nodiscard]] inline std::string get_current_date_and_time_short(auto now) {
   auto const timew = std::chrono::current_zone()->to_local(now);
   return std::format("{:%d.%m.%Y %H:%M:%S}", timew);
}

[[nodiscard]] inline std::string get_current_timestamp(auto now) {
   return std::format("{:%Y%m%d%H%M}", std::chrono::current_zone()->to_local(now));
   }

[[nodiscard]] inline std::string get_current_time_and_date(void) {
   return get_current_time_and_date(std::chrono::system_clock::now());
   }

[[nodiscard]] inline std::string get_current_date_and_time_short() {
   auto now = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());

   return get_current_date_and_time_short(now);
   }


[[nodiscard]] inline std::string get_current_timestamp(void) {
   return get_current_timestamp(std::chrono::system_clock::now());
   }
