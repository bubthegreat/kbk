"""
MUD Terminal - A simple terminal simulator for testing area navigation.
"""
import dearpygui.dearpygui as dpg
from area_editor.app_state import app_state


class MudTerminal:
    """Simple MUD terminal for testing area navigation and look commands."""

    def __init__(self, main_window=None):
        """Initialize the MUD terminal.

        Args:
            main_window: Reference to the main window for updating UI selection
        """
        self.main_window = main_window
        self.window_id = None
        self.output_container_id = None
        self.input_id = None
        self.current_room_vnum = None
        self.command_history = []
        self.history_index = -1
        self.navigating_internally = False  # Flag to track if we're navigating from within terminal

    def open(self, room_vnum: int, clear_history: bool = True):
        """Open the terminal at the specified room.

        Args:
            room_vnum: The room to open the terminal at
            clear_history: If True, clear the terminal output. If False, keep existing history.
        """
        if not app_state.current_area:
            return

        self.current_room_vnum = room_vnum

        # Create or show terminal in right sidebar
        if self.window_id and dpg.does_item_exist(self.window_id):
            dpg.show_item(self.window_id)
            if clear_history:
                self._clear_output()
        else:
            self._create_embedded_terminal()

        # Show initial room (only if clearing history or terminal just created)
        if clear_history or not dpg.does_item_exist(self.output_container_id):
            self._show_room()

        # Focus the input field
        if self.input_id and dpg.does_item_exist(self.input_id):
            dpg.focus_item(self.input_id)

    def _create_embedded_terminal(self):
        """Create the terminal embedded in the right sidebar."""
        # Clear the terminal container
        if dpg.does_item_exist("terminal_container"):
            dpg.delete_item("terminal_container", children_only=True)

        # Create terminal UI inside the container
        with dpg.group(parent="terminal_container") as self.window_id:
            # Output area - using a child window to hold colored text widgets
            with dpg.child_window(
                width=-1,  # Fill available width
                height=-40,
                border=True,
                tag="mud_output_window",
                horizontal_scrollbar=False,  # Disable horizontal scrollbar
                no_scrollbar=False  # Keep vertical scrollbar
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
            # Get the container width and wrap text to fit
            container_width = dpg.get_item_width(self.output_container_id)
            # Subtract some padding for scrollbar and borders
            wrap_width = max(container_width - 20, 100) if container_width > 0 else -1

            dpg.add_text(text, parent=self.output_container_id, color=color, wrap=wrap_width)
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
        if self.input_id and dpg.does_item_exist(self.input_id):
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
        # Quit - clear terminal and show placeholder
        elif cmd in ['quit', 'q', 'exit']:
            self._clear_output()
            self.current_room_vnum = None
            if dpg.does_item_exist("terminal_container"):
                dpg.delete_item("terminal_container", children_only=True)
                dpg.add_text(
                    "Select a room and click 'Open Terminal' to start",
                    parent="terminal_container",
                    color=(120, 120, 120)
                )
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

        # Update UI selection to match the new room
        # Set flag to indicate we're navigating internally (don't clear history)
        self.navigating_internally = True
        self._update_ui_selection(new_room_vnum)
        self.navigating_internally = False

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

        # Blank line after description
        self._add_output("")

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

        # Find mobiles in this room by checking resets
        # We need to track which mobile reset we're on for equipment/inventory
        mobiles_here = []
        mobile_resets = []  # Track reset objects for equipment lookup

        for reset in app_state.current_area.resets:
            if reset.command == 'M' and reset.arg3 == self.current_room_vnum:
                # This is a mobile reset for this room
                mob_vnum = reset.arg1
                if mob_vnum in app_state.current_area.mobiles:
                    mob = app_state.current_area.mobiles[mob_vnum]
                    mobiles_here.append(mob)
                    mobile_resets.append(reset)

        # Find objects on the ground (O resets)
        objects_here = []
        for reset in app_state.current_area.resets:
            if reset.command == 'O' and reset.arg3 == self.current_room_vnum:
                # This is an object reset for this room
                obj_vnum = reset.arg1
                if obj_vnum in app_state.current_area.objects:
                    obj = app_state.current_area.objects[obj_vnum]
                    objects_here.append(obj)

        # Display mobiles (no blank line before them)
        if mobiles_here:
            for i, mob in enumerate(mobiles_here):
                # Show the mobile's long description (how they appear in the room)
                if mob.long_description:
                    self._add_output(mob.long_description.strip(), color=(200, 200, 255))
                else:
                    # Fallback to short description if no long description
                    self._add_output(f"{mob.short_description} is here.", color=(200, 200, 255))

                # Show equipment and inventory for this mobile
                # Find E (equip) and G (give) resets that follow this M reset
                self._show_mobile_equipment(mob, i, mobile_resets)

        # Display objects on the ground
        if objects_here:
            for obj in objects_here:
                # Show the object's long description (how it appears in the room)
                if obj.long_description:
                    self._add_output(obj.long_description.strip(), color=(150, 255, 150))
                else:
                    # Fallback to short description
                    self._add_output(f"{obj.short_description} is here.", color=(150, 255, 150))

        self._add_output("")  # Blank line

    def _show_mobile_equipment(self, mob, mobile_index: int, mobile_resets: list):
        """Show equipment and inventory for a mobile.

        Args:
            mob: The mobile object
            mobile_index: Index of this mobile in the mobile_resets list
            mobile_resets: List of all M resets in this room
        """
        # Wear location names
        wear_locations = {
            0: "as a light",
            1: "on left finger",
            2: "on right finger",
            3: "around neck",
            4: "around neck",
            5: "on body",
            6: "on head",
            7: "on legs",
            8: "on feet",
            9: "on hands",
            10: "on arms",
            11: "as a shield",
            12: "about body",
            13: "around waist",
            14: "on left wrist",
            15: "on right wrist",
            16: "wielded",
            17: "held in hand"
        }

        # Find the position of this mobile's reset in the full reset list
        # E and G resets apply to the most recent M reset
        mobile_reset_index = -1
        m_count = 0
        for i, reset in enumerate(app_state.current_area.resets):
            if reset.command == 'M':
                if m_count == mobile_index and reset.arg3 == self.current_room_vnum:
                    mobile_reset_index = i
                    break
                if reset.arg3 == self.current_room_vnum:
                    m_count += 1

        if mobile_reset_index == -1:
            return

        # Find E (equip) and G (give) resets that follow this M reset
        # They apply until we hit another M, O, or S reset
        equipment = []
        inventory = []

        for i in range(mobile_reset_index + 1, len(app_state.current_area.resets)):
            reset = app_state.current_area.resets[i]

            # Stop at next mobile, object, or end
            if reset.command in ['M', 'O', 'S']:
                break

            if reset.command == 'E':
                # Equipment reset
                obj_vnum = reset.arg1
                wear_loc = reset.arg3
                if obj_vnum in app_state.current_area.objects:
                    obj = app_state.current_area.objects[obj_vnum]
                    loc_name = wear_locations.get(wear_loc, f"in slot {wear_loc}")
                    equipment.append((obj, loc_name))

            elif reset.command == 'G':
                # Give (inventory) reset
                obj_vnum = reset.arg1
                if obj_vnum in app_state.current_area.objects:
                    obj = app_state.current_area.objects[obj_vnum]
                    inventory.append(obj)

        # Only display if mobile has equipment or inventory
        if equipment or inventory:
            # Display equipment
            for obj, loc_name in equipment:
                self._add_output(f"  ...{loc_name}: {obj.short_description}", color=(180, 220, 180))

            # Display inventory (carried items)
            for obj in inventory:
                self._add_output(f"  ...carrying: {obj.short_description}", color=(180, 220, 180))

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
        self._add_output("  Other: help, quit (closes terminal)", color=(200, 200, 200))
        self._add_output("", color=(200, 200, 200))
        self._add_output("This is a simplified MUD simulator for testing area navigation.", color=(180, 180, 180))

    def _update_ui_selection(self, room_vnum: int):
        """Update the UI selection to match the current room.

        Args:
            room_vnum: The room vnum to select in the UI
        """
        if not self.main_window or not app_state.current_area_id:
            return

        # Update app state selection
        app_state.select_item('room', room_vnum)

        # Update tree selection
        if hasattr(self.main_window, 'area_tree'):
            tree = self.main_window.area_tree
            area_id = app_state.current_area_id

            # Deselect previous item
            if tree.current_selection and tree.current_selection in tree.selectable_items:
                prev_id = tree.selectable_items[tree.current_selection]
                if dpg.does_item_exist(prev_id):
                    dpg.set_value(prev_id, False)

            # Select new item
            tree.current_selection = (area_id, 'room', room_vnum)
            if (area_id, 'room', room_vnum) in tree.selectable_items:
                new_id = tree.selectable_items[(area_id, 'room', room_vnum)]
                if dpg.does_item_exist(new_id):
                    dpg.set_value(new_id, True)

        # Update editor panel
        if hasattr(self.main_window, 'editor_panel'):
            self.main_window.editor_panel.show_room_editor(room_vnum)

        # Update properties panel
        if hasattr(self.main_window, 'properties_panel'):
            room = app_state.current_area.rooms.get(room_vnum)
            if room:
                self.main_window.properties_panel.show_room_properties(room, room_vnum)

