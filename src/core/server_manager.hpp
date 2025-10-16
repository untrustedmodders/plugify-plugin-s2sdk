#pragma once

#include <mutex>
#include <thread>

using TaskCallback = void (*)(const plg::vector<plg::any>&);

class ServerManager {
public:
	void AddTaskForNextWorldUpdate(TaskCallback task, const plg::vector<plg::any>& userData = {});
	void AddTaskForNextFrame(TaskCallback task, const plg::vector<plg::any>& userData = {});

	void OnGameFrame();
	void OnPreWorldUpdate();

private:
	struct Task {
		TaskCallback callback;
		plg::vector<plg::any> userData;

		bool operator<(const Task& other) const {
			return callback < other.callback;
		}
	};

	plg::multiset<Task> m_nextWorldUpdateTasks;
	plg::multiset<Task> m_nextTasks;
};

extern ServerManager g_ServerManager;