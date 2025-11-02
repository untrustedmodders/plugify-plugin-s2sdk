#pragma once

#include <eiface.h>
#include <entitysystem.h>
#include <igameevents.h>
#include <iserver.h>

class CBaseEntity;
class CServerSideClientBase;

namespace utils {
	CBaseEntity* FindEntityByClassname(CEntityInstance* start, const char* name);

	CBasePlayerController* GetController(CBaseEntity* entity);
	CBasePlayerController* GetController(CPlayerSlot slot);
	bool IsPlayerSlot(CPlayerSlot slot);

	CPlayerSlot GetEntityPlayerSlot(CBaseEntity* entity);
	CUtlClientVector* GetClientList();
	CServerSideClientBase* GetClientBySlot(CPlayerSlot slot);

	// Normalize the angle between -180 and 180.
	float NormalizeDeg(float a);
	// Gets the difference in angle between 2 angles.
	// c can be PI (for radians) or 180.0 (for degrees);
	float GetAngleDifference(float x, float y, float c, bool relative = false);

	// Print functions
	bool CFormat(char* buffer, uint64_t buffer_size, const char* text);
	void ClientPrintFilter(IRecipientFilter* filter, int msgDest, const char* msgName);
	void PrintConsole(CPlayerSlot slot, std::string_view message);
	void PrintChat(CPlayerSlot slot, std::string_view message);
	void PrintCentre(CPlayerSlot slot, std::string_view message);
	void PrintAlert(CPlayerSlot slot, std::string_view message);
	void PrintHtmlCentre(CPlayerSlot slot, std::string_view message, int duration);// This one uses HTML formatting.
	void PrintConsoleAll(std::string_view message);
	void PrintChatAll(std::string_view message);
	void PrintCentreAll(std::string_view message);
	void PrintAlertAll(std::string_view message);
	void PrintHtmlCentreAll(std::string_view message, int duration);// This one uses HTML formatting.

	// Color print
	void CPrintChat(CPlayerSlot slot, std::string_view message);
	void CPrintChatAll(std::string_view message);

	// Sounds
	void PlaySoundToClient(CPlayerSlot player, int channel, const char* soundName, float volume, soundlevel_t soundLevel, int flags, int pitch, const Vector& origin, float soundTime);

	// Return true if the spawn found is truly valid (not in the ground or out of bounds)
	bool IsSpawnValid(const Vector& origin);
	bool FindValidSpawn(Vector& origin, QAngle& angles);

	// Helper to convert plg::vector<uint8_t> to CUtlBuffer
	inline CUtlBuffer CreateUtlBufferFromVector(const plg::vector<uint8_t>& data) {
		CUtlBuffer buffer;
		if (!data.empty()) {
			buffer.Put(data.data(), static_cast<int>(data.size()));
			buffer.SeekGet(CUtlBuffer::SEEK_HEAD, 0); // Reset read position
		}
		return buffer;
	}

	// Helper to convert CUtlBuffer to plg::vector<uint8_t>
	inline void ConvertUtlBufferToVector(const CUtlBuffer* buffer, plg::vector<uint8_t>& output) {
		output.clear();
		if (buffer && buffer->TellPut() > 0) {
			output.assign(static_cast<const uint8_t*>(buffer->Base()),
				     static_cast<const uint8_t*>(buffer->Base()) + buffer->TellPut());
		}
	}

	// Helper to convert plg::string to CUtlString and back
	inline void ConvertUtlStringToPlgString(const CUtlString* utl_str, plg::string& plg_str) {
		if (utl_str && utl_str->Get()) {
			plg_str = plg::string(utl_str->Get(), static_cast<size_t>(utl_str->Length()));
		} else {
			plg_str.clear();
		}
	}

	const fs::path& GameDirectory();

}// namespace utils

enum class HudDest {
	Notify = 1,
	Console = 2,
	Chat = 3,
	Center = 4,
	Alert = 6,
};

enum class FieldType {
	Auto,
	Float32,
	Float64,
	Int32,
	UInt32,
	Int64,
	UInt64,
	Boolean,
	Character,
	String,
	CString,
	HScript,
	EHandle,
	Resource,
	Vector3d,
	Vector2d,
	Vector4d,
	Color32,
	QAngle,
	Quaternion
};
