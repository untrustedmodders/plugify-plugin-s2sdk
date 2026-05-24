#pragma once

#include <polyhook/polyhook.hpp>
#include <igamesystem.h>
#include <igamesystemfactory.h>
#include <networksystem/inetworkmessages.h>

class Source2SDK final : public plg::Plugin {
public:
	plg::PluginResult OnPluginStart() override;
	plg::PluginResult OnPluginEnd() override;
};