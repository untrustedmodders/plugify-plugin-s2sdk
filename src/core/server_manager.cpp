#include "server_manager.hpp"

#include <edict.h>

void ServerManager::OnGameFrame() {
	std::vector<Task> tasksToExecute;

	{
		std::scoped_lock lock(m_frameTasksMutex);
		if (m_nextTasks.empty())
			return;

		tasksToExecute.swap(m_nextTasks);
	}

	plg::print(LS_DETAILED, "Executing queued tasks of size: {} on tick number {}\n", tasksToExecute.size(), gpGlobals->tickcount);

	for (const auto& [callback, userData] : tasksToExecute) {
		callback(userData);
	}
}

void ServerManager::OnPreWorldUpdate() {
	std::vector<Task> tasksToExecute;

	{
		std::scoped_lock lock(m_worldUpdateMutex);
		if (m_nextWorldUpdateTasks.empty())
			return;

		tasksToExecute.swap(m_nextWorldUpdateTasks);
	}

	plg::print(LS_DETAILED, "Executing queued tasks of size: {} at time {}\n", tasksToExecute.size(), gpGlobals->curtime);

	for (const auto& [callback, userData] : tasksToExecute) {
		callback(userData);
	}
}

void ServerManager::AddTaskForNextFrame(TaskCallback task, const plg::vector<plg::any>& userData) {
	std::scoped_lock lock(m_frameTasksMutex);
	m_nextTasks.emplace_back(task, userData);
}

void ServerManager::AddTaskForNextWorldUpdate(TaskCallback task, const plg::vector<plg::any>& userData) {
	std::scoped_lock lock(m_worldUpdateMutex);
	m_nextWorldUpdateTasks.emplace_back(task, userData);
}

ServerManager g_ServerManager;