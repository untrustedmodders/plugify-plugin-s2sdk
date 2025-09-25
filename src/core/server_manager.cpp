#include "server_manager.hpp"

#include <edict.h>

void ServerManager::OnGameFrame() {
	if (m_nextTasks.empty())
		return;

	//S2_LOGF(LS_DEBUG, "Executing queued tasks of size: {} on tick number {}\n", m_nextTasks.size(), gpGlobals->tickcount);

	for (const auto& [nextTask, userData]: m_nextTasks) {
		nextTask(userData);
	}

	m_nextTasks.clear();
}

void ServerManager::OnPreWorldUpdate() {
	if (m_nextWorldUpdateTasks.empty())
		return;

	//S2_LOGF(LS_DEBUG, "Executing queued tasks of size: {} at time {}\n", m_nextWorldUpdateTasks.size(), gpGlobals->curtime);

	for (const auto& [nextTask, userData]: m_nextWorldUpdateTasks) {
		nextTask(userData);
	}

	m_nextWorldUpdateTasks.clear();
}

void ServerManager::AddTaskForNextFrame(TaskCallback task, const plg::vector<plg::any>& userData) {
	m_nextTasks.emplace_back(task, userData);
}

void ServerManager::AddTaskForNextWorldUpdate(TaskCallback task, const plg::vector<plg::any>& userData) {
	m_nextWorldUpdateTasks.emplace_back(task, userData);
}

ServerManager g_ServerManager;