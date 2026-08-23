#pragma once

#include <string_view>

using ModuleHandle = void*;
using SymbolPtr    = void*;

class ModuleLookup {
public:
    static ModuleHandle FindModule(std::string_view name);

    static SymbolPtr FindSymbol(ModuleHandle module, std::string_view symbol);
 
    static SymbolPtr FindSymbolFromModule(std::string_view module, std::string_view symbol);
 
    template <typename FuncPtr>
    static FuncPtr FindSymbolAs(std::string_view module, std::string_view symbol) {
        return reinterpret_cast<FuncPtr>(FindSymbolFromModule(module, symbol));
    }
};
