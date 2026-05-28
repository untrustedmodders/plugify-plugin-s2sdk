#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

#include "errorhandling.hpp"
#include "mergeoperations.hpp"
#include "setmethods.hpp"
#include "trysetmethods.hpp"
#include "arraypushmethods.hpp"
#include "getmethods.hpp"
#include "navigationmethods.hpp"
#include "configuration.hpp"
#include "typechecking.hpp"
#include "getasmethods.hpp"
#include "querymethods.hpp"
#include "removalmethods.hpp"
#include "jsonexport.hpp"

// Generated from configs.pplugin (group: config)

namespace configs {

  /**
   * @brief RAII wrapper for Config pointer - provides configuration file reading, writing, and manipulation functionality.
   * @return 
   */
  class Config final {
  public:
    /**
     * @brief Reads a configuration file from the specified path.
     * @param path (string): Path to the configuration file.
     * @return
     */
    explicit Config(const plg::string& path)
      : Config(Read(path), Ownership::Owned) {}

    /**
     * @brief Reads multiple configuration files and merges them.
     * @param paths (string[]): Vector of paths to configuration files.
     * @return
     */
    explicit Config(const plg::vector<plg::string>& paths)
      : Config(ReadMultiple(paths), Ownership::Owned) {}

    /**
     * @brief Creates a new empty configuration object.
     * @return
     */
    explicit Config()
      : Config(Make(), Ownership::Owned) {}

    ~Config() {
      destroy();
    }

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    Config(Config&& other) noexcept
      : _handle(other._handle)
      , _ownership(other._ownership) {
      other.nullify();
    }

    Config& operator=(Config&& other) noexcept {
      if (this != &other) {
        destroy();
        _handle = other._handle;
        _ownership = other._ownership;
        other.nullify();
      }
      return *this;
    }

    Config(void* handle, Ownership ownership = Ownership::Borrowed) : _handle(handle), _ownership(ownership) {}

    [[nodiscard]] auto get() const noexcept { return _handle; }

    [[nodiscard]] auto release() noexcept {
      auto handle = _handle;
      nullify();
      return handle;
    }

    void reset() noexcept {
      destroy();
      nullify();
    }

    void swap(Config& other) noexcept {
      using std::swap;
      swap(_handle, other._handle);
      swap(_ownership, other._ownership);
    }

    friend void swap(Config& lhs, Config& rhs) noexcept { lhs.swap(rhs); }

    explicit operator bool() const noexcept { return _handle != nullptr; }
    [[nodiscard]] auto operator<=>(const Config& other) const noexcept { return _handle <=> other._handle; }
    [[nodiscard]] bool operator==(const Config& other) const noexcept { return _handle == other._handle; }

    /**
     * @brief Sets an error message for the configuration system.
     * @param error (string): The error message to set.
     */
    static void SetError(const plg::string& error) {
      configs::SetError(error);
    }

    /**
     * @brief Retrieves the last error message from the configuration system.
     * @return string: The last error message.
     */
    static plg::string GetError() {
      return configs::GetError();
    }

    /**
     * @brief Merges another configuration object into the current one.
     * @param other (Config): Pointer to the Config object to merge from.
     */
    void Merge(const Config& other) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::Merge(_handle, other.get());
    }

    /**
     * @brief Merges another configuration object into the current one with move semantics.
     * @param other (Config): Pointer to the Config object to merge from (will be moved).
     */
    void MergeMove(Config&& other) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::MergeMove(_handle, other.release());
    }

    /**
     * @brief Returns the type of the current configuration node.
     * @return int32: The NodeType enum value.
     */
    int32_t GetType() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::GetType(_handle);
    }

    /**
     * @brief Checks if the current node is null.
     * @return bool: True if the node is null, false otherwise.
     */
    bool IsNull() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::IsNull(_handle);
    }

    /**
     * @brief Checks if the current node is a boolean.
     * @return bool: True if the node is a boolean, false otherwise.
     */
    bool IsBool() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::IsBool(_handle);
    }

    /**
     * @brief Checks if the current node is an integer.
     * @return bool: True if the node is an integer, false otherwise.
     */
    bool IsInt() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::IsInt(_handle);
    }

    /**
     * @brief Checks if the current node is a float.
     * @return bool: True if the node is a float, false otherwise.
     */
    bool IsFloat() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::IsFloat(_handle);
    }

    /**
     * @brief Checks if the current node is a string.
     * @return bool: True if the node is a string, false otherwise.
     */
    bool IsString() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::IsString(_handle);
    }

    /**
     * @brief Checks if the current node is an object.
     * @return bool: True if the node is an object, false otherwise.
     */
    bool IsObject() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::IsObject(_handle);
    }

    /**
     * @brief Checks if the current node is an array.
     * @return bool: True if the node is an array, false otherwise.
     */
    bool IsArray() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::IsArray(_handle);
    }

    /**
     * @brief Sets a configuration value to null.
     * @param key (string): The key to set.
     */
    void SetNull(const plg::string& key) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::SetNull(_handle, key);
    }

    /**
     * @brief Sets a configuration value to an empty object.
     * @param key (string): The key to set.
     */
    void SetObject(const plg::string& key) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::SetObject(_handle, key);
    }

    /**
     * @brief Sets a configuration value to an empty array.
     * @param key (string): The key to set.
     */
    void SetArray(const plg::string& key) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::SetArray(_handle, key);
    }

    /**
     * @brief Sets a boolean configuration value.
     * @param key (string): The key to set.
     * @param value (bool): The boolean value to set.
     */
    void SetBool(const plg::string& key, bool value) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::SetBool(_handle, key, value);
    }

    /**
     * @brief Sets an integer configuration value.
     * @param key (string): The key to set.
     * @param value (int64): The integer value to set.
     */
    void SetInt(const plg::string& key, int64_t value) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::SetInt(_handle, key, value);
    }

    /**
     * @brief Sets a float configuration value.
     * @param key (string): The key to set.
     * @param value (double): The float value to set.
     */
    void SetFloat(const plg::string& key, double value) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::SetFloat(_handle, key, value);
    }

    /**
     * @brief Sets a string configuration value.
     * @param key (string): The key to set.
     * @param value (string): The string value to set.
     */
    void SetString(const plg::string& key, const plg::string& value) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::SetString(_handle, key, value);
    }

    /**
     * @brief Attempts to set a configuration value from a boolean, with type conversion if needed.
     * @param key (string): The key to set.
     * @param value (bool): The boolean value to set.
     * @return bool: True if the value was set successfully, false otherwise.
     */
    bool TrySetFromBool(const plg::string& key, bool value) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::TrySetFromBool(_handle, key, value);
    }

    /**
     * @brief Attempts to set a configuration value from an integer, with type conversion if needed.
     * @param key (string): The key to set.
     * @param value (int64): The integer value to set.
     * @return bool: True if the value was set successfully, false otherwise.
     */
    bool TrySetFromInt(const plg::string& key, int64_t value) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::TrySetFromInt(_handle, key, value);
    }

    /**
     * @brief Attempts to set a configuration value from a float, with type conversion if needed.
     * @param key (string): The key to set.
     * @param value (double): The float value to set.
     * @return bool: True if the value was set successfully, false otherwise.
     */
    bool TrySetFromFloat(const plg::string& key, double value) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::TrySetFromFloat(_handle, key, value);
    }

    /**
     * @brief Attempts to set a configuration value from a string, with type conversion if needed.
     * @param key (string): The key to set.
     * @param value (string): The string value to set.
     * @return bool: True if the value was set successfully, false otherwise.
     */
    bool TrySetFromString(const plg::string& key, const plg::string& value) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::TrySetFromString(_handle, key, value);
    }

    /**
     * @brief Pushes a null value to the current array node.
     */
    void PushNull() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::PushNull(_handle);
    }

    /**
     * @brief Pushes a boolean value to the current array node.
     * @param value (bool): The boolean value to push.
     */
    void PushBool(bool value) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::PushBool(_handle, value);
    }

    /**
     * @brief Pushes an integer value to the current array node.
     * @param value (int64): The integer value to push.
     */
    void PushInt(int64_t value) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::PushInt(_handle, value);
    }

    /**
     * @brief Pushes a float value to the current array node.
     * @param value (double): The float value to push.
     */
    void PushFloat(double value) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::PushFloat(_handle, value);
    }

    /**
     * @brief Pushes a string value to the current array node.
     * @param value (string): The string value to push.
     */
    void PushString(const plg::string& value) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::PushString(_handle, value);
    }

    /**
     * @brief Pushes an empty object to the current array node.
     */
    void PushObject() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::PushObject(_handle);
    }

    /**
     * @brief Pushes an empty array to the current array node.
     */
    void PushArray() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::PushArray(_handle);
    }

    /**
     * @brief Retrieves a boolean value from the configuration.
     * @param key (string): The key to retrieve.
     * @param defaultValue (bool): The default value to return if the key is not found.
     * @return bool: The boolean value if found, otherwise the default value.
     */
    bool GetBool(const plg::string& key = "##this##", bool defaultValue = false) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::GetBool(_handle, key, defaultValue);
    }

    /**
     * @brief Retrieves an integer value from the configuration.
     * @param key (string): The key to retrieve.
     * @param defaultValue (int64): The default value to return if the key is not found.
     * @return int64: The integer value if found, otherwise the default value.
     */
    int64_t GetInt(const plg::string& key = "##this##", int64_t defaultValue = 0) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::GetInt(_handle, key, defaultValue);
    }

    /**
     * @brief Retrieves a float value from the configuration.
     * @param key (string): The key to retrieve.
     * @param defaultValue (double): The default value to return if the key is not found.
     * @return double: The float value if found, otherwise the default value.
     */
    double GetFloat(const plg::string& key = "##this##", double defaultValue = 0.0) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::GetFloat(_handle, key, defaultValue);
    }

    /**
     * @brief Retrieves a string value from the configuration.
     * @param key (string): The key to retrieve.
     * @param defaultValue (string): The default value to return if the key is not found.
     * @return string: The string value if found, otherwise the default value.
     */
    plg::string GetString(const plg::string& key = "##this##", const plg::string& defaultValue = {}) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::GetString(_handle, key, defaultValue);
    }

    /**
     * @brief Retrieves a value from the configuration and converts it to boolean.
     * @param key (string): The key to retrieve.
     * @param defaultValue (bool): The default value to return if the key is not found.
     * @return bool: The boolean value if found and convertible, otherwise the default value.
     */
    bool GetAsBool(const plg::string& key = "##this##", bool defaultValue = false) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::GetAsBool(_handle, key, defaultValue);
    }

    /**
     * @brief Retrieves a value from the configuration and converts it to integer.
     * @param key (string): The key to retrieve.
     * @param defaultValue (int64): The default value to return if the key is not found.
     * @return int64: The integer value if found and convertible, otherwise the default value.
     */
    int64_t GetAsInt(const plg::string& key = "##this##", int64_t defaultValue = 0) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::GetAsInt(_handle, key, defaultValue);
    }

    /**
     * @brief Retrieves a value from the configuration and converts it to float.
     * @param key (string): The key to retrieve.
     * @param defaultValue (double): The default value to return if the key is not found.
     * @return double: The float value if found and convertible, otherwise the default value.
     */
    double GetAsFloat(const plg::string& key = "##this##", double defaultValue = 0.0) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::GetAsFloat(_handle, key, defaultValue);
    }

    /**
     * @brief Retrieves a value from the configuration and converts it to string.
     * @param key (string): The key to retrieve.
     * @param defaultValue (string): The default value to return if the key is not found.
     * @return string: The string value if found and convertible, otherwise the default value.
     */
    plg::string GetAsString(const plg::string& key = "##this##", const plg::string& defaultValue= {}) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::GetAsString(_handle, key, defaultValue);
    }

    /**
     * @brief Checks if a key exists in the current configuration node.
     * @param key (string): The key to check.
     * @return bool: True if the key exists, false otherwise.
     */
    bool HasKey(const plg::string& key) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::HasKey(_handle, key);
    }

    /**
     * @brief Checks if the current configuration node is empty.
     * @return bool: True if the node is empty, false otherwise.
     */
    bool Empty() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::Empty(_handle);
    }

    /**
     * @brief Returns the size of the current configuration node (number of elements in array or object).
     * @return int64: The size of the node.
     */
    int64_t GetSize() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::GetSize(_handle);
    }

    /**
     * @brief Returns the name of the current configuration node.
     * @return string: The name of the node.
     */
    plg::string GetName() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::GetName(_handle);
    }

    /**
     * @brief Returns the full path to the current configuration node.
     * @return string: The path to the node.
     */
    plg::string GetPath() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::GetPath(_handle);
    }

    /**
     * @brief Jumps to the first child node in the current configuration node.
     * @return bool: True if the jump was successful, false otherwise.
     */
    bool JumpFirst() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::JumpFirst(_handle);
    }

    /**
     * @brief Jumps to the last child node in the current configuration node.
     * @return bool: True if the jump was successful, false otherwise.
     */
    bool JumpLast() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::JumpLast(_handle);
    }

    /**
     * @brief Jumps to the next sibling node in the current configuration level.
     * @return bool: True if the jump was successful, false otherwise.
     */
    bool JumpNext() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::JumpNext(_handle);
    }

    /**
     * @brief Jumps to the previous sibling node in the current configuration level.
     * @return bool: True if the jump was successful, false otherwise.
     */
    bool JumpPrev() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::JumpPrev(_handle);
    }

    /**
     * @brief Jumps to a child node with the specified key, optionally creating it if it doesn't exist.
     * @param key (string): The key of the node to jump to.
     * @param create (bool): If true, creates the node if it doesn't exist.
     * @return bool: True if the jump was successful, false otherwise.
     */
    bool JumpKey(const plg::string& key, bool create = false) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::JumpKey(_handle, key, create);
    }

    /**
     * @brief Jumps to the nth child node in the current configuration node.
     * @param n (int32): The index of the child node to jump to.
     * @return bool: True if the jump was successful, false otherwise.
     */
    bool JumpN(int32_t n) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::JumpN(_handle, n);
    }

    /**
     * @brief Jumps back to the parent node in the configuration hierarchy.
     * @return bool: True if the jump was successful, false otherwise.
     */
    bool JumpBack() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::JumpBack(_handle);
    }

    /**
     * @brief Jumps back to the root node of the configuration.
     */
    void JumpRoot() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::JumpRoot(_handle);
    }

    /**
     * @brief Removes the current configuration node.
     * @return int32: The number of nodes removed.
     */
    int32_t Remove() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::Remove(_handle);
    }

    /**
     * @brief Removes a child node with the specified key from the current configuration node.
     * @param key (string): The key of the node to remove.
     * @return bool: True if the node was removed successfully, false otherwise.
     */
    bool RemoveKey(const plg::string& key) {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::RemoveKey(_handle, key);
    }

    /**
     * @brief Clears all child nodes from the current configuration node.
     */
    void Clear() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      configs::Clear(_handle);
    }

    /**
     * @brief Converts the current configuration node to a JSON string.
     * @return string: The JSON string representation of the current node.
     */
    plg::string NodeToJsonString() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::NodeToJsonString(_handle);
    }

    /**
     * @brief Converts the entire configuration tree to a JSON string.
     * @return string: The JSON string representation of the entire configuration tree.
     */
    plg::string RootToJsonString() {
      if (_handle == nullptr) throw std::runtime_error("Config: empty handle");
      return configs::RootToJsonString(_handle);
    }

  private:
    void destroy() const noexcept {
      if (_handle != nullptr && _ownership == Ownership::Owned) {
        Delete(_handle);
      }
    }

    void nullify() noexcept {
      _handle = nullptr;
      _ownership = Ownership::Borrowed;
    }

    void* _handle{nullptr};
    Ownership _ownership{Ownership::Borrowed};
  };


} // namespace configs
