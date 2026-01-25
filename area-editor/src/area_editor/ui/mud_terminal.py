"""
MUD Terminal - A simple terminal simulator for testing area navigation.
"""
import textwrap
import dearpygui.dearpygui as dpg
from pathlib import Path
from area_editor.app_state import app_state
from area_editor.models.room import Room, Exit
from area_editor.writers.are_writer import AreWriter


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
        self.output_buffer = []  # Buffer to accumulate all output text (for copying)

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
            # Copy button at the top
            dpg.add_button(label="Copy to Clipboard", callback=self._copy_to_clipboard, width=-1)

            # Output area - using a child window to hold colored text widgets
            with dpg.child_window(
                width=-1,
                height=-60,  # Adjusted to account for copy button
                border=True,
                tag="mud_output_window",
                horizontal_scrollbar=False,
                no_scrollbar=False
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
        self.output_buffer = []
        if self.output_container_id and dpg.does_item_exist(self.output_container_id):
            dpg.delete_item(self.output_container_id, children_only=True)

    def _add_output(self, text: str, color: tuple = (200, 200, 200)):
        """Add text to the output with optional color.

        Args:
            text: The text to add
            color: RGB tuple for text color (default: light gray)
        """
        # Add to buffer for clipboard copying
        self.output_buffer.append(text)

        if self.output_container_id and dpg.does_item_exist(self.output_container_id):
            # Get the container width and wrap text to fit
            container_width = dpg.get_item_width(self.output_container_id)
            wrap_width = max(container_width - 20, 100) if container_width > 0 else -1

            dpg.add_text(text, parent=self.output_container_id, color=color, wrap=wrap_width)
            # Auto-scroll to bottom
            dpg.set_y_scroll(self.output_container_id, -1.0)

    def _copy_to_clipboard(self):
        """Copy all terminal output to clipboard."""
        if self.output_buffer:
            full_text = "\n".join(self.output_buffer)
            dpg.set_clipboard_text(full_text)

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
        # Dig commands
        elif cmd == 'dig':
            if args and args[0] in ['north', 'n', 'south', 's', 'east', 'e', 'west', 'w', 'up', 'u', 'down', 'd']:
                # Check if a target vnum was provided
                target_vnum = None
                if len(args) > 1:
                    try:
                        target_vnum = int(args[1])
                    except ValueError:
                        self._add_output(f"Invalid vnum: {args[1]}", color=(255, 100, 100))
                        return
                self._dig(args[0], target_vnum)
            else:
                self._add_output("Usage: dig <direction> [vnum]", color=(255, 200, 100))
                self._add_output("  Directions: north, south, east, west, up, down", color=(200, 200, 200))
                self._add_output("  If vnum is provided, links to existing room with bidirectional exit", color=(200, 200, 200))
                self._add_output("  If vnum is omitted, creates a new room", color=(200, 200, 200))
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

    def _dig(self, direction: str, target_vnum: int = None):
        """Dig a new room or link to an existing room in the specified direction.

        Args:
            direction: Direction to dig (north, south, east, west, up, down)
            target_vnum: Optional vnum of existing room to link to. If None, creates a new room.
        """
        # Map direction names to numbers
        dir_map = {
            'north': 0, 'n': 0,
            'east': 1, 'e': 1,
            'south': 2, 's': 2,
            'west': 3, 'w': 3,
            'up': 4, 'u': 4,
            'down': 5, 'd': 5
        }

        # Reverse direction map for creating bidirectional exits
        reverse_dir = {0: 2, 1: 3, 2: 0, 3: 1, 4: 5, 5: 4}

        dir_num = dir_map.get(direction)
        if dir_num is None:
            return

        room = app_state.current_area.rooms.get(self.current_room_vnum)
        if not room:
            return

        # Check if exit already exists
        if dir_num in room.exits:
            self._add_output("There is already an exit in that direction.", color=(255, 200, 100))
            return

        area = app_state.current_area

        # If target_vnum is provided, link to existing room
        if target_vnum is not None:
            # Check if target room exists
            if target_vnum not in area.rooms:
                self._add_output(f"ERROR: Room #{target_vnum} does not exist!", color=(255, 100, 100))
                return

            target_room = area.rooms[target_vnum]

            # Create exit from current room to target room
            room.exits[dir_num] = Exit(
                direction=dir_num,
                to_room=target_vnum,
                description="",
                keywords="",
                locks=0,
                key_vnum=0
            )

            # Create reverse exit from target room back to current room
            reverse_dir_num = reverse_dir[dir_num]
            target_room.exits[reverse_dir_num] = Exit(
                direction=reverse_dir_num,
                to_room=self.current_room_vnum,
                description="",
                keywords="",
                locks=0,
                key_vnum=0
            )

            # Save the area file if we have a file path
            if app_state.current_file:
                try:
                    writer = AreWriter(area)
                    writer.write(app_state.current_file)
                    self._add_output(f"Two-way link established to room #{target_vnum}.", color=(100, 255, 100))
                    # Mark the area as modified
                    app_state.mark_modified()
                except Exception as e:
                    self._add_output(f"ERROR: Failed to save area file: {e}", color=(255, 100, 100))
                    # Remove the exits since we couldn't save
                    del room.exits[dir_num]
                    del target_room.exits[reverse_dir_num]
                    return
            else:
                self._add_output(f"Two-way link established to room #{target_vnum}.", color=(100, 255, 100))
                self._add_output("WARNING: No area file loaded, changes not saved!", color=(255, 200, 100))

            # Move to the target room
            self.current_room_vnum = target_vnum
            self._add_output("")  # Blank line
            self._show_room()

            # Update UI selection to match the target room
            self.navigating_internally = True
            self._update_ui_selection(target_vnum)
            self.navigating_internally = False

        else:
            # Original behavior: create a new room
            # Find next available vnum in the area
            next_vnum = None
            for vnum in range(area.min_vnum, area.max_vnum + 1):
                if vnum not in area.rooms:
                    next_vnum = vnum
                    break

            if next_vnum is None:
                self._add_output("ERROR: No available vnums in this area!", color=(255, 100, 100))
                self._add_output(f"Area range: {area.min_vnum}-{area.max_vnum}", color=(200, 200, 200))
                return

            # Create the new room
            new_room = Room(
                vnum=next_vnum,
                name="A New Room",
                description="This is a newly created room. Edit this description to describe what the room looks like.\n",
                room_flags=0,
                sector_type=0,  # Inside
                area_number=0
            )

            # Add the new room to the area
            area.rooms[next_vnum] = new_room

            # Create exit from current room to new room
            room.exits[dir_num] = Exit(
                direction=dir_num,
                to_room=next_vnum,
                description="",
                keywords="",
                locks=0,
                key_vnum=0
            )

            # Create reverse exit from new room back to current room
            reverse_dir_num = reverse_dir[dir_num]
            new_room.exits[reverse_dir_num] = Exit(
                direction=reverse_dir_num,
                to_room=self.current_room_vnum,
                description="",
                keywords="",
                locks=0,
                key_vnum=0
            )

            # Save the area file if we have a file path
            if app_state.current_file:
                try:
                    writer = AreWriter(area)
                    writer.write(app_state.current_file)
                    self._add_output(f"Created room #{next_vnum} to the {direction}.", color=(100, 255, 100))
                    # Mark the area as modified
                    app_state.mark_modified()
                except Exception as e:
                    self._add_output(f"ERROR: Failed to save area file: {e}", color=(255, 100, 100))
                    # Remove the room and exits since we couldn't save
                    del area.rooms[next_vnum]
                    del room.exits[dir_num]
                    return
            else:
                self._add_output(f"Created room #{next_vnum} to the {direction}.", color=(100, 255, 100))
                self._add_output("WARNING: No area file loaded, changes not saved!", color=(255, 200, 100))

            # Move to the new room
            self.current_room_vnum = next_vnum
            self._add_output("")  # Blank line
            self._show_room()

            # Refresh the area tree to show the new room
            if self.main_window and hasattr(self.main_window, 'area_tree') and app_state.current_area_id:
                self.main_window.area_tree.populate_from_area(area, app_state.current_area_id)

            # Update UI selection to match the new room
            self.navigating_internally = True
            self._update_ui_selection(next_vnum)
            self.navigating_internally = False

    def _show_room(self):
        """Display the current room."""
        room = app_state.current_area.rooms.get(self.current_room_vnum)
        if not room:
            self._add_output(f"ERROR: Room #{self.current_room_vnum} does not exist!", color=(255, 100, 100))
            return

        # Room name (in cyan color)
        self._add_output(room.name, color=(100, 200, 255))

        # Room description (normal color) - wrapped at 80 characters
        # Preserve paragraph breaks (blank lines) by processing each paragraph separately
        description = room.description.strip()
        if description:
            # Split by double newlines to preserve paragraph breaks
            paragraphs = description.split('\n\n')
            wrapped_paragraphs = []
            for para in paragraphs:
                # Wrap each paragraph individually
                if para.strip():
                    wrapped_paragraphs.append(textwrap.fill(para.strip(), width=80))
            # Join paragraphs with blank lines
            wrapped_description = '\n\n'.join(wrapped_paragraphs)
            self._add_output(wrapped_description, color=(200, 200, 200))

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
                    wrapped = textwrap.fill(mob.long_description.strip(), width=80)
                    self._add_output(wrapped, color=(200, 200, 255))
                else:
                    # Fallback to short description if no long description
                    self._add_output(f"{mob.short_description} is here.", color=(200, 200, 255))

                # NOTE: Equipment/inventory is NOT shown here - only when you 'look' at the mobile

        # Display objects on the ground
        if objects_here:
            for obj in objects_here:
                # Show the object's long description (how it appears in the room)
                if obj.long_description:
                    wrapped = textwrap.fill(obj.long_description.strip(), width=80)
                    self._add_output(wrapped, color=(150, 255, 150))
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
        # Wear location names (matching MUD format)
        wear_locations = {
            0: "<used as light>    ",
            1: "<worn on finger>   ",
            2: "<worn on finger>   ",
            3: "<worn around neck> ",
            4: "<worn around neck> ",
            5: "<worn on body>     ",
            6: "<worn on head>     ",
            7: "<worn on legs>     ",
            8: "<worn on feet>     ",
            9: "<worn on hands>    ",
            10: "<worn on arms>     ",
            11: "<worn as shield>   ",
            12: "<worn about body>  ",
            13: "<worn about waist> ",
            14: "<worn around wrist>",
            15: "<worn around wrist>",
            16: "<mainhand wielded> ",
            17: "<offhand wielded>  "
        }

        # Object extra flags for visual effects
        ITEM_GLOW = 1
        ITEM_HUM = 2
        ITEM_DARK = 3
        ITEM_EVIL = 5
        ITEM_MAGIC = 7
        ITEM_BLESS = 9

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

        # Helper function to check if a bit is set in extra_flags
        def has_flag(obj, flag_bit):
            """Check if an object has a specific extra flag bit set."""
            # The extra_flags is stored as an integer with bits set
            # Bit 1 = ITEM_GLOW, Bit 2 = ITEM_HUM, etc.
            return (obj.extra_flags & (1 << (flag_bit - 1))) != 0

        # Helper function to get flag prefix for an object
        def get_flag_prefix(obj):
            """Get the flag prefix string like '(Glowing) (Humming)' for an object."""
            flags = []
            if has_flag(obj, ITEM_GLOW):
                flags.append("(Glowing)")
            if has_flag(obj, ITEM_HUM):
                flags.append("(Humming)")
            if has_flag(obj, ITEM_DARK):
                flags.append("(Dark)")
            if has_flag(obj, ITEM_EVIL):
                flags.append("(Red Aura)")
            if has_flag(obj, ITEM_MAGIC):
                flags.append("(Magical)")
            if has_flag(obj, ITEM_BLESS):
                flags.append("(Blue Aura)")
            return " ".join(flags) + " " if flags else ""

        # Display equipment - only show slots that have items
        # Only show equipment if mobile has any
        if equipment or inventory:
            # Create a dict of worn equipment by slot
            worn_items = {}
            for obj, loc_name in equipment:
                worn_items[loc_name] = obj

            # Display only equipped items (skip empty slots)
            for slot_num in sorted(wear_locations.keys()):
                loc_label = wear_locations[slot_num]
                if loc_label in worn_items:
                    obj = worn_items[loc_label]
                    flag_prefix = get_flag_prefix(obj)
                    self._add_output(f"{loc_label} {flag_prefix}{obj.short_description}", color=(200, 200, 200))

            # Display inventory (carried items) - not shown in equipment list
            # These are shown separately if needed

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
                wrapped = textwrap.fill(edesc.description.strip(), width=80)
                self._add_output(wrapped, color=(200, 200, 200))
                return

        # Check objects in this room only
        # Build list of objects here (same as in _show_room)
        objects_here = []
        for reset in app_state.current_area.resets:
            if reset.command == 'O' and reset.arg3 == self.current_room_vnum:
                obj_vnum = reset.arg1
                if obj_vnum in app_state.current_area.objects:
                    objects_here.append(app_state.current_area.objects[obj_vnum])

        # Check each object in the room
        for obj in objects_here:
            # Check if target matches object's keywords or short description
            if (target in obj.short_description.lower() or
                target in obj.keywords.lower()):
                desc = obj.long_description or obj.short_description
                wrapped = textwrap.fill(desc.strip(), width=80)
                self._add_output(wrapped, color=(150, 255, 150))
                return

        # Check mobiles in this room
        # Build list of mobiles here (same as in _show_room)
        mobile_resets = []
        for reset in app_state.current_area.resets:
            if reset.command == 'M' and reset.arg3 == self.current_room_vnum:
                mobile_resets.append(reset)

        # Check each mobile in the room
        for i, reset in enumerate(mobile_resets):
            mob_vnum = reset.arg1
            if mob_vnum in app_state.current_area.mobiles:
                mob = app_state.current_area.mobiles[mob_vnum]
                # Check if target matches this mobile's keywords or description
                if (target in mob.short_description.lower() or
                    target in mob.keywords.lower()):
                    # Show mobile description (DESCR field, not LONG)
                    desc = mob.description or mob.long_description or mob.short_description
                    # Preserve paragraph breaks in mobile descriptions
                    if desc:
                        paragraphs = desc.split('\n\n')
                        wrapped_paragraphs = []
                        for para in paragraphs:
                            if para.strip():
                                wrapped_paragraphs.append(textwrap.fill(para.strip(), width=80))
                        wrapped_description = '\n\n'.join(wrapped_paragraphs)
                        self._add_output(wrapped_description, color=(255, 200, 150))
                    # Show equipment and inventory for this specific mobile
                    self._show_mobile_equipment(mob, i, mobile_resets)
                    return

        self._add_output("You don't see that here.", color=(180, 180, 180))

    def _show_help(self):
        """Show available commands."""
        self._add_output("Available Commands:", color=(100, 200, 255))
        self._add_output("  Movement: north (n), south (s), east (e), west (w), up (u), down (d)", color=(200, 200, 200))
        self._add_output("  Building:", color=(200, 200, 200))
        self._add_output("    dig <direction>       - Create a new room in that direction", color=(200, 200, 200))
        self._add_output("    dig <direction> <vnum> - Link to existing room with bidirectional exit", color=(200, 200, 200))
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

