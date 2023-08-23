#pragma once

#include <string>
#include <utility>

enum class EMyErrorType : int { noError = 0, Userbreak, InputError, FileError, unknown = 999 };

using MyErrorInfo = std::pair<EMyErrorType, std::string>;