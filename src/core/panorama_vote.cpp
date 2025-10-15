#include "panorama_vote.hpp"
#include "event_listener.hpp"
#include "server_manager.hpp"
#include "timer_system.hpp"

#include <core/sdk/entity/cplayercontroller.h>
#include <engine/igameeventsystem.h>
#include <networksystem/inetworkmessages.h>
#include <recipientfilter.h>
#include <cstrike15_usermessages.pb.h>
#include <cstrike15/usermessages.h>

void PanoramaVoteHandler::Reset() {
	m_voteInProgress = false;
	m_voteController = nullptr;
	m_voteHandler = nullptr;
	m_voteResult = nullptr;
	m_currentVoteTitle.clear();
	m_currentVoteDetailStr.clear();
}

void PanoramaVoteHandler::Init() {
	if (m_voteInProgress)
		return;

	CVoteController* voteController = nullptr;
	while (nullptr != (voteController = static_cast<CVoteController *>(utils::FindEntityByClassname(voteController, "vote_controller")))) {
		m_voteController = voteController->GetHandle();
	}
}

// Called by vote_cast event
void PanoramaVoteHandler::VoteCast(IGameEvent *event) {
	if (!m_voteController || !m_voteInProgress)
		return;

	if (m_voteHandler != nullptr) {
		CPlayerController* controller = static_cast<CPlayerController*>(event->GetPlayerController("userid"));
		if (!controller)
			return;

		m_voteHandler(VoteAction::Vote, controller->GetPlayerSlot(), event->GetInt("vote_option"));
	}

	CheckForEarlyVoteClose();
}

void PanoramaVoteHandler::RemovePlayerFromVote(CPlayerSlot slot) {
	if (!m_voteInProgress)
		return;

	auto it = std::find(m_voters.begin(), m_voters.end(), slot);
	if (it != m_voters.end()) {
		m_voters.erase(it);
	}
}

bool PanoramaVoteHandler::IsPlayerInVotePool(CPlayerSlot slot) const {
	if (!m_voteInProgress)
		return false;

	auto it = std::find(m_voters.begin(), m_voters.end(), slot);
	return it != m_voters.end();
}

// Removes a client's vote and redraws the vote
bool PanoramaVoteHandler::RedrawVoteToClient(CPlayerSlot slot) {
	if (!m_voteController)
		return false;

	int myVote = m_voteController->m_nVotesCast[slot];
	if (myVote != VOTE_UNCAST) {
		m_voteController->m_nVotesCast[slot] = VOTE_UNCAST;
		--m_voteController->m_nVoteOptionCount[myVote];

		UpdateVoteCounts();
	}

	CRecipientFilter filter;
	filter.AddRecipient(slot); // CPlayerSlot()
	SendVoteStartUM(&filter);

	return true;
}

void PanoramaVoteHandler::UpdateVoteCounts() const {
	if (IGameEvent* event = g_pGameEventManager->CreateEvent("vote_changed")) {
		event->SetInt("vote_option1", m_voteController->m_nVoteOptionCount[VOTE_OPTION1]);
		event->SetInt("vote_option2", m_voteController->m_nVoteOptionCount[VOTE_OPTION2]);
		event->SetInt("vote_option3", m_voteController->m_nVoteOptionCount[VOTE_OPTION3]);
		event->SetInt("vote_option4", m_voteController->m_nVoteOptionCount[VOTE_OPTION4]);
		event->SetInt("vote_option5", m_voteController->m_nVoteOptionCount[VOTE_OPTION5]);
		event->SetInt("potentialVotes", static_cast<int>(m_voters.size()));

		g_pGameEventManager->FireEvent(event, false);
	}
}

bool PanoramaVoteHandler::IsVoteInProgress() const {
	return m_voteInProgress;
}

bool PanoramaVoteHandler::SendYesNoVote(double duration, int caller,
	std::string_view voteTitle, std::string_view detailStr,
	std::string_view votePassTitle, std::string_view detailPassStr,
	int voteFailReason, uint64 recipients, YesNoVoteResult result,
	YesNoVoteHandler handler = nullptr) {
	if (!m_voteController|| m_voteInProgress)
		return false;

	CRecipientFilter filter;
	if (recipients == static_cast<uint64>(-1)) {
		filter.AddAllPlayers();
	} else {
		filter.SetRecipients(recipients);
	}

	if (filter.GetRecipientCount() <= 0)
		return false;

	if (result == nullptr)
		return false;

	plg::print(LS_MESSAGE, "[Vote Start] Starting a new vote [id:{}]. Duration:{} Caller:{} NumClients:{}", m_voteCount, duration, caller, filter.GetRecipientCount());

	m_voteInProgress = true;
	m_recipients = recipients;

	InitVoters(&filter);

	m_voteController->m_nPotentialVotes = static_cast<int>(m_voters.size());
	m_voteController->m_bIsYesNoVote = true;
	m_voteController->m_iActiveIssueIndex = 2;

	m_voteController->m_iOnlyTeamToVote = -1; // use the recipient filter param to handle who votes

	m_voteResult = result;
	m_voteHandler = handler;

	m_currentVoteCaller = caller;
	m_currentVoteTitle = voteTitle;
	m_currentVoteDetailStr = detailStr;
	m_currentVotePassTitle = votePassTitle;
	m_currentVotePassDetailStr = detailPassStr;
	m_currentVoteFailReason = voteFailReason;

	UpdateVoteCounts();

	SendVoteStartUM(&filter);

	if (m_voteHandler != nullptr)
		m_voteHandler(VoteAction::Start, -1, VOTE_NOTINCLUDED);

	g_TimerSystem.CreateTimer(duration, [](uint32_t, const plg::vector<plg::any>& args) {
		int32_t voteNum = plg::get<int32_t>(args[0]);
		// Ensure we dont end the wrong vote
		if (voteNum == g_PanoramaVoteHandler.m_voteCount)
			g_PanoramaVoteHandler.EndVote(VoteEndReason::TimeUp);
	}, {}, {m_voteCount});

	return true;
}

void PanoramaVoteHandler::SendVoteStartUM(IRecipientFilter* filter) {
	INetworkMessageInternal *netMsg = g_pNetworkMessages->FindNetworkMessagePartial("VoteStart");
	auto data = netMsg->AllocateMessage()->As<CCSUsrMsg_VoteStart_t>();

	data->set_team(-1);
	data->set_player_slot(m_currentVoteCaller);
	data->set_vote_type(-1);
	data->set_disp_str(m_currentVoteTitle);
	data->set_details_str(m_currentVoteDetailStr);
	data->set_is_yes_no_vote(true);

	data->Send(filter->GetRecipients());

	delete data;
}

void PanoramaVoteHandler::InitVoters(IRecipientFilter* filter) {
	// Clear any old info
	m_voters.clear();

	for (int i = 0; i < MAXPLAYERS; ++i) {
		m_voteController->m_nVotesCast[i] = VOTE_UNCAST;
	}

	for (int i = 0; i < VOTE_UNCAST; ++i) {
		m_voteController->m_nVoteOptionCount[i] = 0;
	}

	const CPlayerBitVec& recipients = filter->GetRecipients();
	for (int i = 0; i < filter->GetRecipientCount(); ++i) {
		if (recipients.IsBitSet(i)) {
			m_voters.push_back(i);
		}
	}
}

void PanoramaVoteHandler::CheckForEarlyVoteClose() const {
	int votes = 0;
	votes += m_voteController->m_nVoteOptionCount[VOTE_OPTION1];
	votes += m_voteController->m_nVoteOptionCount[VOTE_OPTION2];

	if (votes >= m_voters.size()) {
		// Do this next frame to prevent a crash
		g_ServerManager.AddTaskForNextFrame([](const plg::vector<plg::any>&) {
			g_PanoramaVoteHandler.EndVote(VoteEndReason::AllVotes);
		});
	}
}

void PanoramaVoteHandler::EndVote(VoteEndReason reason) {
	if (!m_voteInProgress)
		return;

	m_voteInProgress = false;

	switch (reason) {
		case VoteEndReason::AllVotes: {
			plg::print(LS_MESSAGE, "[Vote Ending] [id:{}] All possible players voted.", m_voteCount);
			break;
		}
		case VoteEndReason::TimeUp: {
			plg::print(LS_MESSAGE, "[Vote Ending] [id:{}] Time ran out.", m_voteCount);
			break;
		}
		case VoteEndReason::Cancelled: {
			plg::print(LS_MESSAGE, "[Vote Ending] [id:{}] The vote has been cancelled.", m_voteCount);
			break;
		}
	}

	// Cycle global vote counter
	++m_voteCount;

	if (m_voteHandler != nullptr) {
		m_voteHandler(VoteAction::End, -1, static_cast<int>(reason));
	}

	if (!m_voteController) {
		SendVoteFailed();
		return;
	}

	if (m_voteResult == nullptr || reason == VoteEndReason::Cancelled) {
		SendVoteFailed();
		m_voteController->m_iActiveIssueIndex = -1;
		return;
	}

	int numClients = static_cast<int>(m_voters.size());
	int yesVotes = m_voteController->m_nVoteOptionCount[VOTE_OPTION1];
	int noVotes = m_voteController->m_nVoteOptionCount[VOTE_OPTION2];
	int numVotes = yesVotes + noVotes;

	plg::vector<int> clientInfoSlot;
	plg::vector<int> clientInfoItem;

	clientInfoSlot.reserve(m_voters.size());
	clientInfoItem.reserve(m_voters.size());

	for (const auto& slot : m_voters) {
		clientInfoSlot.push_back(slot);
		clientInfoItem.push_back(m_voteController->m_nVotesCast[slot]);
	}

	bool passed = m_voteResult(numVotes, yesVotes, noVotes, numClients, clientInfoSlot, clientInfoItem);
	if (passed) {
		SendVotePassed();
	} else {
		SendVoteFailed();
	}
}

void PanoramaVoteHandler::SendVoteFailed() const {
	INetworkMessageInternal *netMsg = g_pNetworkMessages->FindNetworkMessagePartial("VoteFailed");

	auto data = netMsg->AllocateMessage()->As<CCSUsrMsg_VoteFailed_t>();

	data->set_reason(m_currentVoteFailReason);
	data->set_team(-1);

	CRecipientFilter filter;
	if (m_recipients == static_cast<uint64>(-1)) {
		filter.AddAllPlayers();
	} else {
		filter.SetRecipients(m_recipients);
	}
	data->Send(filter.GetRecipients());

	delete data;
}

void PanoramaVoteHandler::SendVotePassed() const {
	INetworkMessageInternal *netMsg = g_pNetworkMessages->FindNetworkMessagePartial("VotePass");

	auto data = netMsg->AllocateMessage()->As<CCSUsrMsg_VotePass_t>();

	data->set_team(-1);
	data->set_vote_type(2); // VOTEISSUE_NEXTLEVEL
	data->set_disp_str(m_currentVotePassTitle);
	data->set_details_str(m_currentVotePassDetailStr);

	CRecipientFilter filter;
	if (m_recipients == static_cast<uint64>(-1)) {
		filter.AddAllPlayers();
	} else {
		filter.SetRecipients(m_recipients);
	}
	data->Send(filter.GetRecipients());

	delete data;
}

GAME_EVENT_F(vote_cast) {
	g_PanoramaVoteHandler.VoteCast(event);
}

GAME_EVENT_F(round_start) {
	g_PanoramaVoteHandler.Init();
}

PanoramaVoteHandler g_PanoramaVoteHandler;