#include "timer_system.hpp"

#include <edict.h>

double universalTime = 0.0f;
double timerNextThink = 0.0f;
const double engineFixedTickInterval = 0.015625;

void TimerSystem::OnMapEnd() {
	RemoveMapChangeTimers();

	m_hasMapSimulated = false;
	m_hasMapTicked = false;
}

void TimerSystem::OnGameFrame(bool simulating) {
	if (simulating && m_hasMapTicked) {
		universalTime += gpGlobals->curtime - m_lastTickedTime;
		m_hasMapSimulated = true;
	} else {
		universalTime += engineFixedTickInterval;
	}

	m_lastTickedTime = gpGlobals->curtime;
	m_hasMapTicked = true;

	// Handle timer tick
	if (universalTime >= timerNextThink) {
		RunFrame();
		timerNextThink = CalculateNextThink(timerNextThink, 0.1);
	}
}

double TimerSystem::CalculateNextThink(double lastThinkTime, double delay) {
	if (universalTime - lastThinkTime - delay <= 0.1) {
		return lastThinkTime + delay;
	} else {
		return universalTime + delay;
	}
}

void TimerSystem::RunFrame() {
	std::scoped_lock lock(m_mutex);

	while (!m_timers.empty()) {
		auto it = m_timers.begin();

		if (universalTime >= it->executeTime) {
			it->exec = true;
			it->callback(it->id, it->userData);
			it->exec = false;

			if (it->repeat && !it->kill) {
				auto node = m_timers.extract(it);
				node.value().executeTime = universalTime + node.value().delay;
				m_timers.insert(std::move(node));
				continue;
			}

			m_timers.erase(it);
		} else {
			break;
		}
	}
}

void TimerSystem::RemoveMapChangeTimers() {
    std::scoped_lock lock(m_mutex);

	for (auto it = m_timers.begin(); it != m_timers.end();) {
		if (it->noMapChange) {
			it = m_timers.erase(it);
		} else {
			++it;
		}
	}
}

uint32_t TimerSystem::CreateTimer(double delay, TimerCallback callback, TimerFlag flags, const plg::vector<plg::any>& userData) {
	std::scoped_lock lock(m_mutex);
	// Enforce minimum delay to prevent immediate execution and iterator invalidation
	uint32_t id = m_nextId++;
	m_timers.emplace(id, flags & TimerFlag::Repeat, flags & TimerFlag::NoMapChange, false, false, universalTime, universalTime + std::max(delay, engineFixedTickInterval), delay, callback, userData);
	return id;
}

void TimerSystem::KillTimer(uint32_t id) {
	std::scoped_lock lock(m_mutex);

	auto it = std::find_if(m_timers.begin(), m_timers.end(), [id](const Timer& timer) {
		return timer.id == id;
	});

	if (it != m_timers.end()) {
		if (it->exec) {
			it->kill = true;
		} else {
			m_timers.erase(it);
		}
	}
}

void TimerSystem::RescheduleTimer(uint32_t id, double newDelay) {
	std::scoped_lock lock(m_mutex);

	auto it = std::find_if(m_timers.begin(), m_timers.end(), [id](const Timer& timer) {
		return timer.id == id;
	});

	if (it != m_timers.end()) {
		if (!it->exec) {
			auto node = m_timers.extract(it);
			node.value().delay = newDelay;
			node.value().executeTime = universalTime + newDelay;
			m_timers.insert(std::move(node));
		}
	}
}


double TimerSystem::GetTickedTime() {
	return universalTime;
}

double TimerSystem::GetTickedInterval() {
	return engineFixedTickInterval;
}

TimerSystem g_TimerSystem;