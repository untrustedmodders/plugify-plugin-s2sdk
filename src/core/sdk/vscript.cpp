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

#include "vscript.h"

#include "entity/cbaseentity.h"

#include "utils.h"

namespace {
	VScriptBinding ScriptConvertFuncPtrToBinding(const ScriptFunctionBinding_t& binding) {
	    VScriptBinding result = {};

#if defined(_MSC_VER)
	    result.funcadr = binding.m_pFunction;
		result.is_virtual = false;
#elif defined(__GNUC__) || defined(__clang__)
	    if (binding.m_flags & SF_MEMBER_FUNC) {
	        struct GnuMFP {
	            union {
	                void* funcadr;
	                intptr_t vti_plus1;
	            };
	            intptr_t delta;
	        };

	        GnuMFP* p = (GnuMFP*)&binding.m_pFunction;

	        if (p->vti_plus1 & 1) {
	            // Virtual function
	            result.is_virtual = true;
	            result.vtable_index = (p->vti_plus1 - 1) / sizeof(void*);
	        	result.delta = p->delta;
	            result.funcadr = nullptr;
	        } else {
	            // Non-virtual
	            result.funcadr = p->funcadr;
	            result.is_virtual = false;
	        }
	    } else {
    		result.funcadr = binding.m_pFunction;
    		result.is_virtual = false;
	    }
#endif

	    return result;
	}

	using VScriptFuncMap = plg::flat_hash_map<plg::string, VScriptBinding, plg::string_hash, std::equal_to<>>;
	using VScriptTableMap = plg::flat_hash_map<plg::string, VScriptFuncMap, plg::string_hash, std::equal_to<>>;

	VScriptFuncMap scriptFuncMap;
	VScriptTableMap scriptTableMap;
}// namespace

namespace vscript {
	void RegisterFunction(ScriptFunctionBinding_t* pScriptFunction) {
		std::string_view funcName = pScriptFunction->m_desc.m_pszFunction;
		if (scriptTableMap.contains(funcName)) {
			return;
		}

		auto binding = ScriptConvertFuncPtrToBinding(*pScriptFunction);
		scriptFuncMap.emplace(funcName, binding);

#if 1
		// Pretty print registration info
		plg::print(LS_DETAILED, "┌─ Registered Function ────────────────────────────────\n");
		plg::print(LS_DETAILED, "│ Name:        {}\n", pScriptFunction->m_desc.m_pszFunction);
		plg::print(LS_DETAILED, "│ Description: {}\n", pScriptFunction->m_desc.m_pszDescription ? 
										   pScriptFunction->m_desc.m_pszDescription : "(none)");
		plg::print(LS_DETAILED, "│ Returns:     {}\n", GetScriptTypeName(pScriptFunction->m_desc.m_ReturnType));
		plg::print(LS_DETAILED, "│ Parameters:  {}\n", pScriptFunction->m_desc.m_iParamCount);
		plg::print(LS_DETAILED, "│ Address:     {:#x}\n", reinterpret_cast<uintptr_t>(binding.funcadr));
		plg::print(LS_DETAILED, "│ Virtual:     {}\n", binding.is_virtual ? "yes" : "no");
		if (binding.is_virtual) {
			plg::print(LS_DETAILED, "│ VTable idx:  {}\n", binding.vtable_index);
		}
		plg::print(LS_DETAILED, "└──────────────────────────────────────────────────────\n\n");
#endif
	}

	void RegisterScriptClass(ScriptClassDesc_t* pClassDesc) {
		std::string_view className = pClassDesc->m_pszClassname;
		if (scriptTableMap.contains(className)) {
			return;
		}

		auto createMap = [&]() {
			VScriptFuncMap functional;
			functional.reserve(static_cast<size_t>(pClassDesc->m_FunctionBindings.Count()));
			for (const auto& function : pClassDesc->m_FunctionBindings) {
				//(pScriptFunction)->m_pFunction = ScriptConvertFuncPtrToVoid( &class::func );
				functional.emplace(function.m_desc.m_pszFunction, ScriptConvertFuncPtrToBinding(function));
			}
			return functional;
		};
		scriptTableMap.emplace(className, createMap());

#if 1
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
	        for (const auto& function : pClassDesc->m_FunctionBindings) {
	            auto binding = ScriptConvertFuncPtrToBinding(function);
	            
	            plg::print(LS_DETAILED, "║ [{:2}] {:<30} ", idx++, function.m_desc.m_pszFunction);
	            plg::print(LS_DETAILED, "{} -> {}", 
	                      function.m_desc.m_iParamCount > 0 ?
	                          std::format("({} params)", function.m_desc.m_iParamCount) : "()",
	                      GetScriptTypeName(function.m_desc.m_ReturnType));
	            
	            if (binding.is_virtual) {
	                plg::print(LS_DETAILED, " [virtual]");
	            }
	            plg::print(LS_DETAILED, "\n");
	        }
	    }
	    
	    plg::print(LS_DETAILED, "╚═════════════════════════════════════════════════════════════\n\n");
#endif
	}

	VScriptBinding GetFunctionBinding(std::string_view className) {
		auto it = scriptFuncMap.find(className);
		if (it != scriptFuncMap.end()) {
			return it->second;
		}
		plg::print(
			LS_ERROR,
			"vscript::GetFunctionBinding(): '{}' was not found!\n",
			className
		);
		return {};
	}

	VScriptBinding GetFunctionBinding(std::string_view className, std::string_view memberName) {
		auto it = scriptTableMap.find(className);
		if (it != scriptTableMap.end()) {
			auto& table = it->second;
			auto it2 = table.find(memberName);
			if (it2 != table.end()) {
				return it2->second;
			}
			plg::print(
				LS_ERROR,
				"vscript::GetFunctionBinding(): '{}' was not found in '{}'!\n",
				memberName,
				className
			);
		}
		return {};
	}
}
