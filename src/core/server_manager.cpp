#include "server_manager.hpp"

#include <edict.h>

void ServerManager::OnGameFrame() {
	if (m_nextTasks.empty())
		return;

	//plg::print(LS_DEBUG, "Executing queued tasks of size: {} on tick number {}\n", m_nextTasks.size(), gpGlobals->tickcount);

	m_nextTasks.for_each([](const Task& task) {
		const auto& [nextTask, userData] = task;
		nextTask(userData);
	});

	m_nextTasks.clear();
}

void ServerManager::OnPreWorldUpdate() {
	if (m_nextWorldUpdateTasks.empty())
		return;

	//plg::print(LS_DEBUG, "Executing queued tasks of size: {} at time {}\n", m_nextWorldUpdateTasks.size(), gpGlobals->curtime);

	m_nextWorldUpdateTasks.for_each([](const Task& task) {
		const auto& [nextTask, userData] = task;
		nextTask(userData);
	});

	m_nextWorldUpdateTasks.clear();
}

void ServerManager::AddTaskForNextFrame(TaskCallback task, const plg::vector<plg::any>& userData) {
	m_nextTasks.emplace(task, userData);
}

void ServerManager::AddTaskForNextWorldUpdate(TaskCallback task, const plg::vector<plg::any>& userData) {
	m_nextWorldUpdateTasks.emplace(task, userData);
}

ServerManager g_ServerManager;