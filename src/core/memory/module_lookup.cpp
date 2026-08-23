#include "module_lookup.hpp"
#include "os.h"

ModuleHandle ModuleLookup::FindModule(std::string_view name) {
#if PLATFORM_WINDOWS
	HMODULE h = GetModuleHandleA(name.data());
	return static_cast<ModuleHandle>(h);
#else
	void* h = dlopen(name.data(), RTLD_NOLOAD | RTLD_NOW);
	return h;
#endif
}

SymbolPtr ModuleLookup::FindSymbol(ModuleHandle module, std::string_view symbol) {
	if (!module) return nullptr;

#if PLATFORM_WINDOWS
	FARPROC proc = GetProcAddress(static_cast<HMODULE>(module), symbol.data());
	return reinterpret_cast<SymbolPtr>(proc);
#else
	dlerror();
	void* sym = dlsym(module, symbol.data());
	const char* err = dlerror();
	if (err != nullptr) return nullptr;
	return sym;
#endif
}

SymbolPtr ModuleLookup::FindSymbolFromModule(std::string_view module, std::string_view symbol) {
	ModuleHandle mod = FindModule(module);
	if (!mod) return nullptr;
	return FindSymbol(mod, symbol);
}