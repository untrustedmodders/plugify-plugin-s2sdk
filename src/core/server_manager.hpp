#pragma once

using TaskCallback = void (*)(const plg::vector<plg::any>& userData);

class ServerManager {
	ServerManager() = default;
	~ServerManager() = default;
	NONCOPYABLE(ServerManager)

public:
	static auto& Instance() {
		static ServerManager instance;
		return instance;
	}

	void AddTaskForNextWorldUpdate(TaskCallback task, const plg::vector<plg::any>& userData = {});
	void AddTaskForNextFrame(TaskCallback task, const plg::vector<plg::any>& userData = {});

	void OnGameFrame();
	void OnPreWorldUpdate();

private:
	struct Task {
		TaskCallback callback;
		plg::vector<plg::any> userData;
	};

	std::vector<Task> m_nextWorldUpdateTasks;
	std::vector<Task> m_nextTasks;

	std::mutex m_worldUpdateMutex;
	std::mutex m_frameTasksMutex;
};
inline ServerManager& g_ServerManager = ServerManager::Instance();
