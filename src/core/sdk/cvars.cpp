#include "cvars.hpp"

#include <entity2/entitysystem.h>
#include <igameevents.h>
#include <iserver.h>
#include <netmessages.h>
#include <networkbasetypes.pb.h>
#include <networksystem/inetworkmessages.h>
#include <recipientfilter.h>

#include "entity/cbaseplayerpawn.h"

#undef CreateEvent

void cvars::NotifyConVar(ConVarRefAbstract conVar, std::string_view value) {
	IGameEvent* event = g_pGameEventManager->CreateEvent("server_cvar");
	if (event == nullptr) {
		return;
	}

	event->SetString("cvarname", conVar.GetName());
	if (conVar.IsFlagSet(FCVAR_PROTECTED)) {
		event->SetString("cvarvalue", "***PROTECTED***");
	} else {
		event->SetString("cvarvalue", value.data());
	}

	g_pGameEventManager->FireEvent(event);
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
		static INetworkMessageInternal* netMsg = g_pNetworkMessages->FindNetworkMessagePartial("CNETMsg_SetConVar");
		auto msg = netMsg->AllocateMessage()->As<CNETMsg_SetConVar_t>();
		CMsg_CVars_CVar* data = msg->mutable_convars()->add_cvars();
		data->set_name(std::string(name));
		data->set_value(std::string(value));

		UNUSED(msg->Send(slot));
		delete msg;
	}
}

void cvars::SendMultipleConVarValues(CPlayerSlot slot, std::span<const std::string_view> names, std::span<const std::string_view> values, uint32_t size) {
	if (g_pEngineServer->GetPlayerNetInfo(slot)) {
		static INetworkMessageInternal* netMsg = g_pNetworkMessages->FindNetworkMessagePartial("CNETMsg_SetConVar");
		auto msg = netMsg->AllocateMessage()->As<CNETMsg_SetConVar_t>();
		for (uint32_t i = 0; i < size; ++i) {
			CMsg_CVars_CVar* data = msg->mutable_convars()->add_cvars();
			data->set_name(std::string(names[i]));
			data->set_value(std::string(values[i]));
		}

		UNUSED(msg->Send(slot));
		delete msg;
	}
}

int cvars::SendConVarValueQueryToClient(CPlayerSlot slot, std::string_view cvarName, int queryCvarCookieOverride) {
	if (g_pEngineServer->GetPlayerNetInfo(slot)) {
		static INetworkMessageInternal* netMsg = g_pNetworkMessages->FindNetworkMessagePartial("CSVCMsg_GetCvarValue");
		static int iQueryCvarCookieCounter = 0;
		int queryCvarCookie = queryCvarCookieOverride == -1 ? ++iQueryCvarCookieCounter : queryCvarCookieOverride;

		auto msg = netMsg->AllocateMessage()->As<CSVCMsg_GetCvarValue_t>();
		msg->set_cookie(queryCvarCookie);
		msg->set_cvar_name(std::string(cvarName));

		UNUSED(msg->Send(slot));
		delete msg;

		return queryCvarCookie;
	}

	return -1;
}
