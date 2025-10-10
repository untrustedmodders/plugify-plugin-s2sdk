#pragma once

enum class TimerFlag {
	Default = 0,

	Repeat = (1 << 0),
	NoMapChange = (1 << 1)
};

using TimerCallback = void (*)(uint32_t, const plg::vector<plg::any>&);

struct Timer {
	uint32_t id;
	bool repeat;
	bool noMapChange;
	mutable bool exec;
	mutable bool kill;
	//double createTime;
	double executeTime;
	double delay;
	TimerCallback callback;
	plg::vector<plg::any> userData;

	bool operator<(const Timer& other) const {
		return executeTime < other.executeTime ||
			   (executeTime == other.executeTime && id < other.id);
	}
};

class TimerSystem {
public:
	void OnMapEnd();
	void OnGameFrame(bool simulating);

	void RunFrame();
	void RemoveMapChangeTimers();
	static double CalculateNextThink(double lastThinkTime, double delay);
	static double GetTickedTime();
	static double GetTickedInterval();

	uint32_t CreateTimer(double delay, TimerCallback callback, TimerFlag flags = TimerFlag::Default, const plg::vector<plg::any>& userData = {});
	void KillTimer(uint32_t id);
	void RescheduleTimer(uint32_t id, double newDelay);

private:
	bool m_hasMapTicked{};
	bool m_hasMapSimulated{};
	float m_lastTickedTime{};
	plg::set<Timer> m_timers;
	static inline std::atomic<uint32_t> s_nextId{static_cast<uint32_t>(-1)};
};

inline TimerFlag operator|(TimerFlag lhs, TimerFlag rhs) noexcept {
	using underlying = std::underlying_type_t<TimerFlag>;
	return static_cast<TimerFlag>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

inline bool operator&(TimerFlag lhs, TimerFlag rhs) noexcept {
	using underlying = std::underlying_type_t<TimerFlag>;
	return static_cast<underlying>(lhs) & static_cast<underlying>(rhs);
}

inline TimerFlag& operator|=(TimerFlag& lhs, TimerFlag rhs) noexcept {
	lhs = lhs | rhs;
	return lhs;
}

extern TimerSystem g_TimerSystem;