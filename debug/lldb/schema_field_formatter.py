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
import re

_type_field_offsets = {}
_schema_offset_cache = {}
_member_offset_cache = {}

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

    def _get_schema_offset(self, parent_name, field_name):
        key = (parent_name, field_name)
        if key in _schema_offset_cache:
            return _schema_offset_cache[key]

        expr = f'schema::GetOffset("{parent_name}", "{field_name}")'

        options = lldb.SBExpressionOptions()
        options.SetLanguage(lldb.eLanguageTypeC_plus_plus)
        options.SetSuppressPersistentResult(False)

        result = self.valobj.target.EvaluateExpression(expr, options)
        if not result.IsValid() or result.GetError().Fail():
            return 0

        offset = result.GetChildMemberWithName("offset").GetValueAsUnsigned()
        _schema_offset_cache[key] = offset
        return offset

    def _get_member_offset(self, parent_name, field_name):
        key = (parent_name, field_name)
        if key in _member_offset_cache:
            return _member_offset_cache[key]

        expr = f'{parent_name}::{field_name}_offset()'

        options = lldb.SBExpressionOptions()
        options.SetLanguage(lldb.eLanguageTypeC_plus_plus)
        options.SetSuppressPersistentResult(False)

        result = self.valobj.target.EvaluateExpression(expr, options)
        if not result.IsValid() or result.GetError().Fail():
            return None

        offset = result.GetValueAsUnsigned()
        _member_offset_cache[key] = offset
        return offset

    def _parse_template_args(self):
        """Parse template arguments from type name string (MSVC workaround)"""
        type_name = self.valobj.GetType().GetName()

        # Extract template arguments from SchemaField<...>
        match = re.match(r'Schema\S*Field<(.+)>', type_name)
        if not match:
            return None, None

        args_str = match.group(1)

        # Split by commas, but respect nested templates and angle brackets
        args = []
        depth = 0
        current_arg = []

        for char in args_str:
            if char == '<':
                depth += 1
                current_arg.append(char)
            elif char == '>':
                depth -= 1
                current_arg.append(char)
            elif char == ',' and depth == 0:
                args.append(''.join(current_arg).strip())
                current_arg = []
            else:
                current_arg.append(char)

        # Add the last argument
        if current_arg:
            args.append(''.join(current_arg).strip())

        if len(args) < 2:
            return None, None

        # args[0] is value_type, args[1] is parent_type
        value_type_name = args[0]
        parent_type_name = args[1]

        return value_type_name, parent_type_name

    def _resolve_via_schema(self):
        field_name = self.valobj.GetName()
        type_obj = self.valobj.GetType()

        # Try the standard API first (works on GCC/Clang)
        value_type = type_obj.GetTemplateArgumentType(0)
        parent_type = type_obj.GetTemplateArgumentType(1)

        # Fallback for MSVC: parse from type name
        if not value_type or not parent_type:
            value_type_name, parent_type_name = self._parse_template_args()

            if value_type_name and parent_type_name:
                target = self.valobj.GetTarget()
                value_type = target.FindFirstType(value_type_name)
                parent_type = target.FindFirstType(parent_type_name)

        if not value_type or not parent_type:
            self.cached_value = None
            return

        parent_name = parent_type.GetUnqualifiedType().GetName() if parent_type else "void*"

        offset = self._get_schema_offset(parent_name, field_name)
        member_offset = self._get_member_offset(parent_name, field_name)

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
        'type synthetic add -x "SchemaField<[^>]+>" '
        '--python-class schema_field_formatter.SchemaFieldSynthProvider'
    )
    debugger.HandleCommand(
        'type summary add -x "SchemaField<[^>]+>" '
        '--summary-string "${svar.value}"'
    )
    print("✅ SchemaField formatter loaded!")
