#pragma once

#include <cstdint>
// Generated from polyhook.pplugin

namespace polyhook {

  // Enum representing different data types in the system.
  enum class DataType : uint8_t {
    Void = 0, // Represents no data type (void).
    Bool = 1, // Represents a boolean data type (true or false).
    Int8 = 2, // Represents an 8-bit signed integer.
    UInt8 = 3, // Represents an 8-bit unsigned integer.
    Int16 = 4, // Represents a 16-bit signed integer.
    UInt16 = 5, // Represents a 16-bit unsigned integer.
    Int32 = 6, // Represents a 32-bit signed integer.
    UInt32 = 7, // Represents a 32-bit unsigned integer.
    Int64 = 8, // Represents a 64-bit signed integer.
    UInt64 = 9, // Represents a 64-bit unsigned integer.
    Float = 10, // Represents a 32-bit floating point number.
    Double = 11, // Represents a 64-bit double precision floating point number.
    Pointer = 12, // Represents a pointer to any type of data.
    String = 13, // Represents a string data type.
  };

  // Enum representing the type of callback.
  enum class CallbackType : uint8_t {
    Pre = 0, // Callback will be executed before the original function
    Post = 1, // Callback will be executed after the original function
  };

  // Enum representing the possible results of an operation.
  enum class ResultType : int32_t {
    Ignored = 0, // Handler didn't take any action.
    Handled = 1, // We did something, but real function should still be called.
    Override = 2, // Call real function, but use my return value.
    Supercede = 3, // Skip real function; use my return value.
  };


  /// Ownership type for RAII wrappers
  enum class Ownership : bool { Borrowed, Owned };

} // namespace polyhook
