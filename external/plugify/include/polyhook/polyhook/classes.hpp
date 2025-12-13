#pragma once

#include "enums.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

#include "core.hpp"
#include "getters.hpp"
#include "setters.hpp"

// Generated from polyhook.pplugin (group: classes)

namespace polyhook {

  /**
   * @brief RAII wrapper for Callback pointer from hook operations.
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
      if (_handle == nullptr) throw std::runtime_error("Callback: Empty handle");
      return polyhook::AddCallback(_handle, type, handler);
    }

    /**
     * @brief Adds a callback to existing hook
     * @param type (uint8): Whether the hook was in post mode (after processing) or pre mode (before processing).
     * @param handler (function): Callback function which trigger by hook.
     * @param priority (int32): Priority of callback among others
     * @return bool: Returns true on success, false otherwise
     */
    bool AddCallbackWithPriority(CallbackType type, CallbackHandler handler, int32_t priority) {
      if (_handle == nullptr) throw std::runtime_error("Callback: Empty handle");
      return polyhook::AddCallback2(_handle, type, handler, priority);
    }

    /**
     * @brief Removes a callback from existing hook
     * @param type (uint8): Whether the hook was in post mode (after processing) or pre mode (before processing).
     * @param handler (function): Callback function which trigger by hook.
     * @return bool: Returns true on success, false otherwise
     */
    bool RemoveCallback(CallbackType type, CallbackHandler handler) {
      if (_handle == nullptr) throw std::runtime_error("Callback: Empty handle");
      return polyhook::RemoveCallback(_handle, type, handler);
    }

    /**
     * @brief Checks that a callback exists on existing hook
     * @param type (uint8): Whether the hook was in post mode (after processing) or pre mode (before processing).
     * @param handler (function): Callback function which trigger by hook.
     * @return bool: Returns true on success, false otherwise
     */
    bool IsCallbackRegistered(CallbackType type, CallbackHandler handler) {
      if (_handle == nullptr) throw std::runtime_error("Callback: Empty handle");
      return polyhook::IsCallbackRegistered(_handle, type, handler);
    }

    /**
     * @brief Checks that a any callback exists on existing hook
     * @return bool: Returns true on success, false otherwise
     */
    bool AreCallbacksRegistered() {
      if (_handle == nullptr) throw std::runtime_error("Callback: Empty handle");
      return polyhook::AreCallbacksRegistered(_handle);
    }

    /**
     * @brief Get generated function address
     * @return ptr64: Returns jit generated function pointer
     */
    void* GetFunctionAddr() {
      if (_handle == nullptr) throw std::runtime_error("Callback: Empty handle");
      return polyhook::GetFunctionAddr(_handle);
    }

    /**
     * @brief Get original function address
     * @return ptr64: Returns original function pointer
     */
    void* GetOriginalAddr() {
      if (_handle == nullptr) throw std::runtime_error("Callback: Empty handle");
      return polyhook::GetOriginalAddr(_handle);
    }

  private:
    void* _handle{nullptr};
  };


  /**
   * @brief RAII wrapper for Callback::Parameters pointer used in callback handlers.
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
     * @param index (uint64): Value to set
     * @return bool
     */
    bool GetBool(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      return polyhook::GetArgumentBool(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Value to set
     * @return int8
     */
    int8_t GetInt8(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      return polyhook::GetArgumentInt8(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Value to set
     * @return uint8
     */
    uint8_t GetUInt8(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      return polyhook::GetArgumentUInt8(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Value to set
     * @return int16
     */
    int16_t GetInt16(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      return polyhook::GetArgumentInt16(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Value to set
     * @return uint16
     */
    uint16_t GetUInt16(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      return polyhook::GetArgumentUInt16(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Value to set
     * @return int32
     */
    int32_t GetInt32(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      return polyhook::GetArgumentInt32(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Value to set
     * @return uint32
     */
    uint32_t GetUInt32(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      return polyhook::GetArgumentUInt32(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Value to set
     * @return int64
     */
    int64_t GetInt64(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      return polyhook::GetArgumentInt64(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Value to set
     * @return uint64
     */
    uint64_t GetUInt64(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      return polyhook::GetArgumentUInt64(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Value to set
     * @return float
     */
    float GetFloat(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      return polyhook::GetArgumentFloat(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Value to set
     * @return double
     */
    double GetDouble(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      return polyhook::GetArgumentDouble(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Value to set
     * @return ptr64
     */
    void* GetPointer(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      return polyhook::GetArgumentPointer(_handle, index);
    }

    /**
     * @brief Get argument value
     * @param index (uint64): Value to set
     * @return string
     */
    plg::string GetString(uint64_t index) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      return polyhook::GetArgumentString(_handle, index);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Value to set
     * @param value (bool): Value to set
     */
    void SetBool(uint64_t index, bool value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      polyhook::SetArgumentBool(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Value to set
     * @param value (int8): Value to set
     */
    void SetInt8(uint64_t index, int8_t value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      polyhook::SetArgumentInt8(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Value to set
     * @param value (uint8): Value to set
     */
    void SetUInt8(uint64_t index, uint8_t value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      polyhook::SetArgumentUInt8(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Value to set
     * @param value (int16): Value to set
     */
    void SetInt16(uint64_t index, int16_t value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      polyhook::SetArgumentInt16(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Value to set
     * @param value (uint16): Value to set
     */
    void SetUInt16(uint64_t index, uint16_t value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      polyhook::SetArgumentUInt16(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Value to set
     * @param value (int32): Value to set
     */
    void SetInt32(uint64_t index, int32_t value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      polyhook::SetArgumentInt32(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Value to set
     * @param value (uint32): Value to set
     */
    void SetUInt32(uint64_t index, uint32_t value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      polyhook::SetArgumentUInt32(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Value to set
     * @param value (int64): Value to set
     */
    void SetInt64(uint64_t index, int64_t value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      polyhook::SetArgumentInt64(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Value to set
     * @param value (uint64): Value to set
     */
    void SetUInt64(uint64_t index, uint64_t value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      polyhook::SetArgumentUInt64(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Value to set
     * @param value (float): Value to set
     */
    void SetFloat(uint64_t index, float value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      polyhook::SetArgumentFloat(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Value to set
     * @param value (double): Value to set
     */
    void SetDouble(uint64_t index, double value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      polyhook::SetArgumentDouble(_handle, index, value);
    }

    /**
     * @brief Set argument value
     * @param index (uint64): Value to set
     * @param value (ptr64): Value to set
     */
    void SetPointer(uint64_t index, void* value) {
      if (_handle == nullptr) throw std::runtime_error("Parameters: Empty handle");
      polyhook::SetArgumentPointer(_handle, index, value);
    }

  private:
    void* _handle{nullptr};
  };


  /**
   * @brief RAII wrapper for Callback::Return pointer used in callback handlers.
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
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      return polyhook::GetReturnBool(_handle);
    }

    /**
     * @brief Get return value
     * @return int8
     */
    int8_t GetInt8() {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      return polyhook::GetReturnInt8(_handle);
    }

    /**
     * @brief Get return value
     * @return uint8
     */
    uint8_t GetUInt8() {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      return polyhook::GetReturnUInt8(_handle);
    }

    /**
     * @brief Get return value
     * @return int16
     */
    int16_t GetInt16() {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      return polyhook::GetReturnInt16(_handle);
    }

    /**
     * @brief Get return value
     * @return uint16
     */
    uint16_t GetUInt16() {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      return polyhook::GetReturnUInt16(_handle);
    }

    /**
     * @brief Get return value
     * @return int32
     */
    int32_t GetInt32() {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      return polyhook::GetReturnInt32(_handle);
    }

    /**
     * @brief Get return value
     * @return uint32
     */
    uint32_t GetUInt32() {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      return polyhook::GetReturnUInt32(_handle);
    }

    /**
     * @brief Get return value
     * @return int64
     */
    int64_t GetInt64() {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      return polyhook::GetReturnInt64(_handle);
    }

    /**
     * @brief Get return value
     * @return uint64
     */
    uint64_t GetUInt64() {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      return polyhook::GetReturnUInt64(_handle);
    }

    /**
     * @brief Get return value
     * @return float
     */
    float GetFloat() {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      return polyhook::GetReturnFloat(_handle);
    }

    /**
     * @brief Get return value
     * @return double
     */
    double GetDouble() {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      return polyhook::GetReturnDouble(_handle);
    }

    /**
     * @brief Get return value
     * @return ptr64
     */
    void* GetPointer() {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      return polyhook::GetReturnPointer(_handle);
    }

    /**
     * @brief Get return value
     * @return string
     */
    plg::string GetString() {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      return polyhook::GetReturnString(_handle);
    }

    /**
     * @brief Set return value
     * @param value (bool): Value to set
     */
    void SetBool(bool value) {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      polyhook::SetReturnBool(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (int8): Value to set
     */
    void SetInt8(int8_t value) {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      polyhook::SetReturnInt8(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (uint8): Value to set
     */
    void SetUInt8(uint8_t value) {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      polyhook::SetReturnUInt8(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (int16): Value to set
     */
    void SetInt16(int16_t value) {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      polyhook::SetReturnInt16(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (uint16): Value to set
     */
    void SetUInt16(uint16_t value) {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      polyhook::SetReturnUInt16(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (int32): Value to set
     */
    void SetInt32(int32_t value) {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      polyhook::SetReturnInt32(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (uint32): Value to set
     */
    void SetUInt32(uint32_t value) {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      polyhook::SetReturnUInt32(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (int64): Value to set
     */
    void SetInt64(int64_t value) {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      polyhook::SetReturnInt64(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (uint64): Value to set
     */
    void SetUInt64(uint64_t value) {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      polyhook::SetReturnUInt64(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (float): Value to set
     */
    void SetFloat(float value) {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      polyhook::SetReturnFloat(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (double): Value to set
     */
    void SetDouble(double value) {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      polyhook::SetReturnDouble(_handle, value);
    }

    /**
     * @brief Set return value
     * @param value (ptr64): Value to set
     */
    void SetPointer(void* value) {
      if (_handle == nullptr) throw std::runtime_error("Return: Empty handle");
      polyhook::SetReturnPointer(_handle, value);
    }

  private:
    void* _handle{nullptr};
  };


} // namespace polyhook
