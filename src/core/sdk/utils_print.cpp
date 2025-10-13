#include "utils.h"

#include "entity/globaltypes.h"
#include <recipientfilter.h>
#include <engine/igameeventsystem.h>
#include <igameevents.h>
#include <networksystem/inetworkmessages.h>
#include <usermessages.pb.h>
#include <usermessages.h>

#include <tier0/memdbgon.h>

int8_t ConvertColorStringToByte(const char* str, size_t length) {
	switch (length) {
		case 3:
			if (!V_memcmp(str, "red", length))
				return 7;
			break;
		case 4:
			if (!V_memcmp(str, "blue", length))
				return 11;
			if (!V_memcmp(str, "lime", length))
				return 6;
			if (!V_memcmp(str, "grey", length))
				return 8;
			if (!V_memcmp(str, "gold", length))
				return 16;
			break;
		case 5:
			if (!V_memcmp(str, "green", length))
				return 4;
			if (!V_memcmp(str, "grey2", length))
				return 13;
			if (!V_memcmp(str, "olive", length))
				return 5;
			break;
		case 6:
			if (!V_memcmp(str, "purple", length))
				return 3;
			if (!V_memcmp(str, "orchid", length))
				return 14;
			if (!V_memcmp(str, "yellow", length))
				return 9;
			break;
		case 7:
			if (!V_memcmp(str, "default", length))
				return 1;
			if (!V_memcmp(str, "darkred", length))
				return 2;
			break;
		case 8:
			if (!V_memcmp(str, "darkblue", length))
				return 12;
			if (!V_memcmp(str, "lightred", length))
				return 15;
			if (!V_memcmp(str, "bluegrey", length))
				return 10;
			break;
	}
	return 0;
}

enum CFormatResult {
	CFORMAT_NOT_US,
	CFORMAT_OK,
	CFORMAT_OUT_OF_SPACE,
};

struct CFormatContext {
	const char* current;
	char* result;
	char* result_end;
};

bool HasEnoughSpace(const CFormatContext* ctx, uintptr_t space) {
	return (uintptr_t) (ctx->result_end - ctx->result) > space;
}

CFormatResult EscapeChars(CFormatContext* ctx) {
	if (*ctx->current == '{' && *(ctx->current + 1) == '{') {
		if (!HasEnoughSpace(ctx, 1))
			return CFORMAT_OUT_OF_SPACE;

		ctx->current += 2;
		*ctx->result++ = '{';
		return CFORMAT_OK;
	}
	return CFORMAT_NOT_US;
}

CFormatResult ParseColors(CFormatContext* ctx) {
	const char* current = ctx->current;
	if (*current == '{') {
		current++;
		const char* start = current;
		while (*current && *current != '}')
			current++;
		if (*current == '}') {
			size_t length = current - start;
			current++;
			if (char byte = ConvertColorStringToByte(start, length); byte) {
				if (!HasEnoughSpace(ctx, 1))
					return CFORMAT_OUT_OF_SPACE;

				*ctx->result++ = byte;
				ctx->current = current;
				return CFORMAT_OK;
			}
		}
	}
	return CFORMAT_NOT_US;
}

CFormatResult ReplaceNewlines(CFormatContext* ctx) {
	if (*ctx->current == '\n') {
		if (!HasEnoughSpace(ctx, 3))
			return CFORMAT_OUT_OF_SPACE;

		ctx->current++;
		*ctx->result++ = '\xe2';
		*ctx->result++ = '\x80';
		*ctx->result++ = '\xa9';
		return CFORMAT_OK;
	}
	return CFORMAT_NOT_US;
}

CFormatResult AddSpace(CFormatContext* ctx) {
	if (!HasEnoughSpace(ctx, 1))
		return CFORMAT_OUT_OF_SPACE;
	*ctx->result++ = ' ';
	return CFORMAT_OK;
}

bool utils::CFormat(char* buffer, uint64_t buffer_size, const char* text) {
	assert(buffer_size != 0);

	CFormatContext ctx {
		.current = text,
		.result = buffer,
		.result_end = buffer + buffer_size,
	};

	if (AddSpace(&ctx) != CFORMAT_OK)
		return false;

	while (*ctx.current) {
		auto escape_chars = EscapeChars(&ctx);
		if (escape_chars == CFORMAT_OK)
			continue;
		if (escape_chars == CFORMAT_OUT_OF_SPACE) {
			return false;
		}

		auto parse_colors = ParseColors(&ctx);
		if (parse_colors == CFORMAT_OK)
			continue;
		if (parse_colors == CFORMAT_OUT_OF_SPACE) {
			return false;
		}

		auto replace_newlines = ReplaceNewlines(&ctx);
		if (replace_newlines == CFORMAT_OK)
			continue;
		if (replace_newlines == CFORMAT_OUT_OF_SPACE) {
			return false;
		}

		// Everything else
		if (!HasEnoughSpace(&ctx, 1)) {
			return false;
		}
		*ctx.result++ = *ctx.current++;
	}

	// Null terminate
	if (!HasEnoughSpace(&ctx, 1)) {
		return false;
	}
	*ctx.result++ = 0;

	return true;
}

void ClientPrint(std::optional<CPlayerSlot> playerSlot, HudDest dest, std::string_view message) {
	INetworkMessageInternal* pNetMsg = g_pNetworkMessages->FindNetworkMessagePartial("TextMsg");
	auto* data = pNetMsg->AllocateMessage()->As<CUserMessageTextMsg_t>();

	data->set_dest(static_cast<uint32_t>(dest));
	data->add_param(std::string(message));

	if (playerSlot){
		data->Send(*playerSlot);
	} else {
		data->SendToAllClients();
	}
	delete data;
}

void utils::PrintConsole(CPlayerSlot slot, std::string_view message) {
	ClientPrint(slot, HudDest::Console, message);
}

void utils::PrintChat(CPlayerSlot slot, std::string_view message) {
	ClientPrint(slot, HudDest::Chat, message);
}

void utils::PrintCentre(CPlayerSlot slot, std::string_view message) {
	ClientPrint(slot, HudDest::Center, message);
}

void utils::PrintAlert(CPlayerSlot slot, std::string_view message) {
	ClientPrint(slot, HudDest::Alert, message);
}

void utils::PrintHtmlCentre(CPlayerSlot slot, std::string_view message, int duration) {
	IGameEvent* event = g_pGameEventManager->CreateEvent("show_survival_respawn_status");
	if (!event) {
		return;
	}

	event->SetString("loc_token", message.data());
	event->SetInt("duration", duration);
	event->SetInt("userid", slot);

	IGameEventListener2* listener = addresses::GetLegacyGameEventListener(slot);
	listener->FireGameEvent(event);

	g_pGameEventManager->FreeEvent(event);
}

void utils::PrintConsoleAll(std::string_view message) {
	ClientPrint(std::nullopt, HudDest::Console, message);
}

void utils::PrintChatAll(std::string_view message) {
	ClientPrint(std::nullopt, HudDest::Chat, message);
}

void utils::PrintCentreAll(std::string_view message) {
	ClientPrint(std::nullopt, HudDest::Center, message);
}

void utils::PrintAlertAll(std::string_view message) {
	ClientPrint(std::nullopt, HudDest::Alert, message);
}

void utils::PrintHtmlCentreAll(std::string_view message, int duration) {
	IGameEvent* event = g_pGameEventManager->CreateEvent("show_survival_respawn_status", true);
	if (!event) {
		return;
	}

	event->SetString("loc_token", message.data());
	event->SetInt("duration", duration);
	event->SetInt("userid", -1);

	g_pGameEventManager->FireEvent(event);
}

void utils::CPrintChat(CPlayerSlot slot, std::string_view message) {
	char coloredBuffer[MAX_LOGGING_MESSAGE_LENGTH];
	if (CFormat(coloredBuffer, sizeof(coloredBuffer), message.data())) {
		ClientPrint(slot, HudDest::Chat, coloredBuffer);
	} else {
		S2_LOGF(LS_WARNING, "utils::CPrintChat did not have enough space to print: {}\n", message);
	}
}

void utils::CPrintChatAll(std::string_view message) {
	char coloredBuffer[MAX_LOGGING_MESSAGE_LENGTH]; 
	if (CFormat(coloredBuffer, sizeof(coloredBuffer), message.data())) {
		ClientPrint(std::nullopt, HudDest::Chat, coloredBuffer);
	} else {
		S2_LOGF(LS_WARNING, "utils::CPrintChatAll did not have enough space to print: {}\n", message);
	}
}