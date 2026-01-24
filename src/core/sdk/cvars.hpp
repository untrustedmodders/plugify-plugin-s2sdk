#pragma once

#include <icvar.h>
#include <tier1/convar.h>
#include <convar.h>

namespace cvars {
	void SendConVarValue(CPlayerSlot slot, std::string_view name, std::string_view value);
	void SendMultipleConVarValues(CPlayerSlot slot, std::span<const std::string_view> names, std::span<const std::string_view> values, uint32_t size);
	void NotifyConVar(ConVarRefAbstract conVar, std::string_view value);
	void ReplicateConVar(ConVarRefAbstract conVar, std::string_view value);

	static Result<ConVarRefAbstract> CreateConVar(uint64 conVarHandle) {
		ConVarRef conVarRef(conVarHandle);

		if (!conVarRef.IsValidRef()) {
			return MakeError("Invalid ConVar handle: {}\n", conVarHandle);
		}

		ConVarData* conVarData = g_pCVar->GetConVarData(conVarRef);
		if (conVarData == nullptr) {
			return MakeError("Failed to get ConVar data for handle: {}\n", conVarHandle);
		}

		return ConVarRefAbstract(conVarRef, conVarData);
	}

	inline void SetConVarString(ConVarRefAbstract conVar, std::string_view value, bool replicate, bool notify) {
		conVar.SetString(CUtlString(value), -1);
		if (replicate) ReplicateConVar(conVar, value);
		if (notify) NotifyConVar(conVar, value);
	}

	inline void SetConVarStringByHandle(uint64 conVarHandle, std::string_view value, bool replicate, bool notify) {
		if (auto conVar = CreateConVar(conVarHandle)) {
			SetConVarString(*conVar, value, replicate, notify);
		} else {
			plg::print(LS_WARNING, conVar.error());
		}
	}

	template<typename T>
	void SetConVar(ConVarRefAbstract conVar, const T& value, bool replicate, bool notify) {
		if (conVar.GetType() != TranslateConVarType<T>()) {
			plg::print(LS_WARNING, "Invalid cvar type for variable '{}'. Expected: '{}', but got: '{}'. Please ensure the type matches the expected type.\n", conVar.GetName(), conVar.TypeTraits()->m_TypeName, GetCvarTypeTraits(TranslateConVarType<T>())->m_TypeName);
			return;
		}

		conVar.SetAs<T>(value);

		if (notify || replicate) {
			if constexpr (std::same_as<T, CUtlString>) {
				if (replicate) ReplicateConVar(conVar, value.Get());
				if (notify) NotifyConVar(conVar, value.Get());
			} else {
				std::string val;
				if constexpr (std::is_same_v<T, Color>) {
					val = std::format("{} {} {} {}", value.r(), value.g(), value.b(), value.a());
				} else if constexpr (std::is_same_v<T, Vector2D>) {
					val = std::format("{} {}", value.x, value.y);
				} else if constexpr (std::is_same_v<T, Vector> || std::is_same_v<T, QAngle>) {
					val = std::format("{} {} {}", value.x, value.y, value.z);
				} else if constexpr (std::is_same_v<T, Vector4D>) {
					val = std::format("{} {} {} {}", value.x, value.y, value.z, value.w);
				} else {
					val = std::to_string(value);
				}
				if (replicate) ReplicateConVar(conVar, val);
				if (notify) NotifyConVar(conVar, val);
			}
		}
	}

	template<typename T>
	void SetConVarByHandle(uint64 conVarHandle, const T& value, bool replicate, bool notify) {
		if (auto conVar = CreateConVar(conVarHandle)) {
			SetConVar<T>(*conVar, value, replicate, notify);
		} else {
			plg::print(LS_WARNING, conVar.error());
		}
	}
	
	inline void SetConVarByHandle(uint64 conVarHandle, const plg::any& value, bool replicate, bool notify) {
		if (auto conVar = CreateConVar(conVarHandle)) {
			switch (conVar->GetType()) {
				case EConVarType_Bool:
					plg::visit([&](const auto& v) {
						using T = std::decay_t<decltype(v)>;
						if constexpr (std::is_arithmetic_v<T>)
							SetConVar(*conVar, static_cast<bool>(v), replicate, notify);
					}, value);
					break;
				case EConVarType_Int16:
					plg::visit([&](const auto& v) {
						using T = std::decay_t<decltype(v)>;
						if constexpr (std::is_arithmetic_v<T>)
							SetConVar(*conVar, static_cast<int16>(v), replicate, notify);
					}, value);
					break;
				case EConVarType_UInt16:
					plg::visit([&](const auto& v) {
						using T = std::decay_t<decltype(v)>;
						if constexpr (std::is_arithmetic_v<T>)
							SetConVar(*conVar, static_cast<uint16>(v), replicate, notify);
					}, value);
					break;
				case EConVarType_Int32:
					plg::visit([&](const auto& v) {
						using T = std::decay_t<decltype(v)>;
						if constexpr (std::is_arithmetic_v<T>)
							SetConVar(*conVar, static_cast<int32>(v), replicate, notify);
					}, value);
					break;
				case EConVarType_UInt32:
					plg::visit([&](const auto& v) {
						using T = std::decay_t<decltype(v)>;
						if constexpr (std::is_arithmetic_v<T>)
							SetConVar(*conVar, static_cast<uint32>(v), replicate, notify);
					}, value);
					break;
				case EConVarType_Int64:
					plg::visit([&](const auto& v) {
						using T = std::decay_t<decltype(v)>;
						if constexpr (std::is_arithmetic_v<T>)
							SetConVar(*conVar, static_cast<int64>(v), replicate, notify);
					}, value);
					break;
				case EConVarType_UInt64:
					plg::visit([&](const auto& v) {
						using T = std::decay_t<decltype(v)>;
						if constexpr (std::is_arithmetic_v<T>)
							SetConVar(*conVar, static_cast<uint64>(v), replicate, notify);
					}, value);
					break;
				case EConVarType_Float32:
					plg::visit([&](const auto& v) {
						using T = std::decay_t<decltype(v)>;
						if constexpr (std::is_arithmetic_v<T>)
							SetConVar(*conVar, static_cast<float>(v), replicate, notify);
					}, value);
					break;
				case EConVarType_Float64:
					plg::visit([&](const auto& v) {
						using T = std::decay_t<decltype(v)>;
						if constexpr (std::is_arithmetic_v<T>)
							SetConVar(*conVar, static_cast<double>(v), replicate, notify);
					}, value);
					break;
				case EConVarType_String:
					plg::visit([&](const auto& v) {
						using T = std::decay_t<decltype(v)>;
						if constexpr (std::is_same_v<T, plg::string>)
							SetConVar(*conVar, CUtlString(v), replicate, notify);
					}, value);
					break;
				case EConVarType_Color:
					plg::visit([&](const auto& v) {
						using T = std::decay_t<decltype(v)>;
						if constexpr (std::is_arithmetic_v<T>)
							SetConVar(*conVar, std::bit_cast<Color>(static_cast<int>(v)), replicate, notify);
						else if constexpr (std::is_same_v<T, plg::vec4>)
							SetConVar<Color>(*conVar, NewColor(v), replicate, notify);
						else if constexpr (std::is_same_v<T, plg::vec3>)
							SetConVar<Color>(*conVar, NewColor(v), replicate, notify);
					}, value);
					break;
				case EConVarType_Vector2:
					plg::visit([&](const auto& v) {
						using T = std::decay_t<decltype(v)>;
						if constexpr (std::is_same_v<T, plg::vec2>)
							SetConVar<Vector2D>(*conVar, std::bit_cast<Vector2D>(v), replicate, notify);
					}, value);
					break;
				case EConVarType_Vector3:
					plg::visit([&](const auto& v) {
						using T = std::decay_t<decltype(v)>;
						if constexpr (std::is_same_v<T, plg::vec3>)
							SetConVar<Vector>(*conVar, std::bit_cast<Vector>(v), replicate, notify);
					}, value);
					break;
				case EConVarType_Vector4:
					plg::visit([&](const auto& v) {
						using T = std::decay_t<decltype(v)>;
						if constexpr (std::is_same_v<T, plg::vec4>)
							SetConVar<Vector4D>(*conVar, std::bit_cast<Vector4D>(v), replicate, notify);
					}, value);
					break;
				case EConVarType_Qangle:
					plg::visit([&](const auto& v) {
						using T = std::decay_t<decltype(v)>;
						if constexpr (std::is_same_v<T, plg::vec3>)
							SetConVar<QAngle>(*conVar, std::bit_cast<QAngle>(v), replicate, notify);
					}, value);
					break;
				default:
					plg::print(LS_WARNING, "Invalid convar type: {}\n", conVarHandle);
					return;
			}
		} else {
			plg::print(LS_WARNING, conVar.error());
			return;
		}
	}
	
	template<typename T>
	void SetConVarValue(ConVarRefAbstract conVar, std::string_view value) {
		if (conVar.GetType() != TranslateConVarType<T>()) {
			plg::print(LS_WARNING, "Invalid cvar type for variable '{}'. Expected: '{}', but got: '{}'. Please ensure the type matches the expected type.\n", conVar.GetName(), conVar.TypeTraits()->m_TypeName, GetCvarTypeTraits(TranslateConVarType<T>())->m_TypeName);
			return;
		}

		conVar.SetString(CUtlString(value));
	}

	template<typename T>
	T GetConVarValue(ConVarRefAbstract conVar) {
		if (conVar.GetType() != TranslateConVarType<T>()) {
			plg::print(LS_WARNING, "Invalid cvar type for variable '{}'. Expected: '{}', but got: '{}'. Please ensure the type matches the expected type.\n", conVar.GetName(), conVar.TypeTraits()->m_TypeName, GetCvarTypeTraits(TranslateConVarType<T>())->m_TypeName);
			return {};
		}

		return conVar.GetAs<T>();
	}

	template<typename T>
	T GetConVarValueByHandle(uint64 conVarHandle) {
		if (auto conVar = CreateConVar(conVarHandle)) {
			return GetConVarValue<T>(*conVar);
		} else {
			plg::print(LS_WARNING, conVar.error());
			return {};
		}
	}

	inline plg::any GetConVarValueByHandle(uint64 conVarHandle) {
		if (auto conVar = CreateConVar(conVarHandle)) {
			switch (conVar->GetType()) {
				case EConVarType_Bool:
					return GetConVarValue<bool>(*conVar);
				case EConVarType_Int16:
					return GetConVarValue<int16>(*conVar);
				case EConVarType_UInt16:
					return GetConVarValue<uint16>(*conVar);
				case EConVarType_Int32:
					return GetConVarValue<int32>(*conVar);
				case EConVarType_UInt32:
					return GetConVarValue<uint32>(*conVar);
				case EConVarType_Int64:
					return static_cast<int64_t>(GetConVarValue<int64>(*conVar));
				case EConVarType_UInt64:
					return static_cast<uint64_t>(GetConVarValue<uint64>(*conVar));
				case EConVarType_Float32:
					return GetConVarValue<float>(*conVar);
				case EConVarType_Float64:
					return GetConVarValue<double>(*conVar);
				case EConVarType_String:
					return plg::string(static_cast<std::string_view>(GetConVarValue<CUtlString>(*conVar)));
				case EConVarType_Color:
					return std::bit_cast<plg::vec4>(GetConVarValue<Color>(*conVar).ToVector4D());
				case EConVarType_Vector2:
					return std::bit_cast<plg::vec2>(GetConVarValue<Vector2D>(*conVar));
				case EConVarType_Vector3:
					return std::bit_cast<plg::vec3>(GetConVarValue<Vector>(*conVar));
				case EConVarType_Vector4:
					return std::bit_cast<plg::vec4>(GetConVarValue<Vector4D>(*conVar));
				case EConVarType_Qangle:
					return std::bit_cast<plg::vec3>(GetConVarValue<QAngle>(*conVar));
				case EConVarType_Invalid:
				case EConVarType_MAX:
				default:
					return "<invalid>";
			}
		} else {
			plg::print(LS_WARNING, conVar.error());
			return {};
		}
	}

	int SendConVarValueQueryToClient(CPlayerSlot slot, std::string_view cvarName, int queryCvarCookieOverride = -1);
}
