#pragma once

#include <plg/plugin.hpp>
#include <polyhook/polyhook.hpp>
#include <plugin_export.h>

#include <igamesystem.h>
#include <igamesystemfactory.h>
#include <networksystem/inetworkmessages.h>

class Source2SDK final : public plg::IPluginEntry {
public:
	void OnPluginStart() final;
	void OnPluginEnd() final;
	static void OnServerStartup();
};