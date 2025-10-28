/**
* =============================================================================
 * s2sdk
 * Copyright (C) 2023-2025 untrustedmodders
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "vscript.hpp"
#include "utils.hpp"

namespace {
	VScriptBinding ScriptConvertFuncPtrToBinding(ScriptFunctionBinding_t& pScriptFunction) {
	    VScriptBinding result = {};

#if defined(_MSC_VER)
	    result.funcadr = binding.m_pFunction;
		result.is_virtual = false;
#elif defined(__GNUC__) || defined(__clang__)
	    if (pScriptFunction.m_flags & SF_MEMBER_FUNC) {
	        struct GnuMFP {
	            union {
	                void* funcadr;
	                intptr_t vti_plus1;
	            };
	            intptr_t delta;
	        };

	        const GnuMFP* p = reinterpret_cast<const GnuMFP*>(&pScriptFunction.m_pFunction);

	        if (p->vti_plus1 & 1) {
	            // Virtual function
	            result.vtable_index = (p->vti_plus1 - 1) / sizeof(void*);
	        	result.delta = p->delta;
	            result.funcadr = nullptr;
	        } else {
	            // Non-virtual
	            result.funcadr = p->funcadr;
	            result.vtable_index = -1;
	        }
	    } else {
    		result.funcadr = pScriptFunction.m_pFunction;
    		result.vtable_index = -1;
	    }
#endif

		result.binding = &pScriptFunction;
	    return result;
	}

	VScriptClassMap scriptClassMap;
	ScriptClassDesc_t* lastClass;
	void* lastInstance;

}// namespace

namespace vscript {
	void RegisterFunction(ScriptFunctionBinding_t* pScriptFunction) {
		VScriptClass& globalClass = scriptClassMap[""];

		std::string_view funcName = pScriptFunction->m_desc.m_pszScriptName;
		if (globalClass.m_functions.contains(funcName)) {
			return;
		}

		auto binding = ScriptConvertFuncPtrToBinding(*pScriptFunction);

#if 0
		// Pretty print registration info
		plg::print(LS_DETAILED, "┌─ Registered Function ────────────────────────────────\n");
		plg::print(LS_DETAILED, "│ Name:        {}\n", pScriptFunction->m_desc.m_pszScriptName);
		plg::print(LS_DETAILED, "│ Description: {}\n", pScriptFunction->m_desc.m_pszDescription ? 
										   pScriptFunction->m_desc.m_pszDescription : "(none)");
		plg::print(LS_DETAILED, "│ Returns:     {}\n", GetScriptTypeName(pScriptFunction->m_desc.m_ReturnType));
		plg::print(LS_DETAILED, "│ Parameters:  {}\n", pScriptFunction->m_desc.m_iParamCount);
		plg::print(LS_DETAILED, "│ Address:     {:#x}\n", reinterpret_cast<uintptr_t>(binding.funcadr));
		plg::print(LS_DETAILED, "│ Virtual:     {}\n", binding.vtable_index != -1 ? "yes" : "no");
		if (binding.vtable_index != -1) {
			plg::print(LS_DETAILED, "│ VTable idx:  {}\n", binding.vtable_index);
		}
		plg::print(LS_DETAILED, "└──────────────────────────────────────────────────────\n\n");
#endif

		globalClass.m_functions.emplace(funcName, binding);
	}

	void RegisterScriptClass(ScriptClassDesc_t* pClassDesc, void* pInstance) {
		std::string_view className = pClassDesc->m_pszScriptName;
		if (scriptClassMap.contains(className)) {
			return;
		}

		auto createClass = [&]() {
			VScriptClass scriptClass;
			scriptClass.m_descriptor = pClassDesc;
			scriptClass.m_instance = pInstance;
			scriptClass.m_functions.reserve(static_cast<size_t>(pClassDesc->m_FunctionBindings.Count()));
			for (auto& function : pClassDesc->m_FunctionBindings) {
				scriptClass.m_functions.emplace(function.m_desc.m_pszScriptName, ScriptConvertFuncPtrToBinding(function));
			}
			return scriptClass.m_functions;
		};

#if 0
		// Pretty print class registration
	    plg::print(LS_DETAILED, "╔═════════════════════════════════════════════════════════════\n");
	    plg::print(LS_DETAILED, "║ Registered Script Class\n");
	    plg::print(LS_DETAILED, "╠═════════════════════════════════════════════════════════════\n");
	    plg::print(LS_DETAILED, "║ Script Name:  {}\n", pClassDesc->m_pszScriptName ? pClassDesc->m_pszScriptName : "(none)");
	    plg::print(LS_DETAILED, "║ Class Name:   {}\n", pClassDesc->m_pszClassname);
	    plg::print(LS_DETAILED, "║ Description:  {}\n", pClassDesc->m_pszDescription ? pClassDesc->m_pszDescription : "(none)");
	    plg::print(LS_DETAILED, "║ Base Class:   {}\n", pClassDesc->m_pBaseDesc ? pClassDesc->m_pBaseDesc->m_pszClassname : "(none)");
	    plg::print(LS_DETAILED, "║ Functions:    {}\n", pClassDesc->m_FunctionBindings.Count());
	    plg::print(LS_DETAILED, "║ Constructor:  {:#x}\n", reinterpret_cast<uintptr_t>(pClassDesc->m_pfnConstruct));
	    plg::print(LS_DETAILED, "║ Destructor:   {:#x}\n", reinterpret_cast<uintptr_t>(pClassDesc->m_pfnDestruct));
	    plg::print(LS_DETAILED, "║ Helper:       {}\n", pClassDesc->pHelper ? "yes" : "no");
	    plg::print(LS_DETAILED, "╠═════════════════════════════════════════════════════════════\n");
	    
	    // List all member functions
	    if (pClassDesc->m_FunctionBindings.Count() > 0) {
	        plg::print(LS_DETAILED, "║ Member Functions:\n");
	        plg::print(LS_DETAILED, "╠═════════════════════════════════════════════════════════════\n");
	        
	        int idx = 1;
	        for (auto& function : pClassDesc->m_FunctionBindings) {
	            auto binding = ScriptConvertFuncPtrToBinding(function);
	            
	            plg::print(LS_DETAILED, "║ [{:2}] {:<30} ", idx++, function.m_desc.m_pszScriptName);
	            plg::print(LS_DETAILED, "{} -> {}", 
	                      function.m_desc.m_iParamCount > 0 ?
	                          std::format("({} params)", function.m_desc.m_iParamCount) : "()",
	                      GetScriptTypeName(function.m_desc.m_ReturnType));
	            
	            if (binding.vtable_index != -1) {
	                plg::print(LS_DETAILED, " [virtual]");
	            }
	            plg::print(LS_DETAILED, "\n");
	        }
	    }
	    
	    plg::print(LS_DETAILED, "╚═════════════════════════════════════════════════════════════\n\n");
#endif

		scriptClassMap.emplace(className, createClass());
	}

	VScriptBinding GetBinding(std::string_view functionName) {
		VScriptClass& globalClass = scriptClassMap[""];
		auto it = globalClass.m_functions.find(functionName);
		if (it != globalClass.m_functions.end()) {
			return it->second;
		}
		plg::print(
			LS_ERROR,
			"vscript::GetBinding(): '{}' was not found!\n",
			functionName
		);
		return {};
	}

	VScriptBinding GetBinding(std::string_view className, std::string_view functionName) {
		auto it = scriptClassMap.find(className);
		if (it != scriptClassMap.end()) {
			auto& table = it->second;
			auto it2 = table.m_functions.find(functionName);
			if (it2 != table.m_functions.end()) {
				return it2->second;
			}
		}
		plg::print(
			LS_ERROR,
			"vscript::GetBinding(): '{}' was not found in '{}'!\n",
			functionName,
			className
		);
		return {};
	}

}
