#pragma once

#include "schema.h"
#include <eiface.h>
#include <iserver.h>

#if S2SDK_PLATFORM_LINUX || S2SDK_PLATFORM_APPLE
#include <cxxabi.h>
#endif

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

	const fs::path& GameDirectory();

}// namespace utils

enum class FieldType : int {
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
