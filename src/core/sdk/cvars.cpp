#include "cvars.h"

#include <entity2/entitysystem.h>
#include <igameevents.h>
#include <iserver.h>
#include <netmessages.h>
#include <networkbasetypes.pb.h>
#include <networksystem/inetworkmessages.h>
#include <recipientfilter.h>

#include "entity/cbaseplayerpawn.h"

#include <tier0/memdbgon.h>

#undef CreateEvent

void cvars::NotifyConVar(ConVarRefAbstract conVar, std::string_view value) {
	IGameEvent* pEvent = g_pGameEventManager->CreateEvent("server_cvar");
	if (pEvent == nullptr) {
		return;
	}

	pEvent->SetString("cvarname", conVar.GetName());
	if (conVar.IsFlagSet(FCVAR_PROTECTED)) {
		pEvent->SetString("cvarvalue", "***PROTECTED***");
	} else {
		pEvent->SetString("cvarvalue", value.data());
	}

	g_pGameEventManager->FireEvent(pEvent);
}

void cvars::ReplicateConVar(ConVarRefAbstract conVar, std::string_view value) {
	if (!gpGlobals)
		return;

	for (int i = 0; i < gpGlobals->maxClients; ++i) {
		SendConVarValue(CPlayerSlot(i), conVar.GetName(), value);
	}
}

void cvars::SendConVarValue(CPlayerSlot slot, std::string_view name, std::string_view value) {
	if (g_pEngineServer->GetPlayerNetInfo(slot)) {
		static INetworkMessageInternal* pNetMsg = g_pNetworkMessages->FindNetworkMessagePartial("CNETMsg_SetConVar");
		auto msg = pNetMsg->AllocateMessage()->As<CNETMsg_SetConVar_t>();
		CMsg_CVars_CVar* data = msg->mutable_convars()->add_cvars();
		data->set_name(std::string(name));
		data->set_value(std::string(value));

		msg->Send(slot);
		delete msg;
	}
}

void cvars::SendMultipleConVarValues(CPlayerSlot slot, std::span<const std::string_view> names, std::span<const std::string_view> values, uint32_t size) {
	if (g_pEngineServer->GetPlayerNetInfo(slot)) {
		static INetworkMessageInternal* pNetMsg = g_pNetworkMessages->FindNetworkMessagePartial("CNETMsg_SetConVar");
		auto msg = pNetMsg->AllocateMessage()->As<CNETMsg_SetConVar_t>();
		for (uint32_t i = 0; i < size; ++i) {
			CMsg_CVars_CVar* data = msg->mutable_convars()->add_cvars();
			data->set_name(std::string(names[i]));
			data->set_value(std::string(values[i]));
		}
		msg->Send(slot);
		delete msg;
	}
}

int cvars::SendConVarValueQueryToClient(CPlayerSlot slot, std::string_view cvarName, int queryCvarCookieOverride) {
	if (g_pEngineServer->GetPlayerNetInfo(slot)) {
		static INetworkMessageInternal* pNetMsg = g_pNetworkMessages->FindNetworkMessagePartial("CSVCMsg_GetCvarValue");
		static int iQueryCvarCookieCounter = 0;
		int queryCvarCookie = queryCvarCookieOverride == -1 ? ++iQueryCvarCookieCounter : queryCvarCookieOverride;

		auto msg = pNetMsg->AllocateMessage()->As<CSVCMsg_GetCvarValue_t>();
		msg->set_cookie(queryCvarCookie);
		msg->set_cvar_name(std::string(cvarName));

		msg->Send(slot);
		delete msg;

		return queryCvarCookie;
	}

	return -1;
}
