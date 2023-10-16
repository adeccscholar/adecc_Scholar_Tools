#pragma once

#include <stdexcept>
#include <string>
#include <string_view>
#include <chrono>
#include <optional>
#include <source_location>

using src_loc    = std::source_location;
using time_stamp = std::chrono::time_point<std::chrono::system_clock>;


namespace std {

   template <>
   struct formatter<std::source_location> : formatter<std::string_view> {
      template <typename FormatContext>
      auto format(const std::source_location& loc, FormatContext& ctx) {
         std::string_view format_str = "{}:{}:{}:{}";
         return formatter<std::string_view>::format(format_str, loc.file_name(), loc.line(), loc.column(), loc.function_name(), ctx);
      }
   };

}


class TMyExceptionInformation {
   friend void swap(TMyExceptionInformation& lhs, TMyExceptionInformation& rhs) noexcept { lhs.swap(rhs); }
private:
   time_stamp the_timepoint;
   src_loc    the_location;
public:
   TMyExceptionInformation(time_stamp const& timept = std::chrono::system_clock::now(), src_loc const& loc = src_loc::current()) :
      the_timepoint(timept), the_location(loc) {}

   TMyExceptionInformation(TMyExceptionInformation const& ref) :
      the_timepoint(ref.the_timepoint), the_location(ref.the_location) {}

   TMyExceptionInformation(TMyExceptionInformation&& ref) noexcept { swap(ref); }

   virtual ~TMyExceptionInformation() = default;

   void swap(TMyExceptionInformation& ref) noexcept {
      using std::swap;
      swap(the_timepoint, ref.the_timepoint);
      swap(the_location, ref.the_location);
      }

   time_stamp const& timepoint(void) const { return the_timepoint; }
   src_loc    const& location(void) const { return the_location; };


   std::string TimePosition() const { return TimePosition(the_timepoint, the_location); }
   std::string Time() const { return Time(the_timepoint); }
   std::string Position() const { return Position(the_location); }


   static std::string TimePosition(time_stamp const& _time, src_loc const& loc) {
      auto const cz_ts = std::chrono::current_zone()->to_local(_time);
      auto const millis = std::chrono::duration_cast<std::chrono::milliseconds>(_time.time_since_epoch()) % 1000;
      return std::format("[Error at {:%d.%m.%Y %X},{:03d} in file \"{}\" at line {}]", cz_ts, millis.count(), loc.file_name(), loc.line());
      }

   static std::string Time(time_stamp const& _time) {
      auto const cz_ts = std::chrono::current_zone()->to_local(_time);
      auto const millis = std::chrono::duration_cast<std::chrono::milliseconds>(_time.time_since_epoch()) % 1000;
      return std::format("{:%d.%m.%Y %X},{:03d}", cz_ts, millis.count());
      }

   static std::string Position(src_loc const& loc) {
      return std::format("in file \"{}\" at line {}", loc.file_name(), loc.line());
      }

};

class TMy_InputError : public std::runtime_error {
public:
   TMy_InputError(std::string const& msg) : std::runtime_error(msg) { }
};

class TMy_UserBreak : public std::runtime_error {
public:
   TMy_UserBreak(std::string const& msg) : std::runtime_error(msg) { }
};

template <typename ty>
class TMy_StandardError : public TMyExceptionInformation, public ty {
private:
   mutable std::string strMessage; ///< Hilfsviable um Speicherdauer der Rückgabe sicherzustellen
public:
   TMy_StandardError(std::string strMsg, src_loc const& loc = src_loc::current(), 
                     time_stamp timepoint = std::chrono::system_clock::now()) :
         TMyExceptionInformation(timepoint, loc), 
         ty(strMsg) { }
   TMy_StandardError(TMy_StandardError const& ref) : TMyExceptionInformation(ref), ty(ref) { }

   const char* what() const noexcept override {
      strMessage = std::format("{}\n{}", typename ty::what(), TimePosition());
      return strMessage.c_str();
      }
};

using TMy_RuntimeError = TMy_StandardError<std::runtime_error>;
