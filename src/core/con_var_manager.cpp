#include "con_var_manager.hpp"

#include <core/sdk/utils.h>
#include <core/sdk/cvars.h>

ConVarManager::~ConVarManager() {
	if (!g_pCVar) {
		return;
	}

	g_pCVar->RemoveGlobalChangeCallback(&ChangeGlobal);
	for (const auto& [cv, _]: m_cnvCache) {
		g_pCVar->UnregisterConVarCallbacks(*cv);
	}
}

ConVarInfo::ConVarInfo(plg::string name, plg::string description) : name(std::move(name)), description(std::move(description)) {
}

bool ConVarManager::RemoveConVar(const plg::string& name) {
	auto it = m_cnvLookup.find(name);
	if (it != m_cnvLookup.end()) {
		m_cnvCache.erase(std::get<ConVarInfoPtr>(*it)->conVar.get());
		m_cnvLookup.erase(it);
		return true;
	}

	return false;
}

ConVarRef ConVarManager::FindConVar(const plg::string& name) {
	auto it = m_cnvLookup.find(name);
	if (it != m_cnvLookup.end()) {
		return *std::get<ConVarInfoPtr>(*it)->conVar;
	}

	auto& conVarInfo = *m_cnvLookup.emplace(name, std::make_unique<ConVarInfo>(name, "")).first->second;
	conVarInfo.conVar = std::make_unique<ConVarRefAbstract>(name.c_str(), true);
	m_cnvCache.emplace(conVarInfo.conVar.get(), &conVarInfo);

	if (!conVarInfo.conVar->IsValidRef()) {
		S2_LOGF(LS_WARNING, "Failed to find \"{}\" convar\n", name);
		return {};
	}

	return *conVarInfo.conVar;
}

void ConVarManager::HookConVarChange(const plg::string& name, ConVarChangeListenerCallback callback) {
	if (name.empty()) {
		if (m_global.Empty()) {
			g_pCVar->InstallGlobalChangeCallback(&ChangeGlobal);
		}
		m_global.Register(callback);
		return;
	}

	auto it = m_cnvLookup.find(name);
	if (it != m_cnvLookup.end()) {
		auto& conVarInfo = *std::get<ConVarInfoPtr>(*it);
		conVarInfo.hook.Register(callback);
	}
}

void ConVarManager::UnhookConVarChange(const plg::string& name, ConVarChangeListenerCallback callback) {
	if (name.empty()) {
		m_global.Unregister(callback);
		if (m_global.Empty()) {
			g_pCVar->RemoveGlobalChangeCallback(&ChangeGlobal);
		}
		return;
	}

	auto it = m_cnvLookup.find(name);
	if (it != m_cnvLookup.end()) {
		auto& conVarInfo = *std::get<ConVarInfoPtr>(*it);
		conVarInfo.hook.Unregister(callback);
	}
}

void ConVarManager::ChangeGlobal(ConVarRefAbstract* ref, CSplitScreenSlot slot, const char* pNewValue, const char* pOldValue, void*) {
	g_ConVarManager.m_global.Notify(*ref, pNewValue, pOldValue);
}

struct ConVal {
	CVValue_t* cv;
	EConVarType type;
};

// format support
#ifdef FMT_HEADER_ONLY
namespace fmt {
#else
namespace std {
#endif
template<>
struct formatter<ConVal> {
	constexpr auto parse(std::format_parse_context& ctx) {
		return ctx.begin();
	}

	template<class FormatContext>
	auto format(const ConVal& cv, FormatContext& ctx) const {
		switch (const auto& [value, type] = cv ;type) {
			case EConVarType_Bool:
				return std::format_to(ctx.out(), "{}", value->m_bValue);
			case EConVarType_Int16:
				return std::format_to(ctx.out(), "{}", value->m_i16Value);
			case EConVarType_UInt16:
				return std::format_to(ctx.out(), "{}", value->m_u16Value);
			case EConVarType_Int32:
				return std::format_to(ctx.out(), "{}", value->m_i32Value);
			case EConVarType_UInt32:
				return std::format_to(ctx.out(), "{}", value->m_u32Value);
			case EConVarType_Int64:
				return std::format_to(ctx.out(), "{}", value->m_i64Value);
			case EConVarType_UInt64:
				return std::format_to(ctx.out(), "{}", value->m_u64Value);
			case EConVarType_Float32:
				return std::format_to(ctx.out(), "{}", value->m_fl32Value);
			case EConVarType_Float64:
				return std::format_to(ctx.out(), "{}", value->m_fl64Value);
			case EConVarType_String:
				return std::format_to(ctx.out(), "{}", std::string_view(value->m_StringValue.Get(), static_cast<size_t>(value->m_StringValue.Length())));
			case EConVarType_Color:
				return std::format_to(ctx.out(), "{} {} {} {}", value->m_clrValue.r(), value->m_clrValue.g(), value->m_clrValue.b(), value->m_clrValue.a());
			case EConVarType_Vector2:
				return std::format_to(ctx.out(), "{} {}", value->m_vec2Value.x, value->m_vec2Value.y);
			case EConVarType_Vector3:
				return std::format_to(ctx.out(), "{} {} {}", value->m_vec3Value.x, value->m_vec3Value.y, value->m_vec3Value.z);
			case EConVarType_Vector4:
				return std::format_to(ctx.out(), "{} {} {} {}", value->m_vec4Value.x, value->m_vec4Value.y, value->m_vec4Value.z, value->m_vec4Value.w);
			case EConVarType_Qangle:
				return std::format_to(ctx.out(), "{} {} {}", value->m_angValue.x, value->m_angValue.y, value->m_angValue.z);
			case EConVarType_Invalid:
			case EConVarType_MAX:
			default:
				return std::format_to(ctx.out(), "<invalid>");
		}
	}
};
}// namespace std

class ConVarConfigGenerator {
public:
    static void FormatConVarData(std::ofstream& os, const ConVarData* conVarData) {
        if (conVarData->HasHelpText()) {
            auto lines = plg::split(conVarData->GetHelpText(), "\n");
            for (const auto& line : lines) {
                std::print(os, "// {}\n", line);
            }
        }

		std::print(os, "// -\n// Type: \"{}\"\n", conVarData->TypeTraits()->m_TypeName);

    	if (conVarData->HasDefaultValue()) {
			std::print(os, "// Default: \"{}\"\n", ConVal{conVarData->DefaultValue(), conVarData->GetType()});
    	}
        if (conVarData->HasMinValue()) {
			std::print(os, "// Minimum: \"{}\"\n", ConVal{conVarData->MinValue(), conVarData->GetType()});
        }
        if (conVarData->HasMaxValue()) {
			std::print(os, "// Maximum: \"{}\"\n", ConVal{conVarData->MaxValue(), conVarData->GetType()});
        }

        std::print(os, "{} \"{}\"\n\n", conVarData->GetName(), ConVal{conVarData->ValueOrDefault(-1), conVarData->GetType()});
    }
};

class ConVarConfigParser {
public:
	using ConVarMap = std::map<plg::string, plg::string, plg::case_insensitive_equal>;

	static std::expected<ConVarMap, std::string> ParseConfigFile(const fs::path& filePath) {
		std::error_code ec;
		if (!fs::exists(filePath, ec)) {
			return std::unexpected("Config file does not exist");
		}

		std::ifstream file(filePath);
		if (!file.is_open()) {
			return std::unexpected(std::format("Failed to open config file: {}", plg::as_string(filePath)));
		}

		ConVarMap conVars;
		std::string line;

		auto trim = [](std::string_view str) -> std::string_view {
			const auto start = str.find_first_not_of(" \t\n\r\f\v");
			if (start == std::string_view::npos)
				return {};
			const auto end = str.find_last_not_of(" \t\n\r\f\v");
			return str.substr(start, end - start + 1);
		};

		while (std::getline(file, line)) {
			std::string_view buffer = trim(line);

			if (buffer.empty() || buffer.starts_with("//")) {
				continue;
			}

			auto spacePos = buffer.find(' ');
			if (spacePos == std::string::npos) {
				continue;
			}

			std::string_view name = buffer.substr(0, spacePos);
			std::string_view value = trim(buffer.substr(spacePos + 1));

			if (value.starts_with('"') && value.ends_with('"') && value.length() >= 2) {
				value = value.substr(1, value.length() - 2);
			}

			conVars[name] = value;
		}

		return conVars;
	}
};

bool ConVarManager::AutoExecConfig(std::span<const uint64> conVarHandles, bool autoCreate, std::string_view name, std::string_view folder) {
    if (name.empty()) {
    	S2_LOGF(LS_WARNING, "No valid Config's name to process\n");
	    return false;
    }

    std::string_view configFolder = folder.empty() ? "plugify" : folder;
    fs::path configPath = utils::GameDirectory() / S2SDK_GAME_NAME "/cfg" / configFolder;
    fs::path fullPath = configPath / std::format("{}.cfg", name);

    std::vector<ConVarRefAbstract> conVarRefs;
    conVarRefs.reserve(conVarHandles.size());

    for (const auto& handle : conVarHandles) {
		if (auto conVar = cvars::CreateConVar(handle)) {
            conVarRefs.push_back(*conVar);
        } else {
            S2_LOG(LS_WARNING, conVar.error().c_str());
        }
    }

    if (conVarRefs.empty()) {
        S2_LOGF(LS_WARNING, "No valid ConVars to process\n");
    	return false;
    }

	std::error_code ec;
    bool configExists = fs::exists(fullPath, ec);

    if (configExists) {
        if (auto parseResult = ConVarConfigParser::ParseConfigFile(fullPath)) {
			const auto& parsed = *parseResult;
        	for (const auto& conVar : conVarRefs) {
        		plg::string label = conVar.GetName();
                auto it = parsed.find(label);
                if (it != parsed.end()) {
                    UpdateConVarValue(conVar, it->second);
                    S2_LOGF(LS_DEBUG, "Loaded ConVar: {} = \"{}\"\n", label, it->second);
                }
            }
    		S2_LOGF(LS_DEBUG, "Parsed config file: {}\n", plg::as_string(fullPath));
        	return true;
        } else {
            S2_LOGF(LS_WARNING, "Failed to parse config file: {}\n", parseResult.error());
        	return false;
        }
    } else if (autoCreate) {
    	if (!fs::exists(configPath, ec)) {
    		fs::create_directories(configPath, ec);
    	}

    	std::ofstream file(fullPath);
    	if (file.is_open()) {
    		auto now = std::chrono::system_clock::now();
    		std::print(file,
    			"// This file was auto-generated by Plugify (v" S2SDK_VERSION ")\n"
				"// ConVars for plugin \"{}\"\n"
				"// Generated: {:%Y-%m-%d %H:%M:%S}\n\n",
				name, now);
    		for (const auto& conVar : conVarRefs) {
    			ConVarConfigGenerator::FormatConVarData(file, conVar.GetConVarData());
    		}
    		file.close();
    		S2_LOGF(LS_DEBUG, "Created config file: {}\n", plg::as_string(fullPath));
			return true;
    	} else {
    		S2_LOGF(LS_WARNING, "Failed to create config file: {}\n", plg::as_string(fullPath));
			return false;
    	}
    } else {
        S2_LOGF(LS_WARNING, "Config file does not exist and autoCreate is false: {}\n", plg::as_string(fullPath));
		return false;
    }
}

void ConVarManager::UpdateConVarValue(
    ConVarRefAbstract conVar,
	std::string_view value
) {
	bool notify = conVar.GetFlags() & FCVAR_NOTIFY;
	bool replicate = conVar.GetFlags() & FCVAR_REPLICATED;
	switch (EConVarType type = conVar.GetType()) {
		case EConVarType_Bool: {
			using namespace std::string_view_literals;
			plg::case_insensitive_equal eq;
			bool boolean = eq(value, "1"sv) || eq(value, "true"sv) || eq(value, "yes"sv) || eq(value, "on"sv);
			cvars::SetConVar(conVar, boolean, replicate, notify);
			break;
		}

		case EConVarType_Int16: {
			if (const auto number = plg::cast_to<int16>(value)) {
				cvars::SetConVar(conVar, *number, replicate, notify);
			} else {
				S2_LOGF(LS_WARNING, "Failed to parse Int16 value for ConVar '{}': '{}' is not a valid integer (range: {} to {})\n",
						conVar.GetName(), value, std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max());
			}
			break;
		}

		case EConVarType_UInt16: {
			if (const auto number = plg::cast_to<uint16>(value)) {
				cvars::SetConVar(conVar, *number, replicate, notify);
			} else {
				S2_LOGF(LS_WARNING, "Failed to parse UInt16 value for ConVar '{}': '{}' is not a valid unsigned integer (range: 0 to {})\n",
						conVar.GetName(), value, std::numeric_limits<uint16_t>::max());
			}
			break;
		}

		case EConVarType_Int32: {
			if (const auto number = plg::cast_to<int32>(value)) {
				cvars::SetConVar(conVar, *number, replicate, notify);
			} else {
				S2_LOGF(LS_WARNING, "Failed to parse Int32 value for ConVar '{}': '{}' is not a valid integer (range: {} to {})\n",
					   conVar.GetName(), value, std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());
			}
			break;
		}

		case EConVarType_UInt32: {
			if (const auto number = plg::cast_to<uint32>(value)) {
				cvars::SetConVar(conVar, *number, replicate, notify);
			} else {
				S2_LOGF(LS_WARNING, "Failed to parse UInt32 value for ConVar '{}': '{}' is not a valid unsigned integer (range: 0 to {})\n",
						conVar.GetName(), value, std::numeric_limits<uint32_t>::max());
			}
			break;
		}

		case EConVarType_Int64: {
			if (const auto number = plg::cast_to<int64>(value)) {
				cvars::SetConVar(conVar, *number, replicate, notify);
			} else {
				S2_LOGF(LS_WARNING, "Failed to parse Int64 value for ConVar '{}': '{}' is not a valid integer (range: {} to {})\n",
						conVar.GetName(), value, std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max());
			}
			break;
		}

		case EConVarType_UInt64: {
			if (const auto number = plg::cast_to<uint64>(value)) {
				cvars::SetConVar(conVar, *number, replicate, notify);
			} else {
				S2_LOGF(LS_WARNING, "Failed to parse UInt64 value for ConVar '{}': '{}' is not a valid unsigned integer (range: 0 to {})\n",
						conVar.GetName(), value, std::numeric_limits<uint64_t>::max());
			}
			break;
		}

		case EConVarType_Float32: {
			if (const auto number = plg::cast_to<float>(value)) {
				cvars::SetConVar(conVar, *number, replicate, notify);
			} else {
				S2_LOGF(LS_WARNING, "Failed to parse Float32 value for ConVar '{}': '{}' is not a valid floating-point number\n",
						conVar.GetName(), value);
			}
			break;
		}

		case EConVarType_Float64: {
			if (const auto number = plg::cast_to<double>(value)) {
				cvars::SetConVar(conVar, *number, replicate, notify);
			} else {
				S2_LOGF(LS_WARNING, "Failed to parse Float64 value for ConVar '{}': '{}' is not a valid floating-point number\n",
						conVar.GetName(), value);
			}
			break;
		}

		case EConVarType_String: {
			cvars::SetConVar(conVar, CUtlString(value.data(), static_cast<int>(value.size())), replicate, notify);
			break;
		}

		case EConVarType_Color: {
			if (const auto color = plg::parse<int>(value); color.size() == 4) {
				cvars::SetConVar(conVar, Color(color[0], color[1], color[2], color[3]), replicate, notify);
			} else {
				S2_LOGF(LS_WARNING, "Invalid Color value for ConVar '{}': expected 4 components, got {}. Value: '{}'\n",
						conVar.GetName(), color.size(), value);
			}
			break;
		}

		case EConVarType_Vector2: {
			if (const auto vector = plg::parse<float>(value); vector.size() == 2) {
				cvars::SetConVar(conVar, Vector2D(vector[0], vector[1]), replicate, notify);
			} else {
				S2_LOGF(LS_WARNING, "Invalid Vector2D value for ConVar '{}': expected 2 components, got {}. Value: '{}'\n",
						conVar.GetName(), vector.size(), value);
			}
			break;
		}

		case EConVarType_Vector3: {
			if (const auto vector = plg::parse<float>(value); vector.size() == 3) {
				cvars::SetConVar(conVar, Vector(vector[0], vector[1], vector[2]), replicate, notify);
			} else {
				S2_LOGF(LS_WARNING, "Invalid Vector3 value for ConVar '{}': expected 3 components, got {}. Value: '{}'\n",
						conVar.GetName(), vector.size(), value);
			}
			break;
		}

		case EConVarType_Vector4: {
			if (const auto vector = plg::parse<float>(value); vector.size() == 4) {
				cvars::SetConVar(conVar, Vector4D(vector[0], vector[1], vector[2], vector[3]), replicate, notify);
			} else {
				S2_LOGF(LS_WARNING, "Invalid Vector4D value for ConVar '{}': expected 4 components, got {}. Value: '{}'\n",
						conVar.GetName(), vector.size(), value);
			}
			break;
		}

		case EConVarType_Qangle: {
			if (const auto vector = plg::parse<float>(value); vector.size() == 3) {
				cvars::SetConVar(conVar, QAngle(vector[0], vector[1], vector[2]), replicate, notify);
			} else {
				S2_LOGF(LS_WARNING, "Invalid QAngle value for ConVar '{}': expected 3 components (pitch, yaw, roll), got {}. Value: '{}'\n",
						conVar.GetName(), vector.size(), value);
			}
			break;
		}

		default:
			S2_LOGF(LS_WARNING, "Invalid convar type: {}\n", static_cast<int>(type));
			break;
	}
}

ConVarManager g_ConVarManager;