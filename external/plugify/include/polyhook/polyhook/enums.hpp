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

  // Enum representing register storage offsets based on RegisterType layout (x64)
  enum class RegisterType : uint64_t {
    XMM0 = 0, // XMM register 0.
    XMM1 = 2, // XMM register 1.
    XMM2 = 4, // XMM register 2.
    XMM3 = 6, // XMM register 3.
    XMM4 = 8, // XMM register 4.
    XMM5 = 10, // XMM register 5.
    XMM6 = 12, // XMM register 6.
    XMM7 = 14, // XMM register 7.
    XMM8 = 16, // XMM register 8.
    XMM9 = 18, // XMM register 9.
    XMM10 = 20, // XMM register 10.
    XMM11 = 22, // XMM register 11.
    XMM12 = 24, // XMM register 12.
    XMM13 = 26, // XMM register 13.
    XMM14 = 28, // XMM register 14.
    XMM15 = 30, // XMM register 15.
    R15 = 32, // General-purpose register R15.
    R14 = 33, // General-purpose register R14.
    R13 = 34, // General-purpose register R13.
    R12 = 35, // General-purpose register R12.
    R11 = 36, // General-purpose register R11.
    R10 = 37, // General-purpose register R10.
    R9 = 38, // General-purpose register R9.
    R8 = 39, // General-purpose register R8.
    RDI = 40, // Destination Index register.
    RSI = 41, // Source Index register.
    RBP = 42, // Base Pointer register.
    RBX = 43, // Base register.
    RDX = 44, // Data register.
    RCX = 45, // Counter register.
    RAX = 46, // Accumulator register.
    RFLAGS = 47, // CPU flags register.
    COUNT = 48, // Number of register entries.
  };


  /// Ownership type for RAII wrappers
  enum class Ownership : bool { Borrowed, Owned };

} // namespace polyhook
