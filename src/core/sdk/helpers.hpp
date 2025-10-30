#pragma once

#include "entity/cplayerpawn.h"
#include "entity/cplayercontroller.h"
#include "utils.hpp"

namespace helpers {
	template<typename T = CBaseEntity>
	T* GetEntity(
		int entityHandle,
		std::source_location location = std::source_location::current()
	) {
		auto* entity = static_cast<T*>(g_pGameEntitySystem->GetEntityInstance(CEntityHandle(static_cast<uint32>(entityHandle))));
		if (!entity) {
			plg::print(LS_WARNING, "Cannot execute '{}' on invalid entity handle: {}\n",
				location.function_name(), entityHandle);
			return {};
		}
		return entity;
	}

	inline CPlayerController* GetController(
		int playerSlot,
		std::source_location location = std::source_location::current()
	) {
		auto* controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
		if (!controller) {
			plg::print(LS_WARNING, "Cannot execute '{}' on invalid player slot: {}\n",
				location.function_name(), playerSlot);
			return {};
		}

		return controller;
	}

	inline std::pair<CPlayerController*, CPlayerPawn*> GetController2(
		int playerSlot,
		std::source_location location = std::source_location::current()
	) {
		auto* controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
		if (!controller) {
			plg::print(LS_WARNING, "Cannot execute '{}' on invalid player slot: {}\n",
				location.function_name(), playerSlot);
			return {};
		}

		auto* pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
		if (!pawn) {
			plg::print(LS_WARNING, "Cannot execute '{}' on invalid player pawn: {}\n",
				location.function_name(), playerSlot);
			return {};
		}

		return {controller, pawn};
	}

	template<typename S, typename T, typename M>
	S* GetService(
		int playerSlot,
		M T::*member,
		std::source_location location = std::source_location::current()
	) {
		auto* controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
		if (!controller) {
			plg::print(LS_WARNING, "Cannot execute '{}' on invalid player slot: {}\n",
				location.function_name(), playerSlot);
			return {};
		}

		auto& field = controller->*member;
		S* service = field;
		if (!service) {
			plg::print(LS_WARNING, "Cannot execute '{}' on invalid player service: {}\n",
				location.function_name(), playerSlot);
			return {};
		}

		return service;
	}

	template<typename S, typename T, typename M>
	S* GetService2(
		int playerSlot,
		M T::*member,
		std::source_location location = std::source_location::current()
	) {
		auto* controller = static_cast<CPlayerController*>(utils::GetController(playerSlot));
		if (!controller) {
			plg::print(LS_WARNING, "Cannot execute '{}' on invalid player slot: {}\n",
				location.function_name(), playerSlot);
			return {};
		}

		auto pawn = static_cast<CPlayerPawn*>(controller->GetCurrentPawn());
		if (!pawn) {
			plg::print(LS_WARNING, "Cannot execute '{}' on invalid player pawn: {}\n",
				location.function_name(), playerSlot);
			return {};
		}

		auto& field = pawn->*member;
		S* service = field;
		if (!service) {
			plg::print(LS_WARNING, "Cannot execute '{}' on invalid player service: {}\n",
				location.function_name(), playerSlot);
			return {};
		}

		return service;
	}

	inline variant_t GetVariant(const plg::any& value, FieldType type) {
		variant_t variant;
		switch (type) {
			case FieldType::Float32:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_arithmetic_v<T>) variant = static_cast<float>(v);
				}, value);
				break;
			case FieldType::Float64:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_arithmetic_v<T>) variant = static_cast<double>(v);
				}, value);
				break;
			case FieldType::Int32:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_arithmetic_v<T>) variant = static_cast<int>(v);
				}, value);
				break;
			case FieldType::UInt32:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_arithmetic_v<T>) variant = static_cast<uint>(v);
				}, value);
				break;
			case FieldType::Int64:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_arithmetic_v<T>) variant = static_cast<int64>(v);
				}, value);
				break;
			case FieldType::UInt64:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_arithmetic_v<T>) variant = static_cast<uint64>(v);
				}, value);
				break;
			case FieldType::Boolean:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_arithmetic_v<T>) variant = static_cast<bool>(v);
				}, value);
				break;
			case FieldType::Character:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_arithmetic_v<T>) variant = static_cast<char>(v);
				}, value);
				break;
			case FieldType::String:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_same_v<T, plg::string>) variant = castable_string_t(v.c_str());
				}, value);
				break;
			case FieldType::CString:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_same_v<T, plg::string>) variant = v.c_str();
				}, value);
				break;
			case FieldType::HScript:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_pointer_v<T>) variant = reinterpret_cast<HSCRIPT>(v);
				}, value);
				break;
			case FieldType::EHandle:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_arithmetic_v<T>) variant = CEntityHandle((uint32) v);
				}, value);
				break;
			case FieldType::Resource:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_pointer_v<T>) variant = reinterpret_cast<ResourceHandle_t>(v);
				}, value);
				break;
			case FieldType::Vector3d:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_same_v<T, plg::vec3> || std::is_same_v<T, plg::vec4>) variant = *reinterpret_cast<const Vector*>(&v);
				}, value);
				break;
			case FieldType::Vector2d:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_same_v<T, plg::vec2> || std::is_same_v<T, plg::vec3> || std::is_same_v<T, plg::vec4>) variant = *reinterpret_cast<const Vector2D*>(&v);
				}, value);
				break;
			case FieldType::Vector4d:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_same_v<T, plg::vec4>) variant = *reinterpret_cast<const Vector4D*>(&v);
				}, value);
				break;
			case FieldType::Color32:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_same_v<T, int>) variant = *reinterpret_cast<const Color*>(&v);
				}, value);
				break;
			case FieldType::QAngle:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_same_v<T, plg::vec3> || std::is_same_v<T, plg::vec4>) variant = *reinterpret_cast<const QAngle*>(&v);
				}, value);
				break;
			case FieldType::Quaternion:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_same_v<T, plg::vec4>) variant = *reinterpret_cast<const Quaternion*>(&v);
				}, value);
				break;
			default:
				plg::visit([&variant](const auto& v) {
					using T = std::decay_t<decltype(v)>;
					if constexpr (std::is_same_v<T, plg::string>) {
						variant = v.c_str();
					} else if constexpr (std::is_pointer_v<T>) {
						variant = reinterpret_cast<int64>(v);
					} else if constexpr (std::is_same_v<T, long>) {
						variant = static_cast<int64>(v);
					} else if constexpr (std::is_same_v<T, unsigned long>) {
						variant = static_cast<uint64>(v);
					} else if constexpr (std::is_arithmetic_v<T>) {
						variant = v;
					} else if constexpr (std::is_same_v<T, plg::vec2>) {
						variant = *reinterpret_cast<const Vector2D*>(&v);
					} else if constexpr (std::is_same_v<T, plg::vec3>) {
						variant = *reinterpret_cast<const Vector*>(&v);
					} else if constexpr (std::is_same_v<T, plg::vec4>) {
						variant = *reinterpret_cast<const Vector4D*>(&v);
					}
				}, value);
				break;
		}
		return variant;
	}
}