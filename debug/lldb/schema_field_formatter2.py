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

class SchemaFieldSynthProvider:
    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.cached_value = None
        self.update()

    def update(self):
        try:
            # Just call Get() which does all the work for us!
            schema_field_addr = self.valobj.GetLoadAddress()

            options = lldb.SBExpressionOptions()
            options.SetLanguage(lldb.eLanguageTypeC_plus_plus)
            options.SetSuppressPersistentResult(False)

            # Call Get() on our SchemaField
            #get_expr = f"(({self.valobj.GetTypeName()}*)0x{schema_field_addr:x})->Get()"
            get_expr = f"{self.valobj.GetName()}.Get()"

            result = self.valobj.target.EvaluateExpression(get_expr, options)

            if result.IsValid() and not result.GetError().Fail():
                # Get() returns a reference, so we need to dereference it
                self.cached_value = result
            else:
                print(f"Get() call failed: {result.GetError()}")
                self.cached_value = None

        except Exception as e:
            print(f"Exception: {e}")
            import traceback
            traceback.print_exc()
            self.cached_value = None

    def get_child_at_index(self, index):
        if index == 0 and self.cached_value:
            return self.cached_value
        return None

    def num_children(self):
        return 1 if self.cached_value else 0

    def has_children(self):
        return self.cached_value is not None


def schema_field_recognizer(valobj, internal_dict):
    """
    Recognizer function that matches variables ending with _prop
    and confirms they are SchemaField types
    """
    name = valobj.GetName()
    typename = valobj.GetTypeName()

    # Check if name ends with _prop
    if name and name.endswith('_prop'):
        # Verify it's actually a SchemaField type
        if 'SchemaField<' in typename:
            print(f"✓ Recognized _prop field: {name} ({typename})")
            return True

    return False


def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand(
        'type synthetic add -x "^SchemaField<.+>$" '
        '--python-class schema_field_formatter2.SchemaFieldSynthProvider'
    )

    debugger.HandleCommand(
        'type summary add -x "^SchemaField<.+>$" '
        '--summary-string "${var.value}"'
    )

    print("✅ SchemaField formatter LOADED!")
    print("   - Type match: SchemaField<T>")
    print("   - Name match: *_prop fields")
    print("💡 Tip: Check 'frame variable' output for debug info")
