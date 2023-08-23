#pragma once

#include <string>
#include <utility>

/// enum with error types for MyErrorInfo
enum class EMyErrorType : int { noError = 0,   ///< no error 
                                Userbreak,     ///< user canceled the function
                                InputError,    ///< input error of the user
                                FileError,     ///< error in a file operation 
                                DatabaseError, ///< error in the context of the database
                                RuntimeError,  ///< unspecific runtime error
                                unknown = 999  ///< unknown or unexpected error 
                               };

/// type for unexpected returns of functions
using MyErrorInfo = std::pair<EMyErrorType, std::string>;
