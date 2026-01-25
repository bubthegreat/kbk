"""
Custom wrapped text input widget for DearPyGui that auto-wraps at 80 characters.

Note: Due to DearPyGui limitations, wrapping occurs when you unfocus (blur) from
the text field, not in real-time as you type. This is because DearPyGui does not
expose the internal text buffer during editing, and programmatic changes to the
text value while focused are ignored.
"""
import dearpygui.dearpygui as dpg


class WrappedTextInput:
    """A text input widget that automatically wraps text at 80 characters."""

    def __init__(self, default_value: str, callback, user_data, width: int = -1, min_height: int = 60, line_height: int = 20):
        """Initialize the wrapped text input.

        Args:
            default_value: Initial text value
            callback: Callback function when text changes
            user_data: User data to pass to callback
            width: Width of the widget (-1 for full width)
            min_height: Minimum height of the widget
            line_height: Height per line of text (for auto-sizing)
        """
        self.callback = callback
        self.user_data = user_data
        self.width = int(width) if width != -1 else -1
        self.min_height = int(min_height)
        self.line_height = int(line_height)
        self.input_tag = None
        # Ensure default_value is a string
        default_value = str(default_value) if default_value is not None else ""
        self.last_value = self._wrap_text(default_value)
        
    def _wrap_text(self, text: str, width: int = 80) -> str:
        """Wrap text at specified width, breaking at spaces when possible.

        Args:
            text: Text to wrap
            width: Maximum line width (default 80)

        Returns:
            Wrapped text with newlines inserted
        """
        if not text:
            return text

        lines = []
        current_line = ""

        # Split by existing newlines first to preserve paragraphs
        paragraphs = text.split('\n')

        for paragraph in paragraphs:
            if not paragraph:
                # Preserve empty lines
                lines.append("")
                continue

            words = paragraph.split(' ')
            current_line = ""

            for word in words:
                # If adding this word would exceed width
                if current_line and len(current_line) + 1 + len(word) > width:
                    # Save current line and start new one
                    lines.append(current_line)
                    current_line = word
                else:
                    # Add word to current line
                    if current_line:
                        current_line += " " + word
                    else:
                        current_line = word

            # Add the last line of this paragraph
            if current_line:
                lines.append(current_line)

        return '\n'.join(lines)
    
    def _calculate_height(self, text: str) -> int:
        """Calculate the height needed to display all lines of text.

        Args:
            text: The text to calculate height for

        Returns:
            Height in pixels (as integer)
        """
        if not text:
            return int(self.min_height)

        # Count the number of lines
        line_count = text.count('\n') + 1

        # Calculate height: line_count * line_height + some padding
        calculated_height = line_count * self.line_height + 10

        # Return at least min_height, ensure it's an integer
        return int(max(calculated_height, self.min_height))

    def _on_text_change(self, sender, app_data, user_data):
        """Handle text changes when unfocusing (blur)."""
        current_text = dpg.get_value(self.input_tag)

        # Apply wrapping
        wrapped = self._wrap_text(current_text)

        # Update the value if wrapping changed it
        if wrapped != current_text:
            dpg.set_value(self.input_tag, wrapped)

        self.last_value = wrapped

        # Resize to fit content
        new_height = self._calculate_height(wrapped)
        dpg.configure_item(self.input_tag, height=new_height)

        # Call the original callback with our stored user_data
        if self.callback:
            self.callback(sender, app_data, self.user_data)
    
    def create(self, parent=None):
        """Create the widget and return its tag.

        Args:
            parent: Parent widget tag (optional, uses current DearPyGui context if None)

        Returns:
            Tag of the created input widget
        """
        # Calculate initial height based on content
        initial_height = self._calculate_height(self.last_value)

        # Build kwargs for add_input_text
        # Note: We don't pass user_data to DearPyGui because we handle it ourselves in _on_text_change
        kwargs = {
            'default_value': self.last_value,
            'multiline': True,
            'width': self.width,
            'height': initial_height,
            'callback': self._on_text_change,
            'tab_input': True,
            'no_horizontal_scroll': True
        }

        # Only add parent if it's not None (let DearPyGui use current context otherwise)
        if parent is not None:
            kwargs['parent'] = parent

        self.input_tag = dpg.add_input_text(**kwargs)

        return self.input_tag


