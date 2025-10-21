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

class CUtlVector:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj

        self.mem = self.valobj.GetChildMemberWithName('m_Memory')
        self.size_obj = self.valobj.GetChildMemberWithName('m_Size')
        self.alloc_obj = self.mem.GetChildMemberWithName('m_nAllocationCount')
        self.data_ptr = self.mem.GetChildMemberWithName('m_pMemory')

        self.data_type = valobj.GetType().GetTemplateArgumentType(0)
        self.type_size = self.data_type.GetByteSize()

        self.size = self.size_obj.GetValueAsUnsigned(0)
        self.capacity = self.alloc_obj.GetValueAsUnsigned(0)
        self.data_addr = self.data_ptr.GetValueAsUnsigned(0)

    def num_children(self):
        return 2 + (self.size if self.size is not None else 0)

    def get_child_at_index(self, index):
        if index < 0 or index >= self.num_children():
            return None
        try:
            if index == 0:
                return self.valobj.CreateValueFromExpression('size', f'(size_t){self.size}')
            if index == 1:
                return self.valobj.CreateValueFromExpression('capacity', f'(size_t){self.capacity}')
            elem_index = index - 2
            if self.data_addr == 0 or elem_index >= self.size:
                return None
            offset = elem_index * self.type_size
            addr = self.data_addr + offset
            return self.valobj.CreateValueFromAddress(f'[{elem_index}]', addr, self.data_type)
        except:
            return None

class CEntityHandleProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj.GetNonSyntheticValue()
        self.cached_value = None

        got = self.valobj.EvaluateExpression("Get()")
        if got.IsValid() and not got.GetError().Fail():
            self.cached_value = got

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

class CHandleProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj.GetNonSyntheticValue()
        self.cached_value = None

        obj_type = self.valobj.GetType().GetTemplateArgumentType(0)
        if not (obj_type and obj_type.IsValid()):
            return

        options = lldb.SBExpressionOptions()
        options.SetLanguage(lldb.eLanguageTypeC_plus_plus)
        options.SetSuppressPersistentResult(False)

        got = self.valobj.EvaluateExpression("Get()", options)
        if not (got and got.IsValid()) or got.GetError().Fail():
            return

        ptr_type = obj_type.GetPointerType()
        try_cast = got.Cast(ptr_type)
        if try_cast and try_cast.IsValid() and not try_cast.GetError().Fail():
            obj = try_cast.Dereference()
            if obj and obj.IsValid():
                self.cached_value = obj
                return

        addr = got.GetValueAsUnsigned(0)
        ptr_sv = valobj.CreateValueFromAddress("value", addr, ptr_type)
        if not (ptr_sv and ptr_sv.IsValid()):
            return

        obj_sv = ptr_sv.Dereference()
        if obj_sv and obj_sv.IsValid():
            self.cached_value = obj_sv

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
    # CUtlVector<...>
    debugger.HandleCommand('type synthetic add -x "^CUtlVector<[^>]+>" --python-class game_formatter.CUtlVector')
    debugger.HandleCommand('type summary add -x "^CUtlVector<[^>]+>" --summary-string "${var%L}"')

    # CEntityHandle
    debugger.HandleCommand('type synthetic add -x "CEntityHandle$" --python-class game_formatter.CEntityHandleProvider')

    # CHandle<...>
    debugger.HandleCommand('type synthetic add -x "CHandle<[^>]+>" --python-class game_formatter.CHandleProvider')

    # CUtlString
    debugger.HandleCommand('type summary add -x "^CUtlString$" --summary-string "${var.m_pString}"')
    debugger.HandleCommand('type summary add -x "^CUtlSymbolLarge$" --summary-string "${var.u.m_pAsString}"')
    debugger.HandleCommand('type summary add -x "^CUtlSymbol$" --summary-string "${var.u.m_pAsString}"')

    debugger.HandleCommand('type summary add -x "^Vector$"   --summary-string "(${var.x}, ${var.y}, ${var.z})"')
    debugger.HandleCommand('type summary add -x "^QAngle$"   --summary-string "(${var.x}, ${var.y}, ${var.z})"')
    debugger.HandleCommand('type summary add -x "^Vector2D$" --summary-string "(${var.x}, ${var.y})"')
    debugger.HandleCommand('type summary add -x "^Vector4$"  --summary-string "(${var.x}, ${var.y}, ${var.z}, ${var.w})"')

    debugger.HandleCommand('type summary add -x "^Color$"    --summary-string "${var._color[0]:02x}, ${var._color[1]:02x}, ${var._color[2]:02x}, ${var._color[3]:02x}"')
    debugger.HandleCommand('type summary add -x "^GameTime_t$" --summary-string "${var.m_Value}"')
    debugger.HandleCommand('type summary add -x "^GameTick_t$" --summary-string "${var.m_Value}"')

    print("✅ Game formatter loaded!")
