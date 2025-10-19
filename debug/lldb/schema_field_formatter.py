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
_schema_offset_cache = {}

class SchemaFieldSynthProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.cached_value = None
        self.update()

    def update(self):
        try:
            self._resolve_via_schema()
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

    def _get_schema_offset(self, parent_name, field_name):
        key = (parent_name, field_name)
        if key in _schema_offset_cache:
            return _schema_offset_cache[key]

        options = lldb.SBExpressionOptions()
        options.SetLanguage(lldb.eLanguageTypeC_plus_plus)
        options.SetSuppressPersistentResult(False)
        expr = f'schema::GetOffset("{parent_name}", "{field_name}")'
        result = self.valobj.target.EvaluateExpression(expr, options)
        if result.IsValid() and not result.GetError().Fail():
            offset = result.GetChildMemberWithName("offset").GetValueAsUnsigned()
            _schema_offset_cache[key] = offset
            return offset
        return 0

    def _resolve_via_schema(self):
        field_name = self.valobj.GetName()
        type_obj = self.valobj.GetType()
        value_type = type_obj.GetTemplateArgumentType(0)
        parent_type = type_obj.GetTemplateArgumentType(1)
        parent_name = parent_type.GetUnqualifiedType().GetName() if parent_type else "void*"

        offset = self._get_schema_offset(parent_name, field_name)
        member_offset = self._get_member_offset(parent_type, field_name)
        if member_offset is None:
            self.cached_value = None
            return

        parent_addr = self.valobj.GetLoadAddress() - member_offset
        final_addr = parent_addr + offset
        self.cached_value = self.valobj.CreateValueFromAddress("value", final_addr, value_type)

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
        'type synthetic add -x "^SchemaField<.+>$" '
        '--python-class schema_field_formatter.SchemaFieldSynthProvider'
    )
    debugger.HandleCommand(
        'type summary add -x "^SchemaField<.+>$" '
        '--summary-string "${svar.value}"'
    )
    print("✅ SchemaField formatter LOADED!")

