#pragma once

#include <iostream>
#include <string>
#include <source_location>
#include <chrono>

#if defined WITH_TRACE
const bool ShouldTrace = true;
#else
const bool ShouldTrace = false;
#endif	

template <bool boTrace = ShouldTrace>
void Trace(std::string const& text, std::ostream& out = std::cerr, 
           std::source_location loc = std::source_location::current(),
           std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now()) {
	if constexpr (boTrace == true) {
      auto const cz_ts = std::chrono::current_zone()->to_local(now);
      auto const millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
      
      out << std::format("{:%d.%m.%Y %X},{:03d}: {} [in function \"{}\" in file \"{}\" at line {}]\n", 
                          cz_ts, millis.count(), text, loc.function_name(), loc.file_name(), loc.line());
      }
   }

