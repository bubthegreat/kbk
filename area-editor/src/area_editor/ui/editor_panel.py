"""
Editor panel for editing areas, rooms, objects, and mobiles.
"""
import dearpygui.dearpygui as dpg
from area_editor.app_state import app_state
from area_editor.ui.mud_terminal import MudTerminal


class EditorPanel:
    """Main editor panel that displays different editors based on selection."""

    def __init__(self, main_window):
        """Initialize the editor panel."""
        self.main_window = main_window
        self.current_editor = None
        self.welcome_group_id = None
        self.editor_group_id = None
        self.current_vnum = None  # Track current item being edited
        self.mud_terminal = MudTerminal(main_window)  # MUD terminal instance with main_window reference
    
    def create(self):
        """Create the editor panel."""
        dpg.add_text("Editor", color=(200, 200, 200))
        dpg.add_separator()

        # Welcome message when nothing is selected
        with dpg.group(tag="welcome_group") as self.welcome_group_id:
            dpg.add_spacer(height=50)
            dpg.add_text(
                "Welcome to KBK Area Editor",
                color=(150, 150, 150)
            )
            dpg.add_spacer(height=20)
            dpg.add_text(
                "Select an item from the Area Explorer to begin editing,",
                color=(120, 120, 120)
            )
            dpg.add_text(
                "or create a new area from the File menu.",
                color=(120, 120, 120)
            )

        # Editor group (hidden by default)
        with dpg.group(tag="editor_group", show=False) as self.editor_group_id:
            pass
    
    def _clear_editor(self):
        """Clear the editor content."""
        if self.editor_group_id and dpg.does_item_exist(self.editor_group_id):
            dpg.delete_item(self.editor_group_id, children_only=True)

    def _show_editor(self):
        """Show the editor and hide the welcome screen."""
        if self.welcome_group_id:
            dpg.configure_item(self.welcome_group_id, show=False)
        if self.editor_group_id:
            dpg.configure_item(self.editor_group_id, show=True)

    def _on_room_field_changed(self, sender, app_data, user_data):
        """Handle room field changes."""
        field_name, room_vnum = user_data
        room = app_state.current_area.rooms.get(room_vnum)
        if not room:
            return

        # Update the room field
        if field_name == 'name':
            room.name = app_data
        elif field_name == 'description':
            room.description = app_data

        # Mark as modified
        app_state.mark_modified()

        # Update status bar
        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_file_info(app_state.get_file_info())

    def _on_object_field_changed(self, sender, app_data, user_data):
        """Handle object field changes."""
        field_name, obj_vnum = user_data
        obj = app_state.current_area.objects.get(obj_vnum)
        if not obj:
            return

        # Update the object field
        if field_name == 'short_description':
            obj.short_description = app_data
        elif field_name == 'long_description':
            obj.long_description = app_data

        # Mark as modified
        app_state.mark_modified()

        # Update status bar
        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_file_info(app_state.get_file_info())

    def _on_mobile_field_changed(self, sender, app_data, user_data):
        """Handle mobile field changes."""
        field_name, mob_vnum = user_data
        mob = app_state.current_area.mobiles.get(mob_vnum)
        if not mob:
            return

        # Update the mobile field
        if field_name == 'short_description':
            mob.short_description = app_data
        elif field_name == 'long_description':
            mob.long_description = app_data

        # Mark as modified
        app_state.mark_modified()

        # Update status bar
        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_file_info(app_state.get_file_info())

    def _on_area_field_changed(self, sender, app_data, user_data):
        """Handle area field changes."""
        field_name = user_data
        area = app_state.current_area
        if not area:
            return

        # Update the area field
        if field_name == 'name':
            area.name = app_data

        # Mark as modified
        app_state.mark_modified()

        # Update status bar
        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_file_info(app_state.get_file_info())

    def _on_room_link_clicked(self, sender, app_data, user_data):
        """Handle clicking on a room link in exits."""
        room_vnum = user_data

        # Check if the room exists
        if app_state.current_area and room_vnum in app_state.current_area.rooms:
            # Update selection in app_state
            app_state.select_item('room', room_vnum)

            # Update the tree selection (if we can access it)
            if hasattr(self.main_window, 'area_tree'):
                tree = self.main_window.area_tree
                # Deselect previous item
                if tree.current_selection and tree.current_selection in tree.selectable_items:
                    prev_id = tree.selectable_items[tree.current_selection]
                    if dpg.does_item_exist(prev_id):
                        dpg.set_value(prev_id, False)

                # Select new item
                tree.current_selection = ('room', room_vnum)
                if ('room', room_vnum) in tree.selectable_items:
                    new_id = tree.selectable_items[('room', room_vnum)]
                    if dpg.does_item_exist(new_id):
                        dpg.set_value(new_id, True)

            # Show the room editor and update properties panel
            room = app_state.current_area.rooms.get(room_vnum)
            self.show_room_editor(room_vnum)
            if hasattr(self.main_window, 'properties_panel') and room:
                self.main_window.properties_panel.show_room_properties(room, room_vnum)
        else:
            # Room doesn't exist - show error in status bar
            if hasattr(self.main_window, 'status_bar'):
                self.main_window.status_bar.set_status(
                    f"Room #{room_vnum} not found in current area",
                    color=(255, 100, 100)
                )

    def show_room_editor(self, room_vnum):
        """Show the room editor for the specified room."""
        room = app_state.current_area.rooms.get(room_vnum)
        if not room:
            return

        self._clear_editor()
        self._show_editor()
        self.current_vnum = room_vnum

        # Automatically open the terminal for this room
        # If navigating from within terminal, don't clear history
        clear_history = not self.mud_terminal.navigating_internally
        self.mud_terminal.open(room_vnum, clear_history=clear_history)

        with dpg.group(parent=self.editor_group_id):
            # Header with room number
            dpg.add_text(f"Room #{room_vnum}", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=10)

            # Show validation errors if any
            validation_result = app_state.get_validation_result()
            if validation_result:
                errors = validation_result.get_errors_for_item('room', room_vnum)
                if errors:
                    # Show error panel
                    with dpg.child_window(height=100, border=True):
                        dpg.add_text("⚠ Validation Errors:", color=(255, 100, 100))
                        dpg.add_separator()
                        for error in errors:
                            severity_color = (255, 100, 100) if error.severity == "error" else (255, 200, 100)
                            dpg.add_text(f"• [{error.severity.upper()}] {error.message}",
                                       color=severity_color, wrap=-1)
                            if error.details:
                                dpg.add_text(f"  {error.details}", color=(150, 150, 150), wrap=-1)
                    dpg.add_spacer(height=10)

            dpg.add_text("Name:", color=(150, 150, 150))
            dpg.add_input_text(
                default_value=room.name,
                width=-1,
                callback=self._on_room_field_changed,
                user_data=('name', room_vnum)
            )
            dpg.add_spacer(height=10)

            dpg.add_text("Description:", color=(150, 150, 150))
            dpg.add_input_text(
                default_value=room.description,
                multiline=True,
                height=100,
                width=-1,
                callback=self._on_room_field_changed,
                user_data=('description', room_vnum)
            )
            dpg.add_spacer(height=10)

            dpg.add_text(f"Sector Type: {room.sector_type}", color=(150, 150, 150))
            dpg.add_text(f"Room Flags: {room.room_flags}", color=(150, 150, 150))
            dpg.add_spacer(height=10)

            if room.exits:
                dpg.add_text(f"Exits ({len(room.exits)}):", color=(150, 150, 150))
                # room.exits is a dict mapping direction -> Exit object
                direction_names = ['north', 'east', 'south', 'west', 'up', 'down']
                for direction, exit in room.exits.items():
                    dir_name = direction_names[direction] if direction < len(direction_names) else f"dir{direction}"

                    # Create a horizontal group for the exit with clickable room link
                    with dpg.group(horizontal=True):
                        dpg.add_text(f"  {dir_name} -> ", color=(120, 120, 120))
                        dpg.add_button(
                            label=f"Room #{exit.to_room}",
                            callback=self._on_room_link_clicked,
                            user_data=exit.to_room,
                            width=100,
                            height=20
                        )

            if room.extra_descriptions:
                dpg.add_spacer(height=10)
                dpg.add_text(f"Extra Descriptions ({len(room.extra_descriptions)}):", color=(150, 150, 150))
                dpg.add_spacer(height=5)
                for i, edesc in enumerate(room.extra_descriptions, 1):
                    dpg.add_text(f"EDESC #{i}:", color=(180, 180, 120))
                    dpg.add_text(f"Keywords: {edesc.keywords}", color=(120, 120, 120))
                    dpg.add_spacer(height=3)
                    dpg.add_input_text(
                        default_value=edesc.description,
                        multiline=True,
                        height=80,
                        width=-1,
                        readonly=True  # Make read-only for now
                    )

            # Add properties section inline
            dpg.add_spacer(height=20)
            dpg.add_separator()
            dpg.add_spacer(height=10)
            dpg.add_text("Properties", color=(200, 200, 200))
            dpg.add_separator()
            dpg.add_spacer(height=5)

            # Quick stats
            dpg.add_text("Quick Info", color=(150, 150, 150))
            dpg.add_spacer(height=5)
            dpg.add_text(f"Exits: {len(room.exits)}", color=(120, 120, 120))
            dpg.add_text(f"Extra Descs: {len(room.extra_descriptions)}", color=(120, 120, 120))
            dpg.add_text(f"Sector: {room.sector_type}", color=(120, 120, 120))
            dpg.add_text(f"Flags: {room.room_flags}", color=(120, 120, 120))

            if room.exits:
                dpg.add_spacer(height=10)
                dpg.add_text("Exit Directions", color=(150, 150, 150))
                dpg.add_spacer(height=5)
                direction_names = ['North', 'East', 'South', 'West', 'Up', 'Down']
                for direction in sorted(room.exits.keys()):
                    dir_name = direction_names[direction] if direction < len(direction_names) else f"Dir{direction}"
                    dpg.add_text(f"• {dir_name}", color=(120, 120, 120))
                    dpg.add_spacer(height=10)

    def show_object_editor(self, obj_vnum):
        """Show the object editor for the specified object."""
        obj = app_state.current_area.objects.get(obj_vnum)
        if not obj:
            return

        self._clear_editor()
        self._show_editor()
        self.current_vnum = obj_vnum

        with dpg.group(parent=self.editor_group_id):
            dpg.add_text(f"Object #{obj_vnum}", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=10)

            dpg.add_text("Short Description:", color=(150, 150, 150))
            dpg.add_input_text(
                default_value=obj.short_description,
                width=-1,
                callback=self._on_object_field_changed,
                user_data=('short_description', obj_vnum)
            )
            dpg.add_spacer(height=10)

            dpg.add_text("Long Description:", color=(150, 150, 150))
            dpg.add_input_text(
                default_value=obj.long_description,
                width=-1,
                callback=self._on_object_field_changed,
                user_data=('long_description', obj_vnum)
            )
            dpg.add_spacer(height=10)

            dpg.add_text(f"Item Type: {obj.item_type}", color=(150, 150, 150))
            dpg.add_text(f"Material: {obj.material}", color=(150, 150, 150))
            dpg.add_text(f"Level: {obj.level}", color=(150, 150, 150))
            dpg.add_text(f"Weight: {obj.weight}", color=(150, 150, 150))
            dpg.add_text(f"Cost: {obj.cost}", color=(150, 150, 150))

            if obj.affects:
                dpg.add_spacer(height=10)
                dpg.add_text(f"Affects ({len(obj.affects)}):", color=(150, 150, 150))
                for affect in obj.affects:
                    dpg.add_text(f"  {affect.apply_type}: {affect.modifier}", color=(120, 120, 120))

            if obj.extra_descriptions:
                dpg.add_spacer(height=10)
                dpg.add_text(f"Extra Descriptions ({len(obj.extra_descriptions)}):", color=(150, 150, 150))
                dpg.add_spacer(height=5)
                for i, edesc in enumerate(obj.extra_descriptions, 1):
                    dpg.add_text(f"EDESC #{i}:", color=(180, 180, 120))
                    dpg.add_text(f"Keywords: {edesc.keywords}", color=(120, 120, 120))
                    dpg.add_spacer(height=3)
                    dpg.add_input_text(
                        default_value=edesc.description,
                        multiline=True,
                        height=80,
                        width=-1,
                        readonly=True  # Make read-only for now
                    )
                    dpg.add_spacer(height=10)

            # Add properties section inline
            dpg.add_spacer(height=20)
            dpg.add_separator()
            dpg.add_spacer(height=10)
            dpg.add_text("Properties", color=(200, 200, 200))
            dpg.add_separator()
            dpg.add_spacer(height=5)

            # Quick stats
            dpg.add_text("Quick Info", color=(150, 150, 150))
            dpg.add_spacer(height=5)
            dpg.add_text(f"Type: {obj.item_type}", color=(120, 120, 120))
            dpg.add_text(f"Level: {obj.level}", color=(120, 120, 120))
            dpg.add_text(f"Weight: {obj.weight}", color=(120, 120, 120))
            dpg.add_text(f"Cost: {obj.cost}", color=(120, 120, 120))
            dpg.add_text(f"Material: {obj.material}", color=(120, 120, 120))
            dpg.add_text(f"Extra Descs: {len(obj.extra_descriptions)}", color=(120, 120, 120))

            if obj.affects:
                dpg.add_spacer(height=10)
                dpg.add_text(f"Affects ({len(obj.affects)})", color=(150, 150, 150))
                dpg.add_spacer(height=5)
                for affect in obj.affects[:5]:  # Show first 5
                    dpg.add_text(f"• {affect.apply_type}: {affect.modifier:+d}", color=(120, 120, 120))
                if len(obj.affects) > 5:
                    dpg.add_text(f"  ... and {len(obj.affects) - 5} more", color=(100, 100, 100))

    def show_mobile_editor(self, mob_vnum):
        """Show the mobile editor for the specified mobile."""
        mob = app_state.current_area.mobiles.get(mob_vnum)
        if not mob:
            return

        self._clear_editor()
        self._show_editor()
        self.current_vnum = mob_vnum

        with dpg.group(parent=self.editor_group_id):
            dpg.add_text(f"Mobile #{mob_vnum}", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=10)

            dpg.add_text("Short Description:", color=(150, 150, 150))
            dpg.add_input_text(
                default_value=mob.short_description,
                width=-1,
                callback=self._on_mobile_field_changed,
                user_data=('short_description', mob_vnum)
            )
            dpg.add_spacer(height=10)

            dpg.add_text("Long Description:", color=(150, 150, 150))
            dpg.add_input_text(
                default_value=mob.long_description,
                multiline=True,
                height=60,
                width=-1,
                callback=self._on_mobile_field_changed,
                user_data=('long_description', mob_vnum)
            )
            dpg.add_spacer(height=10)

            dpg.add_text(f"Race: {mob.race}", color=(150, 150, 150))
            dpg.add_text(f"Level: {mob.level}", color=(150, 150, 150))
            dpg.add_text(f"Alignment: {mob.alignment}", color=(150, 150, 150))
            dpg.add_text(f"Hit Dice: {mob.hit_dice}", color=(150, 150, 150))
            dpg.add_text(f"Damage Dice: {mob.damage_dice}", color=(150, 150, 150))
            dpg.add_text(f"Hitroll: {mob.hitroll}", color=(150, 150, 150))

            # Add properties section inline
            dpg.add_spacer(height=20)
            dpg.add_separator()
            dpg.add_spacer(height=10)
            dpg.add_text("Properties", color=(200, 200, 200))
            dpg.add_separator()
            dpg.add_spacer(height=5)

            # Quick stats
            dpg.add_text("Quick Info", color=(150, 150, 150))
            dpg.add_spacer(height=5)
            dpg.add_text(f"Race: {mob.race}", color=(120, 120, 120))
            dpg.add_text(f"Level: {mob.level}", color=(120, 120, 120))
            dpg.add_text(f"Alignment: {mob.alignment}", color=(120, 120, 120))

            dpg.add_spacer(height=10)
            dpg.add_text("Combat Stats", color=(150, 150, 150))
            dpg.add_spacer(height=5)
            dpg.add_text(f"Hit Dice: {mob.hit_dice}", color=(120, 120, 120))
            dpg.add_text(f"Damage Dice: {mob.damage_dice}", color=(120, 120, 120))
            dpg.add_text(f"Hitroll: {mob.hitroll}", color=(120, 120, 120))
            dpg.add_text(f"AC Pierce: {mob.ac_pierce}", color=(120, 120, 120))
            dpg.add_text(f"AC Bash: {mob.ac_bash}", color=(120, 120, 120))
            dpg.add_text(f"AC Slash: {mob.ac_slash}", color=(120, 120, 120))
            dpg.add_text(f"AC Exotic: {mob.ac_exotic}", color=(120, 120, 120))

            # Show equipment and inventory from resets
            self._show_mobile_equipment_in_editor(mob_vnum)

    def _show_mobile_equipment_in_editor(self, mob_vnum: int):
        """Show equipment and inventory for a mobile in the editor.

        Args:
            mob_vnum: The mobile vnum to show equipment for
        """
        # Wear location names
        wear_locations = {
            0: "as a light", 1: "on left finger", 2: "on right finger",
            3: "around neck", 4: "around neck", 5: "on body", 6: "on head",
            7: "on legs", 8: "on feet", 9: "on hands", 10: "on arms",
            11: "as a shield", 12: "about body", 13: "around waist",
            14: "on left wrist", 15: "on right wrist", 16: "wielded", 17: "held in hand"
        }

        # Find all M resets for this mobile
        equipment = []
        inventory = []

        for i, reset in enumerate(app_state.current_area.resets):
            if reset.command == 'M' and reset.arg1 == mob_vnum:
                # Found a mobile reset for this mobile
                # Look for E and G resets that follow
                for j in range(i + 1, len(app_state.current_area.resets)):
                    next_reset = app_state.current_area.resets[j]

                    # Stop at next mobile, object, or end
                    if next_reset.command in ['M', 'O', 'S']:
                        break

                    if next_reset.command == 'E':
                        # Equipment reset
                        obj_vnum = next_reset.arg1
                        wear_loc = next_reset.arg3
                        if obj_vnum in app_state.current_area.objects:
                            obj = app_state.current_area.objects[obj_vnum]
                            loc_name = wear_locations.get(wear_loc, f"in slot {wear_loc}")
                            equipment.append((obj, loc_name))

                    elif next_reset.command == 'G':
                        # Give (inventory) reset
                        obj_vnum = next_reset.arg1
                        if obj_vnum in app_state.current_area.objects:
                            obj = app_state.current_area.objects[obj_vnum]
                            inventory.append(obj)

        # Only display if mobile has equipment or inventory
        if equipment or inventory:
            dpg.add_spacer(height=10)
            dpg.add_text("Equipment & Inventory", color=(150, 150, 150))
            dpg.add_spacer(height=5)

            for obj, loc_name in equipment:
                dpg.add_text(f"  {loc_name}: {obj.short_description}", color=(120, 120, 120))

            for obj in inventory:
                dpg.add_text(f"  carrying: {obj.short_description}", color=(120, 120, 120))

    def show_area_editor(self):
        """Show the area editor for the current area."""
        area = app_state.current_area
        if not area:
            return

        self._clear_editor()
        self._show_editor()
        self.current_vnum = None  # Area doesn't have a vnum

        with dpg.group(parent=self.editor_group_id):
            dpg.add_text("Area Information", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=10)

            dpg.add_text("Area Name:", color=(150, 150, 150))
            dpg.add_input_text(
                default_value=area.name,
                width=-1,
                callback=self._on_area_field_changed,
                user_data='name'
            )
            dpg.add_spacer(height=10)

            dpg.add_text(f"VNUM Range: {area.min_vnum} - {area.max_vnum}", color=(150, 150, 150))
            dpg.add_text(f"Security: {area.security}", color=(150, 150, 150))
            dpg.add_spacer(height=10)

            dpg.add_text("Statistics:", color=(150, 150, 150))
            dpg.add_text(f"  Rooms: {len(area.rooms)}", color=(120, 120, 120))
            dpg.add_text(f"  Objects: {len(area.objects)}", color=(120, 120, 120))
            dpg.add_text(f"  Mobiles: {len(area.mobiles)}", color=(120, 120, 120))

