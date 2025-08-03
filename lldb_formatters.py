import lldb

def get_context_summary(value, internal_dict):
    """
    lldb summary provider for TypingContext.
    It iterates over the internal map and calls to_string() on each Type pointer.
    """
    try:
        # Get the 'context' member (the std::map)
        context_map = value.GetChildMemberWithName('context')
        if not context_map.IsValid():
            return "map not valid"

        size = context_map.GetNumChildren()
        if size == 0:
            return "size=0"

        summary_parts = []
        for i in range(size):
            entry = context_map.GetChildAtIndex(i)
            key = entry.GetChildMemberWithName('first')
            type_ptr_val = entry.GetChildMemberWithName('second')

            if not key.IsValid() or not type_ptr_val.IsValid():
                continue

            key_string = key.GetSummary()
            if not key_string:
                key_string = key.GetValueAsCString()

            if key_string and len(key_string) > 2 and key_string[0] == '"' and key_string[-1] == '"':
                key_string = key_string[1:-1]

            # Use lldb's API to safely dereference the pointer and call the method
            type_summary = "nullptr"
            if type_ptr_val.GetValueAsUnsigned() != 0:
                # Safely dereference the pointer
                try:
                    dereferenced_type_value = type_ptr_val.Dereference()
                    if dereferenced_type_value.IsValid():
                        # Call the to_string() virtual method
                        to_string_result = dereferenced_type_value.EvaluateExpression("to_string()")
                        if to_string_result.GetError().Success():
                            type_summary = to_string_result.GetSummary()
                            if type_summary and len(type_summary) > 2 and type_summary[0] == '"' and type_summary[-1] == '"':
                                type_summary = type_summary[1:-1]
                        else:
                            type_summary = f"Error: {to_string_result.GetError().GetCString()}"
                    else:
                        type_summary = "invalid pointer"
                except Exception as ex:
                    type_summary = f"Exception during dereference: {ex}"

            summary_parts.append(f"{key_string}: {type_summary}")

        return "{ " + ", ".join(summary_parts) + " }"
    except Exception as e:
        return f"Error in summary provider: {e}"

def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand('type summary add -F lldb_formatters.get_context_summary TypingContext')
    print("TypingContext formatter loaded.")