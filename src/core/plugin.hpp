#pragma once

#include <polyhook/polyhook.hpp>

#include <igamesystem.h>
#include <igamesystemfactory.h>
#include <networksystem/inetworkmessages.h>

class Source2SDK final : public plg::IPluginEntry {
public:
	void OnPluginStart() final;
	void OnPluginEnd() final;
	static void OnServerStartup();
};