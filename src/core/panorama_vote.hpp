#pragma once

#include <ehandle.h>
#include <igameevents.h>
#include <core/sdk/entity/cvotecontroller.h>

enum CastVote {
	VOTE_NOTINCLUDED = -1,
	VOTE_OPTION1, // Use this for Yes
	VOTE_OPTION2, // Use this for No
	VOTE_OPTION3,
	VOTE_OPTION4,
	VOTE_OPTION5,
	VOTE_UNCAST = 5
};

enum class VoteEndReason {
	AllVotes,  // All possible votes were cast
	TimeUp,	   // Time ran out
	Cancelled, // The vote got cancelled
};

enum class VoteAction {
	Start, // nothing passed
	Vote,  // param1 = client slot, param2 = choice (VOTE_OPTION1=yes, VOTE_OPTION2=no)
	End,   // param1 = -1, param2 = YesNoVoteEndReason reason why the vote ended
};

// Used to handle events relating to the vote menu
using YesNoVoteHandler = void(*)(VoteAction, int, int);

// Sends the vote results when the vote ends
// Return true for vote to pass, false to fail
using YesNoVoteResult = bool(*)(int numVotes, int yesVotes, int noVotes, int numClients, const plg::vector<int>& clientInfoSlot, const plg::vector<int>& clientInfoItem);

class PanoramaVoteHandler
{
public:
	void Reset();
	void Init();
	void VoteCast(IGameEvent* event);

	void RemovePlayerFromVote(CPlayerSlot slot);
	bool IsPlayerInVotePool(CPlayerSlot slot) const;
	// Removes a client's vote and redraws the vote
	// True if client was in the vote, false if no ongoing vote
	bool RedrawVoteToClient(CPlayerSlot slot);
	/* Returns true if a vote is in progress, false otherwise */
	bool IsVoteInProgress() const;
	// Ends the ongoing vote with the specified reason
	void EndVote(VoteEndReason reason);

	/**
	 * @brief Start a new Yes/No vote
	 *
	 * @param duration		Maximum time to leave vote active for
	 * @param caller		Player slot of the vote caller. Use VOTE_CALLER_SERVER for 'Server'.
	 * @param voteTitle		Translation string to use as the vote message. (Only '#SFUI_vote' or '#Panorama_vote' strings)
	 * @param detailStr		Extra string used in some vote translation strings.
	 * @param votePassTitle	Translation string to use as the vote message when the vote passes. (Only '#SFUI_vote' or '#Panorama_vote' strings)
	 * @param detailPassStr Extra string used in some vote translation strings when the vote passes.
	 * @param recipients	Recipient filter with all the clients who are allowed to participate in the vote.
	 * @param result		Called when a menu action is completed.
	 * @param handler		Called when the vote has finished.
	 */
	bool SendYesNoVote(
		double duration, int caller,
		std::string_view voteTitle, std::string_view detailStr,
		std::string_view votePassTitle, std::string_view detailPassStr,
		int voteFailReason,
		uint64 recipients, YesNoVoteResult result, YesNoVoteHandler handler
	);

protected:
	void CheckForEarlyVoteClose() const;
	void InitVoters(IRecipientFilter* filter);
	void SendVoteStartUM(IRecipientFilter* filter);
	void UpdateVoteCounts() const;
	void SendVoteFailed() const;
	void SendVotePassed() const;

private:
	CHandle<CVoteController> m_voteController;
	YesNoVoteHandler m_voteHandler{};
	YesNoVoteResult m_voteResult{};
	uint64 m_recipients{static_cast<uint64>(-1)};
	bool m_voteInProgress{};
	//int m_voterCount{};
	int m_voteCount{};
	int m_currentVoteCaller{};
	int m_currentVoteFailReason{};
	std::string m_currentVoteTitle;
	std::string m_currentVoteDetailStr;
	std::string m_currentVotePassTitle;
	std::string m_currentVotePassDetailStr;
	std::inplace_vector<CPlayerSlot, MAXPLAYERS> m_voters;
};

extern PanoramaVoteHandler g_PanoramaVoteHandler;
