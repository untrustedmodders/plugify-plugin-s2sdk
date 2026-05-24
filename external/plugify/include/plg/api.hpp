#pragma once
#include <string_view>

namespace plg {
	constexpr int kApiVersion = 3;

	enum class PluginCode { Ok, Failed };

	class PluginResult {
	public:
		PluginResult() noexcept = default;

		explicit PluginResult(std::string_view message, PluginCode code = PluginCode::Failed)
			: _code{code}, _message{message} {}

		explicit operator bool() const noexcept { return _code == PluginCode::Ok; }
		operator std::string_view() const noexcept { return _message; }

		PluginCode GetCode() const noexcept { return _code; }
		std::string_view GetMessage() const noexcept { return _message; }

	private:
		PluginCode _code{};
		plg::string	_message{};
	};

	struct PluginContext {
		bool hasUpdate{};
		bool hasStart{};
		bool hasEnd{};
		bool hasDebug{};
	};
}
