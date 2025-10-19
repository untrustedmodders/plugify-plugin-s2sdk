#pragma once

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
	};

	plg::hybrid_vector<Task, 16> m_nextWorldUpdateTasks;
	plg::hybrid_vector<Task, 16> m_nextTasks;

	std::mutex m_worldUpdateMutex;
	std::mutex m_frameTasksMutex;
};

extern ServerManager g_ServerManager;