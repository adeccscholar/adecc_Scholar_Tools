﻿#pragma once

#include <string>
#include <tuple>

/// enum with error types for MyErrorInfo
enum class EMyErrorType : int { noError = 0,   ///< no error 
                                NotSupported,  ///< the operation isn't supported
                                Userbreak,     ///< user canceled the function
                                InputError,    ///< input error of the user
                                FileError,     ///< error in a file operation 
                                DatabaseError, ///< error in the context of the database
                                RuntimeError,  ///< unspecific runtime error
                                unknown = 999  ///< unknown or unexpected error 
                               };

/// type for unexpected returns of functions
using MyErrorInfo = std::tuple<EMyErrorType, std::string, std::string>;
