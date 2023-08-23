#pragma once
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <codecvt>
#include <locale>

#include <string_view>
#include <optional>
#include <iterator>
#include <atomic>
#include <fstream>
#include <stdexcept>
#include <filesystem>

using namespace std::literals;


struct call_with_string {
   using my_string = std::string;
   using my_file   = std::ifstream;
   using my_view   = std::string_view;
};

struct call_with_wstring {
   using my_string = std::wstring;
   using my_file = std::wifstream;
   using my_view = std::wstring_view;
};

template <typename ty>
struct is_my_string_param {
   static constexpr bool value = std::is_same<std::remove_cvref_t<ty>, call_with_string>::value ||
      std::is_same<std::remove_cvref_t<ty>, call_with_wstring>::value;
};


template <typename ty>
concept my_string_param = is_my_string_param<ty>::value;

// nur nutzen, wenn nicht parallel
template <my_string_param ty = call_with_string>
inline auto GetContent(std::filesystem::path const& strFile, typename ty::my_string& strBuffer) {
   typename ty::my_file ifs(strFile);
   ifs.exceptions(std::ios_base::badbit);
   if (!ifs.is_open()) [[unlikely]] throw std::runtime_error("file \""s + strFile.string() + "\" can't opened"s);
   else {
      if constexpr (std::is_same<ty, call_with_string>::value) {
         const auto iSize = std::filesystem::file_size(strFile);
         strBuffer.resize(iSize);
         ifs.read(strBuffer.data(), iSize);
         //return std::string_view { strBuffer.data(), strBuffer.size() };
         }
      else if constexpr (std::is_same<ty, call_with_wstring>::value) {
         //std::wstring strLine = L"";
         std::locale loc(std::locale::empty(), new std::codecvt_utf8<wchar_t>);
         ifs.imbue(loc);
         //while (std::getline(ifs, strLine)) strBuffer += strLine + L"\n";
         std::getline(ifs, strBuffer, L'\0');
         //return std::wstring_view { strBuffer.data(), strBuffer.size() };
         }
      else static_assert_no_supported();
      }
   return typename ty::my_view { strBuffer.data(), strBuffer.size() };
   }

template <my_string_param ty = call_with_string>
struct my_line_iterator {
   using iterator_category = std::input_iterator_tag;
   using value_type = typename ty::my_view;
   using difference_type = std::ptrdiff_t;
   using reference_type = const value_type&;
   using pointer_type = const value_type*;

   my_line_iterator() { };
   my_line_iterator(typename ty::my_view const& input) : theText(input) { ++* this; };

   my_line_iterator& operator = (typename ty::my_view const& vw) {
      theText = vw;
      start_pos = 0u;
      end_pos = 0u;
      return *this;
   }

   my_line_iterator& operator = (my_line_iterator const& ref) {
      theText = ref.theText;
      theLine = ref.theLine;
      start_pos = ref.start_pos;
      end_pos = ref.end_pos;
      return *this;
   }

   reference_type operator*() const { return theLine; }
   pointer_type operator->() const { return &theLine; }

   my_line_iterator& operator++() {
      if (theText) {
         end_pos = theText->find('\n', start_pos);
         if (end_pos != ty::my_view::npos) {
            theLine = theText->substr(start_pos, end_pos - start_pos);
            start_pos = end_pos + 1;
         }
         else {
            //theText = { };
            theText = std::nullopt;
         }
      }
      return *this;
   }

   my_line_iterator operator++(int) {
      auto elem(*this);
      ++* this;
      return elem;
   }

   friend  bool operator == (my_line_iterator const& lhs, my_line_iterator const& rhs) {
      return lhs.theText == rhs.theText;
   }

   friend  bool operator != (my_line_iterator const& lhs, my_line_iterator const& rhs) {
      return !(lhs == rhs);
   }

private:
   std::optional<typename ty::my_view> theText = { };
   typename ty::my_view theLine;
   size_t start_pos = 0u, end_pos = 0u;
};

template <my_string_param ty = call_with_string>
struct my_lines {
   my_lines(typename ty::my_view const& input) { theText = input;  }
   my_lines(my_lines const& ref) { theText = ref.theText;  }

   my_line_iterator<ty> begin() const { return my_line_iterator<ty>(theText); }
   my_line_iterator<ty> end() const { return my_line_iterator<ty>(); }


   typename ty::my_view theText;

};

template <my_string_param ty = call_with_string>
struct my_line {
   my_line(void) : view() { }
   my_line(typename ty::my_view const& input) : view(input) { }
   typename ty::my_view view;
};

template <my_string_param ty = call_with_string>
struct my_line_count {

   my_line_count(void) : view(), index(counter++) { }
   my_line_count(typename ty::my_view const& input) : view(input), index(counter++) { }

   typename ty::my_view view;
   int index;

   static inline std::atomic<int> counter = 0;
   static int GetCounter(void) { return counter;  }
   static void reset(void) { counter = 0; }
};

