#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

#include "core.hpp"
#include "getters.hpp"
#include "setters.hpp"
#include "lookup.hpp"

// Generated from polyhook.pplugin (group: classes)

namespace polyhook {

  /**
   * @brief RAII wrapper for Callback pointer from hook operations.
   * @return 
   */
  class Callback final {
  public:
    Callback() = default;

    Callback(const Callback&) = default;
    Callback& operator=(const Callback&) = default;
    Callback(Callback&&) noexcept = default;
    Callback& operator=(Callback&&) noexcept = default;
    ~Callback() = default;

    explicit Callback(void* handle) : _handle(handle) {}

    [[nodiscard]] auto get() const noexcept { return _handle; }

    [[nodiscard]] auto release() noexcept {
      auto handle = _handle;
      _handle = nullptr;
      return handle;
    }

    void reset() noexcept {
      _handle = nullptr;
    }

    void swap(Callback& other) noexcept {
      using std::swap;
      swap(_handle, other._handle);
    }

    friend void swap(Callback& lhs, Callback& rhs) noexcept { lhs.swap(rhs); }

    explicit operator bool() const noexcept { return _handle != nullptr; }
    [[nodiscard]] auto operator<=>(const Callback& other) const noexcept { return _handle <=> other._handle; }
    [[nodiscard]] bool operator==(const Callback& other) const noexcept { return _handle == other._handle; }

    /**
     * @brief Adds a callback to existing hook
     * @param type (uint8): Whether the hook was in post mode (after processing) or pre mode (before processing).
     * @param handler (function): Callback function which trigger by hook.
     * @return bool: Returns true on success, false otherwise
     */
    bool AddCallback(CallbackType type, CallbackHandler handler) {
      if (_handle == nullptr) throw std::runtime_error("Callback: empty handle");
      return polyhook::AddCallback(_handle, type, handler);
    }

    /**
     * @brief Adds a callback to existing hook
     * @param type (uint8): Whether the hook was in post mode (after processing) or pre mode (before processing).
     * @param handler (function): Callback function which trigger by hook.
     * @param priority (int32): Priority of callback among others
     * @return bool: Returns true on success, false otherwise
     */
    bool AddCallback2(CallbackType type, CallbackHandler handler, int32_t priority) {
      if (_handle == nullptr) throw std::runtime_error("Callback: empty handle");
      return polyhook::AddCallback2(_handle, type, handler, priority);
    }

    /**
     * @brief Removes a callback from existing hook
     * @param type (uint8): Whether the hook was in post mode (after processing) or pre mode (before processing).
     * @param handler (function): Callback function which trigger by hook.
     * @return bool: Returns true on success, false otherwise
     */
    bool RemoveCallback(CallbackType type, CallbackHandler handler) {
      if (_handle == nullptr) throw std::runtime_error("Callback: empty handle");
      return polyhook::RemoveCallback(_handle, type, handler);
    }

    /**
     * @brief Checks that a callback exists on existing hook
     * @param type (uint8): Whether the hook was in post mode (after processing) or pre mode (before processing).
     * @param handler (function): Callback function which trigger by hook.
     * @return bool: Returns true on success, false otherwise
     */
    bool IsCallbackRegistered(CallbackType type, CallbackHandler handler) {
      if (_handle == nullptr) throw std::runtime_error("Callback: empty handle");
      return polyhook::IsCallbackRegistered(_handle, type, handler);
    }

    /**
     * @brief Checks that a any callback exists on existing hook
     * @return bool: Returns true on success, false otherwise
     */
    bool AreCallbacksRegistered() {
      if (_handle == nullptr) throw std::runtime_error("Callback: empty handle");
      return polyhook::AreCallbacksRegistered(_handle);
    }

    /**
     * @brief Get generated function address
     * @return ptr64: Returns jit generated function pointer
     */
    void* GetFunctionAddr() {
      if (_handle == nullptr) throw std::runtime_error("Callback: empty handle");
      return polyhook::GetFunctionAddr(_handle);
    }

    /**
     * @brief Get original function address
     * @return ptr64: Returns original function pointer
     */
    void* GetOriginalAddr() {
      if (_handle == nullptr) throw std::runtime_error("Callback: empty handle");
      return polyhook::GetOriginalAddr(_handle);
    }

  private:
    void* _handle{nullptr};
  };


  /**
   * @brief RAII wrapper for Callback::Parameters pointer used in callback handlers.
   * @return 
   */
  class Parameters final {
  public:
    Parameters() = default;

    Parameters(const Parameters&) = default;
    Parameters& operator=(const Parameters&) = default;
    Parameters(Parameters&&) noexcept = default;
    Parameters& operator=(Parameters&&) noexcept = default;
    ~Parameters() = default;

    explicit Parameters(void* handle) : _handle(handle) {}

    [[nodiscard]] auto get() const noexcept { return _handle; }

    [[nodiscard]] auto release() noexcept {
      auto handle = _handle;
      _handle = nullptr;
      return handle;
    }

    void reset() noexcept {
      _handle = nullptr;
    }

    void swap(Parameters& other) noexcept {
      using std::swap;
      swap(_handle, other._handle);
    }

    friend void swap(Parameters& lhs, Parameters& rhs) noexcept { lhs.swap(rhs); }

    explicit operator bool() const noexcept { return _handle != nullptr; }
    [[nodiscard]] auto operator<=>(const Parameters& other) const noexcept { return _handle <=> other._handle; }
    [[nodiscard]] bool operator==(const Parameters& other) const noexcept { return _handle == other._handle; }

    /**
     * @brief Get argument value
     * @param index (uint64): Index to get
     * @return bool
     */
    bool GetBool(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      return polyhook::GetArgumentBool(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Index to get
     * @return int8
     */
    int8_t GetInt8(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      return polyhook::GetArgumentInt8(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Index to get
     * @return uint8
     */
    uint8_t GetUInt8(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      return polyhook::GetArgumentUInt8(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Index to get
     * @return int16
     */
    int16_t GetInt16(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      return polyhook::GetArgumentInt16(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Index to get
     * @return uint16
     */
    uint16_t GetUInt16(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      return polyhook::GetArgumentUInt16(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Index to get
     * @return int32
     */
    int32_t GetInt32(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      return polyhook::GetArgumentInt32(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Index to get
     * @return uint32
     */
    uint32_t GetUInt32(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      return polyhook::GetArgumentUInt32(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Index to get
     * @return int64
     */
    int64_t GetInt64(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      return polyhook::GetArgumentInt64(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Index to get
     * @return uint64
     */
    uint64_t GetUInt64(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      return polyhook::GetArgumentUInt64(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Index to get
     * @return float
     */
    float GetFloat(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      return polyhook::GetArgumentFloat(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Index to get
     * @return double
     */
    double GetDouble(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      return polyhook::GetArgumentDouble(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Index to get
     * @return ptr64
     */
    void* GetPointer(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      return polyhook::GetArgumentPointer(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Index to get
     * @return string
     */
    plg::string GetString(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      return polyhook::GetArgumentString(_handle, index);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Index to set
     * @param value (bool): Value to set
     */
    void SetBool(uint64_t index, bool value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      polyhook::SetArgumentBool(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Index to set
     * @param value (int8): Value to set
     */
    void SetInt8(uint64_t index, int8_t value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      polyhook::SetArgumentInt8(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Index to set
     * @param value (uint8): Value to set
     */
    void SetUInt8(uint64_t index, uint8_t value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      polyhook::SetArgumentUInt8(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Index to set
     * @param value (int16): Value to set
     */
    void SetInt16(uint64_t index, int16_t value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      polyhook::SetArgumentInt16(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Index to set
     * @param value (uint16): Value to set
     */
    void SetUInt16(uint64_t index, uint16_t value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      polyhook::SetArgumentUInt16(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Index to set
     * @param value (int32): Value to set
     */
    void SetInt32(uint64_t index, int32_t value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      polyhook::SetArgumentInt32(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Index to set
     * @param value (uint32): Value to set
     */
    void SetUInt32(uint64_t index, uint32_t value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      polyhook::SetArgumentUInt32(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Index to set
     * @param value (int64): Value to set
     */
    void SetInt64(uint64_t index, int64_t value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      polyhook::SetArgumentInt64(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Index to set
     * @param value (uint64): Value to set
     */
    void SetUInt64(uint64_t index, uint64_t value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      polyhook::SetArgumentUInt64(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Index to set
     * @param value (float): Value to set
     */
    void SetFloat(uint64_t index, float value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      polyhook::SetArgumentFloat(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Index to set
     * @param value (double): Value to set
     */
    void SetDouble(uint64_t index, double value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      polyhook::SetArgumentDouble(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Index to set
     * @param value (ptr64): Value to set
     */
    void SetPointer(uint64_t index, void* value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: empty handle");
      polyhook::SetArgumentPointer(_handle, index, value);
    }

  private:
    void* _handle{nullptr};
  };


  /**
   * @brief RAII wrapper for Callback::Return pointer used in callback handlers.
   * @return 
   */
  class Return final {
  public:
    Return() = default;

    Return(const Return&) = default;
    Return& operator=(const Return&) = default;
    Return(Return&&) noexcept = default;
    Return& operator=(Return&&) noexcept = default;
    ~Return() = default;

    explicit Return(void* handle) : _handle(handle) {}

    [[nodiscard]] auto get() const noexcept { return _handle; }

    [[nodiscard]] auto release() noexcept {
      auto handle = _handle;
      _handle = nullptr;
      return handle;
    }

    void reset() noexcept {
      _handle = nullptr;
    }

    void swap(Return& other) noexcept {
      using std::swap;
      swap(_handle, other._handle);
    }

    friend void swap(Return& lhs, Return& rhs) noexcept { lhs.swap(rhs); }

    explicit operator bool() const noexcept { return _handle != nullptr; }
    [[nodiscard]] auto operator<=>(const Return& other) const noexcept { return _handle <=> other._handle; }
    [[nodiscard]] bool operator==(const Return& other) const noexcept { return _handle == other._handle; }

    /**
     * @brief Get return value
     * @return bool
     */
    bool GetBool() {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      return polyhook::GetReturnBool(_handle);
    }

    /**
     * @brief Get return value
     * @return int8
     */
    int8_t GetInt8() {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      return polyhook::GetReturnInt8(_handle);
    }

    /**
     * @brief Get return value
     * @return uint8
     */
    uint8_t GetUInt8() {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      return polyhook::GetReturnUInt8(_handle);
    }

    /**
     * @brief Get return value
     * @return int16
     */
    int16_t GetInt16() {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      return polyhook::GetReturnInt16(_handle);
    }

    /**
     * @brief Get return value
     * @return uint16
     */
    uint16_t GetUInt16() {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      return polyhook::GetReturnUInt16(_handle);
    }

    /**
     * @brief Get return value
     * @return int32
     */
    int32_t GetInt32() {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      return polyhook::GetReturnInt32(_handle);
    }

    /**
     * @brief Get return value
     * @return uint32
     */
    uint32_t GetUInt32() {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      return polyhook::GetReturnUInt32(_handle);
    }

    /**
     * @brief Get return value
     * @return int64
     */
    int64_t GetInt64() {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      return polyhook::GetReturnInt64(_handle);
    }

    /**
     * @brief Get return value
     * @return uint64
     */
    uint64_t GetUInt64() {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      return polyhook::GetReturnUInt64(_handle);
    }

    /**
     * @brief Get return value
     * @return float
     */
    float GetFloat() {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      return polyhook::GetReturnFloat(_handle);
    }

    /**
     * @brief Get return value
     * @return double
     */
    double GetDouble() {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      return polyhook::GetReturnDouble(_handle);
    }

    /**
     * @brief Get return value
     * @return ptr64
     */
    void* GetPointer() {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      return polyhook::GetReturnPointer(_handle);
    }

    /**
     * @brief Get return value
     * @return string
     */
    plg::string GetString() {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      return polyhook::GetReturnString(_handle);
    }

    /**
     * @brief Set return value
     * @param value (bool): Value to set
     */
    void SetBool(bool value) {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      polyhook::SetReturnBool(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (int8): Value to set
     */
    void SetInt8(int8_t value) {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      polyhook::SetReturnInt8(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (uint8): Value to set
     */
    void SetUInt8(uint8_t value) {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      polyhook::SetReturnUInt8(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (int16): Value to set
     */
    void SetInt16(int16_t value) {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      polyhook::SetReturnInt16(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (uint16): Value to set
     */
    void SetUInt16(uint16_t value) {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      polyhook::SetReturnUInt16(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (int32): Value to set
     */
    void SetInt32(int32_t value) {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      polyhook::SetReturnInt32(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (uint32): Value to set
     */
    void SetUInt32(uint32_t value) {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      polyhook::SetReturnUInt32(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (int64): Value to set
     */
    void SetInt64(int64_t value) {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      polyhook::SetReturnInt64(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (uint64): Value to set
     */
    void SetUInt64(uint64_t value) {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      polyhook::SetReturnUInt64(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (float): Value to set
     */
    void SetFloat(float value) {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      polyhook::SetReturnFloat(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (double): Value to set
     */
    void SetDouble(double value) {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      polyhook::SetReturnDouble(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (ptr64): Value to set
     */
    void SetPointer(void* value) {
      if (_handle == nullptr) throw std::runtime_error("Return: empty handle");
      polyhook::SetReturnPointer(_handle, value);
    }

  private:
    void* _handle{nullptr};
  };


  /**
   * @brief RAII wrapper for Callback::Registers pointer used in callback handlers.
   * @return 
   */
  class Registers final {
  public:
    Registers() = default;

    Registers(const Registers&) = default;
    Registers& operator=(const Registers&) = default;
    Registers(Registers&&) noexcept = default;
    Registers& operator=(Registers&&) noexcept = default;
    ~Registers() = default;

    explicit Registers(void* handle) : _handle(handle) {}

    [[nodiscard]] auto get() const noexcept { return _handle; }

    [[nodiscard]] auto release() noexcept {
      auto handle = _handle;
      _handle = nullptr;
      return handle;
    }

    void reset() noexcept {
      _handle = nullptr;
    }

    void swap(Registers& other) noexcept {
      using std::swap;
      swap(_handle, other._handle);
    }

    friend void swap(Registers& lhs, Registers& rhs) noexcept { lhs.swap(rhs); }

    explicit operator bool() const noexcept { return _handle != nullptr; }
    [[nodiscard]] auto operator<=>(const Registers& other) const noexcept { return _handle <=> other._handle; }
    [[nodiscard]] bool operator==(const Registers& other) const noexcept { return _handle == other._handle; }

    /**
     * @brief Get register value
     * @param reg (uint64): Register to get
     * @return bool
     */
    bool GetBool(RegisterType reg) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      return polyhook::GetRegisterBool(_handle, reg);
    }

    /**
     * @brief Get register value
     * @param reg (uint64): Register to get
     * @return int8
     */
    int8_t GetInt8(RegisterType reg) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      return polyhook::GetRegisterInt8(_handle, reg);
    }

    /**
     * @brief Get register value
     * @param reg (uint64): Register to get
     * @return uint8
     */
    uint8_t GetUInt8(RegisterType reg) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      return polyhook::GetRegisterUInt8(_handle, reg);
    }

    /**
     * @brief Get register value
     * @param reg (uint64): Register to get
     * @return int16
     */
    int16_t GetInt16(RegisterType reg) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      return polyhook::GetRegisterInt16(_handle, reg);
    }

    /**
     * @brief Get register value
     * @param reg (uint64): Register to get
     * @return uint16
     */
    uint16_t GetUInt16(RegisterType reg) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      return polyhook::GetRegisterUInt16(_handle, reg);
    }

    /**
     * @brief Get register value
     * @param reg (uint64): Register to get
     * @return int32
     */
    int32_t GetInt32(RegisterType reg) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      return polyhook::GetRegisterInt32(_handle, reg);
    }

    /**
     * @brief Get register value
     * @param reg (uint64): Register to get
     * @return uint32
     */
    uint32_t GetUInt32(RegisterType reg) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      return polyhook::GetRegisterUInt32(_handle, reg);
    }

    /**
     * @brief Get register value
     * @param reg (uint64): Register to get
     * @return int64
     */
    int64_t GetInt64(RegisterType reg) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      return polyhook::GetRegisterInt64(_handle, reg);
    }

    /**
     * @brief Get register value
     * @param reg (uint64): Register to get
     * @return uint64
     */
    uint64_t GetUInt64(RegisterType reg) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      return polyhook::GetRegisterUInt64(_handle, reg);
    }

    /**
     * @brief Get register value
     * @param reg (uint64): Register to get
     * @return float
     */
    float GetFloat(RegisterType reg) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      return polyhook::GetRegisterFloat(_handle, reg);
    }

    /**
     * @brief Get register value
     * @param reg (uint64): Register to get
     * @return double
     */
    double GetDouble(RegisterType reg) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      return polyhook::GetRegisterDouble(_handle, reg);
    }

    /**
     * @brief Get register value
     * @param reg (uint64): Register to get
     * @return ptr64
     */
    void* GetPointer(RegisterType reg) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      return polyhook::GetRegisterPointer(_handle, reg);
    }

    /**
     * @brief Get register value
     * @param reg (uint64): Register to get
     * @return string
     */
    plg::string GetString(RegisterType reg) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      return polyhook::GetRegisterString(_handle, reg);
    }

    /**
     * @brief Set register value
     * @param reg (uint64): Register to set
     * @param value (bool): Value to set
     */
    void SetBool(RegisterType reg, bool value) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      polyhook::SetRegisterBool(_handle, reg, value);
    }

    /**
     * @brief Set register value
     * @param reg (uint64): Register to set
     * @param value (int8): Value to set
     */
    void SetInt8(RegisterType reg, int8_t value) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      polyhook::SetRegisterInt8(_handle, reg, value);
    }

    /**
     * @brief Set register value
     * @param reg (uint64): Register to set
     * @param value (uint8): Value to set
     */
    void SetUInt8(RegisterType reg, uint8_t value) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      polyhook::SetRegisterUInt8(_handle, reg, value);
    }

    /**
     * @brief Set register value
     * @param reg (uint64): Register to set
     * @param value (int16): Value to set
     */
    void SetInt16(RegisterType reg, int16_t value) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      polyhook::SetRegisterInt16(_handle, reg, value);
    }

    /**
     * @brief Set register value
     * @param reg (uint64): Register to set
     * @param value (uint16): Value to set
     */
    void SetUInt16(RegisterType reg, uint16_t value) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      polyhook::SetRegisterUInt16(_handle, reg, value);
    }

    /**
     * @brief Set register value
     * @param reg (uint64): Register to set
     * @param value (int32): Value to set
     */
    void SetInt32(RegisterType reg, int32_t value) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      polyhook::SetRegisterInt32(_handle, reg, value);
    }

    /**
     * @brief Set register value
     * @param reg (uint64): Register to set
     * @param value (uint32): Value to set
     */
    void SetUInt32(RegisterType reg, uint32_t value) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      polyhook::SetRegisterUInt32(_handle, reg, value);
    }

    /**
     * @brief Set register value
     * @param reg (uint64): Register to set
     * @param value (int64): Value to set
     */
    void SetInt64(RegisterType reg, int64_t value) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      polyhook::SetRegisterInt64(_handle, reg, value);
    }

    /**
     * @brief Set register value
     * @param reg (uint64): Register to set
     * @param value (uint64): Value to set
     */
    void SetUInt64(RegisterType reg, uint64_t value) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      polyhook::SetRegisterUInt64(_handle, reg, value);
    }

    /**
     * @brief Set register value
     * @param reg (uint64): Register to set
     * @param value (float): Value to set
     */
    void SetFloat(RegisterType reg, float value) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      polyhook::SetRegisterFloat(_handle, reg, value);
    }

    /**
     * @brief Set register value
     * @param reg (uint64): Register to set
     * @param value (double): Value to set
     */
    void SetDouble(RegisterType reg, double value) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      polyhook::SetRegisterDouble(_handle, reg, value);
    }

    /**
     * @brief Set register value
     * @param reg (uint64): Register to set
     * @param value (ptr64): Value to set
     */
    void SetPointer(RegisterType reg, void* value) {
      if (_handle == nullptr) throw std::runtime_error("Registers: empty handle");
      polyhook::SetRegisterPointer(_handle, reg, value);
    }

  private:
    void* _handle{nullptr};
  };


  /**
   * @brief Global hooking functions for PolyHook plugin.
   * @return 
   */
  class PolyHook final {
  public:
    /**
     * @brief Sets a detour hook
     * @param pFunc (ptr64): Function address
     * @param returnType (uint8): Return type
     * @param arguments (uint8[]): Arguments type array
     * @param varIndex (int32): Index of a first variadic argument or -1
     * @return ptr64: Returns hook pointer
     */
    static Callback HookDetour(void* pFunc, DataType returnType, const plg::vector<DataType>& arguments, int32_t varIndex = -1) {
      return Callback(polyhook::HookDetour(pFunc, returnType, arguments, varIndex));
    }

    /**
     * @brief Sets a mid hook
     * @param pFunc (ptr64): Function address
     * @return ptr64: Returns hook pointer
     */
    static Callback HookDetour2(void* pFunc) {
      return Callback(polyhook::HookDetour2(pFunc));
    }

    /**
     * @brief Removes a detour hook
     * @param pFunc (ptr64): Function address
     * @return bool: Returns true on success, false otherwise
     */
    static bool UnhookDetour(void* pFunc) {
      return polyhook::UnhookDetour(pFunc);
    }

    /**
     * @brief Sets a virtual table hook
     * @param pClass (ptr64): Object pointer
     * @param index (int32): Vtable offset
     * @param returnType (uint8): Return type
     * @param arguments (uint8[]): Arguments type array
     * @param varIndex (int32): Index of a first variadic argument or -1
     * @return ptr64: Returns hook pointer
     */
    static Callback HookVirtualTable(void* pClass, int32_t index, DataType returnType, const plg::vector<DataType>& arguments, int32_t varIndex = -1) {
      return Callback(polyhook::HookVirtualTable(pClass, index, returnType, arguments, varIndex));
    }

    /**
     * @brief Sets a virtual table hook by pointer
     * @param pClass (ptr64): Object pointer
     * @param pFunc (ptr64): Vtable member function address
     * @param returnType (uint8): Return type
     * @param arguments (uint8[]): Arguments type array
     * @param varIndex (int32): Index of a first variadic argument or -1
     * @return ptr64: Returns hook pointer
     */
    static Callback HookVirtualTableByFunc(void* pClass, void* pFunc, DataType returnType, const plg::vector<DataType>& arguments, int32_t varIndex = -1) {
      return Callback(polyhook::HookVirtualTable2(pClass, pFunc, returnType, arguments, varIndex));
    }

    /**
     * @brief Removes a virtual hook table
     * @param pClass (ptr64): Object pointer
     * @param index (int32): Virtual table index
     * @return bool: Returns true on success, false otherwise
     */
    static bool UnhookVirtualTable(void* pClass, int32_t index) {
      return polyhook::UnhookVirtualTable(pClass, index);
    }

    /**
     * @brief Removes a virtual table hook by pointer
     * @param pClass (ptr64): Object pointer
     * @param pFunc (ptr64): Vtable member function address
     * @return bool: Returns true on success, false otherwise
     */
    static bool UnhookVirtualTableByFunc(void* pClass, void* pFunc) {
      return polyhook::UnhookVirtualTable2(pClass, pFunc);
    }

    /**
     * @brief Sets a virtual function hook
     * @param pClass (ptr64): Object pointer
     * @param index (int32): Vtable offset
     * @param returnType (uint8): Return type
     * @param arguments (uint8[]): Arguments type array
     * @param varIndex (int32): Index of a first variadic argument or -1
     * @return ptr64: Returns hook pointer
     */
    static Callback HookVirtualFunc(void* pClass, int32_t index, DataType returnType, const plg::vector<DataType>& arguments, int32_t varIndex = -1) {
      return Callback(polyhook::HookVirtualFunc(pClass, index, returnType, arguments, varIndex));
    }

    /**
     * @brief Sets a virtual function hook by pointer
     * @param pClass (ptr64): Object pointer
     * @param pFunc (ptr64): Vtable member function address
     * @param returnType (uint8): Return type
     * @param arguments (uint8[]): Arguments type array
     * @param varIndex (int32): Index of a first variadic argument or -1
     * @return ptr64: Returns hook pointer
     */
    static Callback HookVirtualFuncByFunc(void* pClass, void* pFunc, DataType returnType, const plg::vector<DataType>& arguments, int32_t varIndex = -1) {
      return Callback(polyhook::HookVirtualFunc2(pClass, pFunc, returnType, arguments, varIndex));
    }

    /**
     * @brief Removes a virtual function table
     * @param pClass (ptr64): Object pointer
     * @param index (int32): Virtual table index
     * @return bool: Returns true on success, false otherwise
     */
    static bool UnhookVirtualFunc(void* pClass, int32_t index) {
      return polyhook::UnhookVirtualFunc(pClass, index);
    }

    /**
     * @brief Removes a virtual function hook by pointer
     * @param pClass (ptr64): Object pointer
     * @param pFunc (ptr64): Vtable member function address
     * @return bool: Returns true on success, false otherwise
     */
    static bool UnhookVirtualFuncByFunc(void* pClass, void* pFunc) {
      return polyhook::UnhookVirtualFunc2(pClass, pFunc);
    }

    /**
     * @brief Attempts to find existing detour hook
     * @param pFunc (ptr64): Function address
     * @return ptr64: Returns hook pointer
     */
    static Callback FindDetour(void* pFunc) {
      return Callback(polyhook::FindDetour(pFunc));
    }

    /**
     * @brief Attempts to find existing virtual hook
     * @param pClass (ptr64): Object pointer
     * @param index (int32): Virtual table index
     * @return ptr64: Returns hook pointer
     */
    static Callback FindVirtual(void* pClass, int32_t index) {
      return Callback(polyhook::FindVirtual(pClass, index));
    }

    /**
     * @brief Attempts to find existing detour hook by pointer
     * @param pClass (ptr64): Object pointer
     * @param pFunc (ptr64): Function address
     * @return ptr64: Returns hook pointer
     */
    static Callback FindVirtualByFunc(void* pClass, void* pFunc) {
      return Callback(polyhook::FindVirtual2(pClass, pFunc));
    }

    /**
     * @brief Attempts to find virtual table index of virtual function
     * @param pFunc (ptr64): Function address
     * @return int32: Virtual table index
     */
    static int32_t GetVirtualIndex(void* pFunc) {
      return polyhook::GetVirtualIndex(pFunc);
    }

    /**
     * @brief Removes all previously set hooks
     */
    static void UnhookAll() {
      polyhook::UnhookAll();
    }

    /**
     * @brief Removes all previously set hooks on the object
     * @param pClass (ptr64): Object pointer
     */
    static void UnhookAllVirtual(void* pClass) {
      polyhook::UnhookAllVirtual(pClass);
    }

  };


} // namespace polyhook
