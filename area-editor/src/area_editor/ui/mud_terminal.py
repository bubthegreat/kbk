"""
MUD Terminal - A simple terminal simulator for testing area navigation.
"""
import dearpygui.dearpygui as dpg
from area_editor.app_state import app_state


class MudTerminal:
    """Simple MUD terminal for testing area navigation and look commands."""

    def __init__(self):
        """Initialize the MUD terminal."""
        self.window_id = None
        self.output_container_id = None
        self.input_id = None
        self.current_room_vnum = None
        self.command_history = []
        self.history_index = -1

    def open(self, room_vnum: int):
        """Open the terminal at the specified room."""
        if not app_state.current_area:
            return

        self.current_room_vnum = room_vnum

        # Create or show window
        if self.window_id and dpg.does_item_exist(self.window_id):
            dpg.show_item(self.window_id)
            self._clear_output()
        else:
            self._create_window()

        # Show initial room
        self._show_room()

        # Focus the input field
        if self.input_id and dpg.does_item_exist(self.input_id):
            dpg.focus_item(self.input_id)

    def _create_window(self):
        """Create the terminal window."""
        with dpg.window(
            label="MUD Terminal",
            width=800,
            height=600,
            pos=(100, 100),
            on_close=self._on_close,
            tag="mud_terminal_window"
        ) as self.window_id:
            # Output area - using a group to hold colored text widgets
            with dpg.child_window(
                height=-40,
                border=True,
                tag="mud_output_window"
            ) as self.output_container_id:
                pass

            # Input area
            with dpg.group(horizontal=True):
                dpg.add_text(">", color=(100, 255, 100))
                self.input_id = dpg.add_input_text(
                    width=-1,
                    callback=self._on_command,
                    on_enter=True,
                    hint="Enter command...",
                    auto_select_all=True  # Auto-select all text when focused
                )

    def _on_close(self):
        """Handle window close."""
        pass

    def _clear_output(self):
        """Clear the output text."""
        if self.output_container_id and dpg.does_item_exist(self.output_container_id):
            dpg.delete_item(self.output_container_id, children_only=True)

    def _add_output(self, text: str, color: tuple = (200, 200, 200)):
        """Add text to the output with optional color.

        Args:
            text: The text to add
            color: RGB tuple for text color (default: light gray)
        """
        if self.output_container_id and dpg.does_item_exist(self.output_container_id):
            dpg.add_text(text, parent=self.output_container_id, color=color, wrap=760)
            # Auto-scroll to bottom
            dpg.set_y_scroll(self.output_container_id, -1.0)

    def _on_command(self, sender, app_data):
        """Handle command input."""
        command = app_data.strip().lower()

        # If empty command, repeat last command
        if not command:
            if self.command_history:
                command = self.command_history[-1]
            else:
                return

        # Add to history (avoid duplicate consecutive commands)
        if not self.command_history or self.command_history[-1] != command:
            self.command_history.append(command)
        self.history_index = len(self.command_history)

        # Echo command (in green like the prompt)
        self._add_output(f"> {command}", color=(100, 255, 100))

        # Process command
        self._process_command(command)

        # Keep the command in the input field (highlighted/selected)
        dpg.set_value(self.input_id, command)

        # Focus the input field and select all text
        dpg.focus_item(self.input_id)
        # Note: DearPyGui doesn't have a direct "select all" API, but setting the value
        # and focusing will allow the user to start typing to replace it, or press
        # Enter to repeat it

    def _process_command(self, command: str):
        """Process a MUD command."""
        parts = command.split()
        if not parts:
            return

        cmd = parts[0]
        args = parts[1:] if len(parts) > 1 else []

        # Movement commands
        if cmd in ['north', 'n', 'south', 's', 'east', 'e', 'west', 'w', 'up', 'u', 'down', 'd']:
            self._move(cmd)
        # Look commands
        elif cmd in ['look', 'l']:
            if args:
                self._look_at(' '.join(args))
            else:
                self._show_room()
        # Help
        elif cmd in ['help', 'h', '?']:
            self._show_help()
        # Quit
        elif cmd in ['quit', 'q', 'exit']:
            dpg.hide_item(self.window_id)
        else:
            self._add_output("Huh?", color=(255, 150, 150))

    def _move(self, direction: str):
        """Move in a direction."""
        # Map direction names to numbers
        dir_map = {
            'north': 0, 'n': 0,
            'east': 1, 'e': 1,
            'south': 2, 's': 2,
            'west': 3, 'w': 3,
            'up': 4, 'u': 4,
            'down': 5, 'd': 5
        }

        dir_num = dir_map.get(direction)
        if dir_num is None:
            return

        room = app_state.current_area.rooms.get(self.current_room_vnum)
        if not room:
            return

        # Check if exit exists
        if dir_num not in room.exits:
            self._add_output("Alas, you cannot go that way.", color=(255, 200, 100))
            return

        # Move to new room
        exit_obj = room.exits[dir_num]
        new_room_vnum = exit_obj.to_room

        if new_room_vnum not in app_state.current_area.rooms:
            self._add_output(f"ERROR: Room #{new_room_vnum} does not exist!", color=(255, 100, 100))
            return

        self.current_room_vnum = new_room_vnum
        self._add_output("")  # Blank line
        self._show_room()

    def _show_room(self):
        """Display the current room."""
        room = app_state.current_area.rooms.get(self.current_room_vnum)
        if not room:
            self._add_output(f"ERROR: Room #{self.current_room_vnum} does not exist!", color=(255, 100, 100))
            return

        # Room name (in cyan color)
        self._add_output(room.name, color=(100, 200, 255))

        # Room description (normal color)
        self._add_output(room.description, color=(200, 200, 200))

        # Exits (in yellow/gold color)
        if room.exits:
            exit_names = []
            dir_names = ['north', 'east', 'south', 'west', 'up', 'down']
            for dir_num in sorted(room.exits.keys()):
                if dir_num < len(dir_names):
                    exit_names.append(dir_names[dir_num])
            self._add_output(f"Exits: {' '.join(exit_names)}", color=(255, 220, 100))
        else:
            self._add_output("Exits: none", color=(255, 220, 100))

        # Objects in room (simplified - just show all objects for now)
        # In a real MUD, this would be filtered by resets
        objects_here = []
        for obj in app_state.current_area.objects.values():
            # For simplicity, we'll just show a few objects as examples
            # In reality, you'd need to check resets to see what's in the room
            pass

        # Mobiles in room (simplified - just show all mobiles for now)
        # In a real MUD, this would be filtered by resets
        mobiles_here = []
        for mob in app_state.current_area.mobiles.values():
            # For simplicity, we'll just show a few mobiles as examples
            # In reality, you'd need to check resets to see what's in the room
            pass

        self._add_output("")  # Blank line

    def _look_at(self, target: str):
        """Look at a specific target."""
        room = app_state.current_area.rooms.get(self.current_room_vnum)
        if not room:
            return

        target = target.lower()

        # Check extra descriptions
        for edesc in room.extra_descriptions:
            keywords = edesc.keywords.lower().split()
            if target in keywords:
                self._add_output(edesc.description, color=(200, 200, 200))
                return

        # Check objects (simplified)
        for obj in app_state.current_area.objects.values():
            if target in obj.short_description.lower():
                self._add_output(obj.long_description or obj.short_description, color=(150, 255, 150))
                return

        # Check mobiles (simplified)
        for mob in app_state.current_area.mobiles.values():
            if target in mob.short_description.lower():
                self._add_output(mob.long_description or mob.short_description, color=(255, 200, 150))
                return

        self._add_output("You don't see that here.", color=(180, 180, 180))

    def _show_help(self):
        """Show available commands."""
        self._add_output("Available Commands:", color=(100, 200, 255))
        self._add_output("  Movement: north (n), south (s), east (e), west (w), up (u), down (d)", color=(200, 200, 200))
        self._add_output("  Look: look, look <target>", color=(200, 200, 200))
        self._add_output("  Other: help, quit", color=(200, 200, 200))
        self._add_output("", color=(200, 200, 200))
        self._add_output("This is a simplified MUD simulator for testing area navigation.", color=(180, 180, 180))

