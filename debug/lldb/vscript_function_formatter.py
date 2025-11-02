"""
=============================================================================
s2sdk
Copyright (C) 2023-2025 untrustedmodders
=============================================================================

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 3.0, as published by the
Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <http://www.gnu.org/licenses/>.
"""

import lldb

_type_field_offsets = {}
_script_binding_cache = {}

class ScriptFunctionSynthProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.cached_value = None
        self.update()

    def update(self):
        try:
            self._resolve_via_vscript()
        except Exception as e:
            print(f"EXCEPTION: {e}")
            import traceback
            traceback.print_exc()
            self.cached_value = None

    def _get_member_offset(self, parent_type, field_name):
        key = (parent_type.GetName(), field_name)
        if key in _type_field_offsets:
            return _type_field_offsets[key]
        for i in range(parent_type.GetNumberOfFields()):
            member = parent_type.GetFieldAtIndex(i)
            if member.GetName() == field_name:
                offset = member.GetOffsetInBytes()
                _type_field_offsets[key] = offset
                return offset
        return None

    def _get_script_binding(self, parent_name, function_name):
        key = (parent_name, function_name)
        if key in _script_binding_cache:
            return _script_binding_cache[key]

        options = lldb.SBExpressionOptions()
        options.SetLanguage(lldb.eLanguageTypeC_plus_plus)
        options.SetSuppressPersistentResult(False)
        expr = f'vscript::GetBinding("{parent_name}", "{function_name}")'
        result = self.valobj.target.EvaluateExpression(expr, options)

        if result.IsValid() and not result.GetError().Fail():
            funcaddr = result.GetChildMemberWithName("funcaddr").GetValueAsUnsigned()
            vtable_index = result.GetChildMemberWithName("vtable_index").GetValueAsUnsigned()
            binding = (funcaddr, vtable_index)
            _script_binding_cache[key] = binding
            return binding
        return 0

    def _resolve_via_vscript(self):
        function_name = self.valobj.GetName()
        type_obj = self.valobj.GetType()
        parent_type = type_obj.GetTemplateArgumentType(0)
        parent_name = parent_type.GetUnqualifiedType().GetName() if parent_type else "void*"

        binding = self._get_script_binding(parent_name, function_name)
        if binding == 0:
            self.cached_value = None
            return

        funcaddr, vtable_index = binding

        # Case 1: Direct function address available - just render it
        if funcaddr != 0:
            void_ptr_type = self.valobj.target.GetBasicType(lldb.eBasicTypeVoid).GetPointerType()
            self.cached_value = self.valobj.CreateValueFromAddress("value", funcaddr, void_ptr_type)
            return

        # Case 2: Only vtable_index available - resolve via parent's vtable
        if vtable_index != 0:
            member_offset = self._get_member_offset(parent_type, function_name)
            if member_offset is None:
                self.cached_value = None
                return

            # Calculate parent object address
            parent_addr = self.valobj.GetLoadAddress() - member_offset

            # Dereference parent to get vptr, then index into vtable
            # *(void**)parent_addr gives us the vtable pointer
            # ((void**)(vtable))[vtable_index] gives us the function pointer
            expr = f'((void**)(*(void**){parent_addr}))[{vtable_index}]'
            self.cached_value = self.valobj.CreateValueFromExpression("value", expr)
            return

        # Neither funcaddr nor vtable_index available
        self.cached_value = None

    def num_children(self):
        return 1 if self.cached_value else 0

    def get_child_at_index(self, index):
        return self.cached_value if index == 0 else None

    def has_children(self):
        return self.cached_value is not None

    def get_child_name(self, index):
        return "value"

    def get_child_index(self, name):
        return 0 if name == "value" else None

class ScriptGlobalSynthProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.cached_value = None
        self.update()

    def update(self):
        try:
            self._resolve_via_vscript()
        except Exception as e:
            print(f"EXCEPTION: {e}")
            import traceback
            traceback.print_exc()
            self.cached_value = None

    def _get_script_binding(self, function_name):
        key = function_name
        if key in _script_binding_cache:
            return _script_binding_cache[key]

        options = lldb.SBExpressionOptions()
        options.SetLanguage(lldb.eLanguageTypeC_plus_plus)
        options.SetSuppressPersistentResult(False)
        expr = f'vscript::GetBinding("", "{function_name}")'
        result = self.valobj.target.EvaluateExpression(expr, options)
        if result.IsValid() and not result.GetError().Fail():
            binding = result.GetChildMemberWithName("funcaddr").GetValueAsUnsigned(0)
            _script_binding_cache[key] = binding
            return binding
        return 0

    def _resolve_via_vscript(self):
        function_name = self.valobj.GetName()
        binding = self._get_script_binding(function_name)
        if binding == 0:
            self.cached_value = None
            return
        self.cached_value = self.valobj.CreateValueFromExpression("value", f"(void*){binding}")

    def num_children(self):
        return 1 if self.cached_value else 0

    def get_child_at_index(self, index):
        return self.cached_value if index == 0 else None

    def has_children(self):
        return self.cached_value is not None

    def get_child_name(self, index):
        return "value"

    def get_child_index(self, name):
        return 0 if name == "value" else None

def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand(
        'type synthetic add -x "ScriptMemberFunction<[^>]+>" '
        '--python-class vscript_function_formatter.ScriptFunctionSynthProvider'
    )
    debugger.HandleCommand(
        'type summary add -x "ScriptMemberFunction<[^>]+>" '
        '--summary-string "${svar.value}"'
    )
    debugger.HandleCommand(
        'type synthetic add -x "ScriptGlobalFunction<[^>]+>" '
        '--python-class vscript_function_formatter.ScriptGlobalSynthProvider'
    )
    debugger.HandleCommand(
        'type summary add -x "ScriptGlobalFunction<[^>]+>" '
        '--summary-string "${svar.value}"'
    )
    print("✅ ScriptFunction formatter loaded!")
