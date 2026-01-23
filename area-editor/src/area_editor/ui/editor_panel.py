"""
Editor panel for editing areas, rooms, objects, and mobiles.
"""
import dearpygui.dearpygui as dpg
from area_editor.app_state import app_state
from area_editor.ui.mud_terminal import MudTerminal
from area_editor.ui.wrapped_text_input import WrappedTextInput
from area_editor.writers.are_writer import AreWriter
from area_editor.constants import (
    ROOM_FLAGS, SECTOR_TYPES, EXIT_FLAGS,
    get_flag_name, get_sector_name, set_flag, has_flag
)


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
        elif field_name == 'sector_type':
            room.sector_type = app_data
        elif field_name == 'room_flags':
            room.room_flags = app_data

        # Mark as modified
        app_state.mark_modified()

        # Update status bar
        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_file_info(app_state.get_file_info())

        # Auto-save to disk
        self._auto_save()

    def _on_room_flag_changed(self, sender, app_data, user_data):
        """Handle room flag checkbox changes."""
        flag_value, room_vnum = user_data
        room = app_state.current_area.rooms.get(room_vnum)
        if not room:
            return

        # Update the flag
        room.room_flags = set_flag(room.room_flags, flag_value, app_data)

        # Mark as modified
        app_state.mark_modified()

        # Update status bar
        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_file_info(app_state.get_file_info())

        # Auto-save to disk
        self._auto_save()

    def _on_exit_field_changed(self, sender, app_data, user_data):
        """Handle exit field changes."""
        field_name, room_vnum, direction = user_data
        room = app_state.current_area.rooms.get(room_vnum)
        if not room or direction not in room.exits:
            return

        exit_obj = room.exits[direction]

        # Update the exit field
        if field_name == 'to_room':
            exit_obj.to_room = app_data
        elif field_name == 'description':
            exit_obj.description = app_data
        elif field_name == 'keywords':
            exit_obj.keywords = app_data
        elif field_name == 'locks':
            exit_obj.locks = app_data
        elif field_name == 'key_vnum':
            exit_obj.key_vnum = app_data

        # Mark as modified
        app_state.mark_modified()

        # Update status bar
        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_file_info(app_state.get_file_info())

        # Auto-save to disk
        self._auto_save()

    def _on_key_selected(self, sender, app_data, user_data, key_items, key_vnums):
        """Handle key selection from combo box."""
        room_vnum, direction = user_data
        room = app_state.current_area.rooms.get(room_vnum)
        if not room or direction not in room.exits:
            return

        exit_obj = room.exits[direction]

        # Find the selected key vnum
        if app_data in key_items:
            index = key_items.index(app_data)
            selected_vnum = key_vnums[index]
            exit_obj.key_vnum = selected_vnum if selected_vnum > 0 else 0

            # Mark as modified
            app_state.mark_modified()

            # Update status bar
            if hasattr(self.main_window, 'status_bar'):
                self.main_window.status_bar.set_file_info(app_state.get_file_info())

            # Auto-save to disk
            self._auto_save()

            # Refresh the editor to show the updated key
            self.show_room_editor(room_vnum)

    def _on_create_key_clicked(self, sender, app_data, user_data):
        """Handle creating a new key object for an exit."""
        room_vnum, direction = user_data
        room = app_state.current_area.rooms.get(room_vnum)
        if not room or direction not in room.exits:
            return

        exit_obj = room.exits[direction]

        # Get the next available vnum for an object
        from area_editor.models.object import Object
        next_vnum = app_state.current_area.get_next_available_vnum('object')

        if next_vnum == -1:
            print("Error: No available vnums for new object")
            return

        # Create a new key object
        direction_names = ['north', 'east', 'south', 'west', 'up', 'down']
        dir_name = direction_names[direction] if direction < len(direction_names) else f"dir{direction}"

        new_key = Object(
            vnum=next_vnum,
            keywords=f"key {dir_name}",
            short_description=f"a {dir_name} key",
            long_description=f"A small key for the {dir_name} exit lies here.",
            item_type="key",
            material="iron",
            level=1,
            weight=1,
            cost=10,
            condition=100
        )

        # Add the key to the area
        app_state.current_area.objects[next_vnum] = new_key

        # Link the key to the exit
        exit_obj.key_vnum = next_vnum

        # Mark as modified
        app_state.mark_modified()

        # Update status bar
        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_file_info(app_state.get_file_info())

        # Auto-save to disk
        self._auto_save()

        # Refresh the area tree to show the new object
        if hasattr(self.main_window, 'area_tree'):
            self.main_window.area_tree.refresh()

        # Refresh the editor to show the updated key
        self.show_room_editor(room_vnum)

    def _on_add_exit_clicked(self, sender, app_data, user_data):
        """Handle add exit button click."""
        room_vnum = user_data
        room = app_state.current_area.rooms.get(room_vnum)
        if not room:
            return

        # Find first available direction
        for direction in range(6):  # 0-5 for north, east, south, west, up, down
            if direction not in room.exits:
                # Add a new exit
                from area_editor.models.room import Exit
                room.exits[direction] = Exit(
                    direction=direction,
                    to_room=0,
                    description="",
                    keywords="",
                    locks=0,
                    key_vnum=0
                )

                # Mark as modified
                app_state.mark_modified()

                # Refresh the editor
                self.show_room_editor(room_vnum)
                break

    def _on_delete_exit_clicked(self, sender, app_data, user_data):
        """Handle delete exit button click."""
        room_vnum, direction = user_data
        room = app_state.current_area.rooms.get(room_vnum)
        if not room or direction not in room.exits:
            return

        # Delete the exit
        del room.exits[direction]

        # Mark as modified
        app_state.mark_modified()

        # Refresh the editor
        self.show_room_editor(room_vnum)

    def _on_extra_desc_field_changed(self, sender, app_data, user_data):
        """Handle extra description field changes."""
        field_name, item_type, item_vnum, edesc_index = user_data

        if item_type == 'room':
            item = app_state.current_area.rooms.get(item_vnum)
        elif item_type == 'object':
            item = app_state.current_area.objects.get(item_vnum)
        else:
            return

        if not item or edesc_index >= len(item.extra_descriptions):
            return

        edesc = item.extra_descriptions[edesc_index]

        # Update the field
        if field_name == 'keywords':
            edesc.keywords = app_data
        elif field_name == 'description':
            edesc.description = app_data

        # Mark as modified
        app_state.mark_modified()

        # Update status bar
        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_file_info(app_state.get_file_info())

        # Auto-save to disk
        self._auto_save()

    def _on_add_extra_desc_clicked(self, sender, app_data, user_data):
        """Handle add extra description button click."""
        item_type, item_vnum = user_data

        if item_type == 'room':
            item = app_state.current_area.rooms.get(item_vnum)
        elif item_type == 'object':
            item = app_state.current_area.objects.get(item_vnum)
        else:
            return

        if not item:
            return

        # Add a new extra description
        from area_editor.models.room import ExtraDescription
        item.extra_descriptions.append(ExtraDescription(
            keywords="new keywords",
            description="New extra description.\n"
        ))

        # Mark as modified
        app_state.mark_modified()

        # Refresh the editor
        if item_type == 'room':
            self.show_room_editor(item_vnum)
        elif item_type == 'object':
            self.show_object_editor(item_vnum)

    def _on_delete_extra_desc_clicked(self, sender, app_data, user_data):
        """Handle delete extra description button click."""
        item_type, item_vnum, edesc_index = user_data

        if item_type == 'room':
            item = app_state.current_area.rooms.get(item_vnum)
        elif item_type == 'object':
            item = app_state.current_area.objects.get(item_vnum)
        else:
            return

        if not item or edesc_index >= len(item.extra_descriptions):
            return

        # Delete the extra description
        del item.extra_descriptions[edesc_index]

        # Mark as modified
        app_state.mark_modified()

        # Refresh the editor
        if item_type == 'room':
            self.show_room_editor(item_vnum)
        elif item_type == 'object':
            self.show_object_editor(item_vnum)

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
        elif field_name == 'keywords':
            obj.keywords = app_data
        elif field_name == 'material':
            obj.material = app_data
        elif field_name == 'item_type':
            obj.item_type = app_data
        elif field_name == 'level':
            obj.level = app_data
        elif field_name == 'weight':
            obj.weight = app_data
        elif field_name == 'cost':
            obj.cost = app_data
        elif field_name == 'condition':
            obj.condition = app_data
        elif field_name == 'extra_flags':
            obj.extra_flags = app_data
        elif field_name == 'wear_flags':
            obj.wear_flags = app_data

        # Mark as modified
        app_state.mark_modified()

        # Update status bar
        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_file_info(app_state.get_file_info())

        # Auto-save to disk
        self._auto_save()

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
        elif field_name == 'description':
            mob.description = app_data
        elif field_name == 'race':
            mob.race = app_data
        elif field_name == 'level':
            mob.level = app_data
        elif field_name == 'alignment':
            mob.alignment = app_data
        elif field_name == 'hitroll':
            mob.hitroll = app_data
        elif field_name == 'hit_dice':
            mob.hit_dice = app_data
        elif field_name == 'mana_dice':
            mob.mana_dice = app_data
        elif field_name == 'damage_dice':
            mob.damage_dice = app_data
        elif field_name == 'ac_pierce':
            mob.ac_pierce = app_data
        elif field_name == 'ac_bash':
            mob.ac_bash = app_data
        elif field_name == 'ac_slash':
            mob.ac_slash = app_data
        elif field_name == 'ac_exotic':
            mob.ac_exotic = app_data
        elif field_name == 'gold':
            mob.gold = app_data
        elif field_name == 'sex':
            mob.sex = app_data
        elif field_name == 'size':
            mob.size = app_data
        elif field_name == 'material':
            mob.material = app_data
        elif field_name == 'start_position':
            mob.start_position = app_data
        elif field_name == 'default_position':
            mob.default_position = app_data

        # Mark as modified
        app_state.mark_modified()

        # Update status bar
        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_file_info(app_state.get_file_info())

        # Auto-save to disk
        self._auto_save()

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

        # Auto-save to disk
        self._auto_save()

    def _auto_save(self):
        """Auto-save the current area to disk and refresh tree."""
        if not app_state.current_area or not app_state.current_file:
            return

        try:
            # Write the area to disk
            writer = AreWriter(app_state.current_area)
            writer.write(app_state.current_file)

            # Clear the modified flag
            app_state.area_modified[app_state.current_area_id] = False

            # Refresh the tree view to show updated room/object/mobile names
            if hasattr(self.main_window, 'area_tree') and app_state.current_area_id:
                self.main_window.area_tree.populate_from_area(
                    app_state.current_area,
                    app_state.current_area_id
                )

            # Update status bar (quietly, no success message for auto-save)
            if hasattr(self.main_window, 'status_bar'):
                self.main_window.status_bar.set_file_info(app_state.get_file_info())

        except Exception as e:
            if hasattr(self.main_window, 'status_bar'):
                self.main_window.status_bar.set_status(
                    f"Error auto-saving: {e}",
                    color=(255, 100, 100)
                )
            print(f"Error auto-saving area file: {e}")
            import traceback
            traceback.print_exc()

    def _on_save_shortcut(self):
        """Handle Ctrl+S save shortcut."""
        if not app_state.current_area or not app_state.current_file:
            if hasattr(self.main_window, 'status_bar'):
                self.main_window.status_bar.set_status(
                    "No area file to save",
                    color=(255, 100, 100)
                )
            return

        try:
            # Write the area to disk
            writer = AreWriter(app_state.current_area)
            writer.write(app_state.current_file)

            # Clear the modified flag
            app_state.area_modified[app_state.current_area_id] = False

            # Refresh the tree view to show updated names
            if hasattr(self.main_window, 'area_tree') and app_state.current_area_id:
                self.main_window.area_tree.populate_from_area(
                    app_state.current_area,
                    app_state.current_area_id
                )

            # Update status bar with success message
            if hasattr(self.main_window, 'status_bar'):
                self.main_window.status_bar.set_status(
                    f"Saved {app_state.current_file.name}",
                    color=(100, 255, 100)
                )
                self.main_window.status_bar.set_file_info(app_state.get_file_info())

        except Exception as e:
            if hasattr(self.main_window, 'status_bar'):
                self.main_window.status_bar.set_status(
                    f"Error saving file: {e}",
                    color=(255, 100, 100)
                )
            print(f"Error saving area file: {e}")
            import traceback
            traceback.print_exc()

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

            # Create tab bar for room editor
            with dpg.tab_bar():
                # General Tab
                with dpg.tab(label="General"):
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
                    dpg.add_text("(Auto-wraps at 80 characters when you unfocus, auto-resizes to fit content)", color=(120, 120, 120))
                    # Use wrapped text input that auto-wraps at 80 characters and auto-resizes
                    wrapped_input = WrappedTextInput(
                        default_value=room.description,
                        callback=self._on_room_field_changed,
                        user_data=('description', room_vnum),
                        width=-1,
                        min_height=60,
                        line_height=20
                    )
                    wrapped_input.create()
                    dpg.add_spacer(height=10)

                    # Sector Type dropdown
                    dpg.add_text("Sector Type:", color=(150, 150, 150))
                    sector_items = [f"{name} - {desc}" for name, value, desc in SECTOR_TYPES]
                    dpg.add_combo(
                        items=sector_items,
                        default_value=sector_items[room.sector_type] if 0 <= room.sector_type < len(sector_items) else sector_items[0],
                        width=300,
                        callback=lambda s, a, u: self._on_room_field_changed(
                            s,
                            SECTOR_TYPES[sector_items.index(a)][1],  # Get the value from the selected item
                            ('sector_type', room_vnum)
                        )
                    )

                # Flags Tab
                with dpg.tab(label="Flags"):
                    dpg.add_spacer(height=10)
                    dpg.add_text("Room Flags:", color=(150, 150, 150))
                    dpg.add_text("(Check the flags that apply to this room)", color=(120, 120, 120))
                    dpg.add_spacer(height=5)

                    # Display flags as a vertical list
                    for flag_name, flag_value, flag_desc in ROOM_FLAGS:
                        dpg.add_checkbox(
                            label=f"{flag_name} - {flag_desc}",
                            default_value=has_flag(room.room_flags, flag_value),
                            callback=self._on_room_flag_changed,
                            user_data=(flag_value, room_vnum)
                        )

                # Exits Tab
                with dpg.tab(label="Exits"):
                    dpg.add_spacer(height=10)
                    self._show_exits_editor(room, room_vnum)

                # Extra Descriptions Tab
                with dpg.tab(label="Extra Descriptions"):
                    dpg.add_spacer(height=10)
                    self._show_extra_descriptions_editor(room, room_vnum)

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

    def _show_exits_editor(self, room, room_vnum):
        """Show the exits editor section."""
        dpg.add_text(f"Exits ({len(room.exits)}):", color=(150, 150, 150))
        dpg.add_text("(Edit exit properties below)", color=(120, 120, 120))
        dpg.add_spacer(height=5)

        direction_names = ['north', 'east', 'south', 'west', 'up', 'down']

        # Display existing exits with edit controls
        if room.exits:
            for direction in sorted(room.exits.keys()):
                exit_obj = room.exits[direction]
                dir_name = direction_names[direction] if direction < len(direction_names) else f"dir{direction}"

                with dpg.child_window(height=240, border=True):
                    # Header with direction and delete button
                    with dpg.group(horizontal=True):
                        dpg.add_text(f"Exit: {dir_name.upper()}", color=(200, 200, 100))
                        dpg.add_spacer(width=10)
                        dpg.add_button(
                            label="Delete Exit",
                            callback=self._on_delete_exit_clicked,
                            user_data=(room_vnum, direction),
                            width=100,
                            height=20
                        )

                    dpg.add_separator()
                    dpg.add_spacer(height=5)

                    # To Room field with navigation button
                    with dpg.group(horizontal=True):
                        dpg.add_text("To Room:", color=(150, 150, 150))
                        dpg.add_input_int(
                            default_value=exit_obj.to_room,
                            width=100,
                            callback=self._on_exit_field_changed,
                            user_data=('to_room', room_vnum, direction)
                        )
                        dpg.add_button(
                            label=f"Go to #{exit_obj.to_room}",
                            callback=self._on_room_link_clicked,
                            user_data=exit_obj.to_room,
                            width=100,
                            height=20
                        )

                    # Description field
                    with dpg.group(horizontal=True):
                        dpg.add_text("Description:", color=(150, 150, 150))
                        dpg.add_input_text(
                            default_value=exit_obj.description,
                            width=400,
                            callback=self._on_exit_field_changed,
                            user_data=('description', room_vnum, direction)
                        )

                    # Keywords field
                    with dpg.group(horizontal=True):
                        dpg.add_text("Keywords:", color=(150, 150, 150))
                        dpg.add_input_text(
                            default_value=exit_obj.keywords,
                            width=400,
                            callback=self._on_exit_field_changed,
                            user_data=('keywords', room_vnum, direction)
                        )

                    # Lock Flags field
                    with dpg.group(horizontal=True):
                        dpg.add_text("Lock Flags:", color=(150, 150, 150))
                        dpg.add_input_int(
                            default_value=exit_obj.locks,
                            width=100,
                            callback=self._on_exit_field_changed,
                            user_data=('locks', room_vnum, direction)
                        )

                    dpg.add_spacer(height=5)

                    # Key selector - improved UI
                    dpg.add_text("Key:", color=(150, 150, 150))
                    with dpg.group(horizontal=True):
                        # Get all key objects in the area
                        key_objects = []
                        if app_state.current_area:
                            for obj_vnum, obj in app_state.current_area.objects.items():
                                if obj.item_type.lower() == "key":
                                    key_objects.append((obj_vnum, obj.short_description))

                        # Build combo items
                        key_items = ["(No Key)"]
                        key_vnums = [-1]  # -1 means no key
                        for vnum, desc in sorted(key_objects):
                            key_items.append(f"#{vnum}: {desc}")
                            key_vnums.append(vnum)

                        # Find current selection
                        current_key_vnum = exit_obj.key_vnum if exit_obj.key_vnum > 0 else -1
                        if current_key_vnum in key_vnums:
                            default_value = key_items[key_vnums.index(current_key_vnum)]
                        else:
                            default_value = "(No Key)"

                        dpg.add_combo(
                            items=key_items,
                            default_value=default_value,
                            width=300,
                            callback=lambda s, a, u: self._on_key_selected(s, a, u, key_items, key_vnums),
                            user_data=(room_vnum, direction)
                        )

                        dpg.add_button(
                            label="Create New Key",
                            callback=self._on_create_key_clicked,
                            user_data=(room_vnum, direction),
                            width=120,
                            height=20
                        )

                dpg.add_spacer(height=5)

        # Add exit button
        dpg.add_button(
            label="+ Add New Exit",
            callback=self._on_add_exit_clicked,
            user_data=room_vnum,
            width=150,
            height=25
        )

    def _show_extra_descriptions_editor(self, room, room_vnum):
        """Show the extra descriptions editor section."""
        dpg.add_text(f"Extra Descriptions ({len(room.extra_descriptions)}):", color=(150, 150, 150))
        dpg.add_text("(Extra descriptions are shown when players 'look' at keywords)", color=(120, 120, 120))
        dpg.add_spacer(height=5)

        if room.extra_descriptions:
            for i, edesc in enumerate(room.extra_descriptions):
                with dpg.child_window(height=150, border=True):
                    # Header with delete button
                    with dpg.group(horizontal=True):
                        dpg.add_text(f"Extra Description #{i+1}", color=(200, 200, 100))
                        dpg.add_spacer(width=10)
                        dpg.add_button(
                            label="Delete",
                            callback=self._on_delete_extra_desc_clicked,
                            user_data=('room', room_vnum, i),
                            width=80,
                            height=20
                        )

                    dpg.add_separator()
                    dpg.add_spacer(height=5)

                    # Keywords field
                    with dpg.group(horizontal=True):
                        dpg.add_text("Keywords:", color=(150, 150, 150))
                        dpg.add_input_text(
                            default_value=edesc.keywords,
                            width=400,
                            callback=self._on_extra_desc_field_changed,
                            user_data=('keywords', 'room', room_vnum, i)
                        )

                    # Description field
                    dpg.add_text("Description:", color=(150, 150, 150))
                    dpg.add_input_text(
                        default_value=edesc.description,
                        multiline=True,
                        height=60,
                        width=-1,
                        callback=self._on_extra_desc_field_changed,
                        user_data=('description', 'room', room_vnum, i)
                    )

                dpg.add_spacer(height=5)

        # Add extra description button
        dpg.add_button(
            label="+ Add Extra Description",
            callback=self._on_add_extra_desc_clicked,
            user_data=('room', room_vnum),
            width=200,
            height=25
        )

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
            dpg.add_text("(Auto-wraps at 80 characters when you unfocus, auto-resizes to fit content)", color=(120, 120, 120))
            # Use wrapped text input that auto-wraps at 80 characters and auto-resizes
            wrapped_input = WrappedTextInput(
                default_value=obj.long_description,
                callback=self._on_object_field_changed,
                user_data=('long_description', obj_vnum),
                width=-1,
                min_height=40,
                line_height=20
            )
            wrapped_input.create()
            dpg.add_spacer(height=10)

            # Keywords
            dpg.add_text("Keywords:", color=(150, 150, 150))
            dpg.add_text("(Space-separated keywords for identifying the object)", color=(120, 120, 120))
            dpg.add_input_text(
                default_value=obj.keywords,
                width=-1,
                callback=self._on_object_field_changed,
                user_data=('keywords', obj_vnum)
            )
            dpg.add_spacer(height=10)

            # Basic Properties Section
            dpg.add_text("Basic Properties", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=5)

            # Item Type
            with dpg.group(horizontal=True):
                dpg.add_text("Item Type:", color=(150, 150, 150))
                dpg.add_input_text(
                    default_value=obj.item_type,
                    width=200,
                    callback=self._on_object_field_changed,
                    user_data=('item_type', obj_vnum)
                )
                dpg.add_text("(e.g., weapon, armor, potion, treasure)", color=(100, 100, 100))

            # Material
            with dpg.group(horizontal=True):
                dpg.add_text("Material:", color=(150, 150, 150))
                dpg.add_input_text(
                    default_value=obj.material,
                    width=200,
                    callback=self._on_object_field_changed,
                    user_data=('material', obj_vnum)
                )
                dpg.add_text("(e.g., iron, wood, cloth, gold)", color=(100, 100, 100))

            dpg.add_spacer(height=10)

            # Stats Section
            dpg.add_text("Stats", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=5)

            # Level
            with dpg.group(horizontal=True):
                dpg.add_text("Level:", color=(150, 150, 150))
                dpg.add_input_int(
                    default_value=obj.level,
                    width=100,
                    callback=self._on_object_field_changed,
                    user_data=('level', obj_vnum)
                )

            # Weight
            with dpg.group(horizontal=True):
                dpg.add_text("Weight:", color=(150, 150, 150))
                dpg.add_input_int(
                    default_value=obj.weight,
                    width=100,
                    callback=self._on_object_field_changed,
                    user_data=('weight', obj_vnum)
                )
                dpg.add_text("(in pounds)", color=(100, 100, 100))

            # Cost
            with dpg.group(horizontal=True):
                dpg.add_text("Cost:", color=(150, 150, 150))
                dpg.add_input_int(
                    default_value=obj.cost,
                    width=100,
                    callback=self._on_object_field_changed,
                    user_data=('cost', obj_vnum)
                )
                dpg.add_text("(in gold)", color=(100, 100, 100))

            # Condition
            with dpg.group(horizontal=True):
                dpg.add_text("Condition:", color=(150, 150, 150))
                dpg.add_input_int(
                    default_value=obj.condition,
                    width=100,
                    callback=self._on_object_field_changed,
                    user_data=('condition', obj_vnum)
                )
                dpg.add_text("(0-100, 100=perfect)", color=(100, 100, 100))

            dpg.add_spacer(height=10)

            # Affects Section
            dpg.add_text(f"Affects ({len(obj.affects)}):", color=(150, 150, 150))
            dpg.add_text("(Stat modifiers when equipped)", color=(120, 120, 120))
            dpg.add_spacer(height=5)

            if obj.affects:
                for i, affect in enumerate(obj.affects):
                    with dpg.group(horizontal=True):
                        dpg.add_text(f"#{i+1}:", color=(150, 150, 150))
                        dpg.add_text(f"Apply Type: {affect.apply_type}", color=(120, 120, 120))
                        dpg.add_text(f"Modifier: {affect.modifier:+d}", color=(120, 120, 120))
                dpg.add_spacer(height=5)

            dpg.add_text("(Affect editing coming soon)", color=(100, 100, 100))
            dpg.add_spacer(height=10)

            # Extra Descriptions section
            dpg.add_spacer(height=10)
            dpg.add_text(f"Extra Descriptions ({len(obj.extra_descriptions)}):", color=(150, 150, 150))
            dpg.add_text("(Extra descriptions are shown when players 'look' or 'examine' the object)", color=(120, 120, 120))
            dpg.add_spacer(height=5)

            if obj.extra_descriptions:
                for i, edesc in enumerate(obj.extra_descriptions):
                    with dpg.child_window(height=150, border=True):
                        # Header with delete button
                        with dpg.group(horizontal=True):
                            dpg.add_text(f"Extra Description #{i+1}", color=(200, 200, 100))
                            dpg.add_spacer(width=10)
                            dpg.add_button(
                                label="Delete",
                                callback=self._on_delete_extra_desc_clicked,
                                user_data=('object', obj_vnum, i),
                                width=80,
                                height=20
                            )

                        dpg.add_separator()
                        dpg.add_spacer(height=5)

                        # Keywords field
                        with dpg.group(horizontal=True):
                            dpg.add_text("Keywords:", color=(150, 150, 150))
                            dpg.add_input_text(
                                default_value=edesc.keywords,
                                width=400,
                                callback=self._on_extra_desc_field_changed,
                                user_data=('keywords', 'object', obj_vnum, i)
                            )

                        # Description field
                        dpg.add_text("Description:", color=(150, 150, 150))
                        dpg.add_input_text(
                            default_value=edesc.description,
                            multiline=True,
                            height=60,
                            width=-1,
                            callback=self._on_extra_desc_field_changed,
                            user_data=('description', 'object', obj_vnum, i)
                        )

                    dpg.add_spacer(height=5)

            # Add extra description button
            dpg.add_button(
                label="+ Add Extra Description",
                callback=self._on_add_extra_desc_clicked,
                user_data=('object', obj_vnum),
                width=200,
                height=25
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
            dpg.add_text("(Auto-wraps at 80 characters when you unfocus, auto-resizes to fit content)", color=(120, 120, 120))
            # Use wrapped text input that auto-wraps at 80 characters and auto-resizes
            wrapped_input = WrappedTextInput(
                default_value=mob.long_description,
                callback=self._on_mobile_field_changed,
                user_data=('long_description', mob_vnum),
                width=-1,
                min_height=40,
                line_height=20
            )
            wrapped_input.create()
            dpg.add_spacer(height=10)

            # Description (detailed description shown when looking at mobile)
            dpg.add_text("Description:", color=(150, 150, 150))
            dpg.add_text("(Shown when players 'look' at the mobile)", color=(120, 120, 120))
            dpg.add_input_text(
                default_value=mob.description,
                multiline=True,
                height=80,
                width=-1,
                callback=self._on_mobile_field_changed,
                user_data=('description', mob_vnum)
            )
            dpg.add_spacer(height=10)

            # Basic Stats Section
            dpg.add_text("Basic Stats", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=5)

            # Race
            with dpg.group(horizontal=True):
                dpg.add_text("Race:", color=(150, 150, 150))
                dpg.add_input_text(
                    default_value=mob.race,
                    width=200,
                    callback=self._on_mobile_field_changed,
                    user_data=('race', mob_vnum)
                )

            # Level
            with dpg.group(horizontal=True):
                dpg.add_text("Level:", color=(150, 150, 150))
                dpg.add_input_int(
                    default_value=mob.level,
                    width=100,
                    callback=self._on_mobile_field_changed,
                    user_data=('level', mob_vnum)
                )

            # Alignment
            with dpg.group(horizontal=True):
                dpg.add_text("Alignment:", color=(150, 150, 150))
                dpg.add_input_int(
                    default_value=mob.alignment,
                    width=100,
                    callback=self._on_mobile_field_changed,
                    user_data=('alignment', mob_vnum)
                )
                dpg.add_text("(-1000 to 1000, negative=evil, positive=good)", color=(100, 100, 100))

            # Gold
            with dpg.group(horizontal=True):
                dpg.add_text("Gold:", color=(150, 150, 150))
                dpg.add_input_int(
                    default_value=mob.gold,
                    width=100,
                    callback=self._on_mobile_field_changed,
                    user_data=('gold', mob_vnum)
                )

            dpg.add_spacer(height=10)

            # Combat Stats Section
            dpg.add_text("Combat Stats", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=5)

            # Hit Dice
            with dpg.group(horizontal=True):
                dpg.add_text("Hit Dice:", color=(150, 150, 150))
                dpg.add_input_text(
                    default_value=mob.hit_dice,
                    width=150,
                    callback=self._on_mobile_field_changed,
                    user_data=('hit_dice', mob_vnum)
                )
                dpg.add_text("(e.g., 10d10+100)", color=(100, 100, 100))

            # Mana Dice
            with dpg.group(horizontal=True):
                dpg.add_text("Mana Dice:", color=(150, 150, 150))
                dpg.add_input_text(
                    default_value=mob.mana_dice,
                    width=150,
                    callback=self._on_mobile_field_changed,
                    user_data=('mana_dice', mob_vnum)
                )
                dpg.add_text("(e.g., 100d1+500)", color=(100, 100, 100))

            # Damage Dice
            with dpg.group(horizontal=True):
                dpg.add_text("Damage Dice:", color=(150, 150, 150))
                dpg.add_input_text(
                    default_value=mob.damage_dice,
                    width=150,
                    callback=self._on_mobile_field_changed,
                    user_data=('damage_dice', mob_vnum)
                )
                dpg.add_text("(e.g., 1d6+2)", color=(100, 100, 100))

            # Hitroll
            with dpg.group(horizontal=True):
                dpg.add_text("Hitroll:", color=(150, 150, 150))
                dpg.add_input_int(
                    default_value=mob.hitroll,
                    width=100,
                    callback=self._on_mobile_field_changed,
                    user_data=('hitroll', mob_vnum)
                )

            dpg.add_spacer(height=5)

            # Armor Class
            dpg.add_text("Armor Class (AC):", color=(150, 150, 150))
            with dpg.group(horizontal=True):
                dpg.add_text("Pierce:", color=(150, 150, 150))
                dpg.add_input_int(
                    default_value=mob.ac_pierce,
                    width=80,
                    callback=self._on_mobile_field_changed,
                    user_data=('ac_pierce', mob_vnum)
                )
                dpg.add_spacer(width=10)
                dpg.add_text("Bash:", color=(150, 150, 150))
                dpg.add_input_int(
                    default_value=mob.ac_bash,
                    width=80,
                    callback=self._on_mobile_field_changed,
                    user_data=('ac_bash', mob_vnum)
                )
                dpg.add_spacer(width=10)
                dpg.add_text("Slash:", color=(150, 150, 150))
                dpg.add_input_int(
                    default_value=mob.ac_slash,
                    width=80,
                    callback=self._on_mobile_field_changed,
                    user_data=('ac_slash', mob_vnum)
                )
                dpg.add_spacer(width=10)
                dpg.add_text("Exotic:", color=(150, 150, 150))
                dpg.add_input_int(
                    default_value=mob.ac_exotic,
                    width=80,
                    callback=self._on_mobile_field_changed,
                    user_data=('ac_exotic', mob_vnum)
                )

            dpg.add_spacer(height=10)

            # Physical Attributes Section
            dpg.add_text("Physical Attributes", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=5)

            # Sex
            with dpg.group(horizontal=True):
                dpg.add_text("Sex:", color=(150, 150, 150))
                sex_items = ["none", "male", "female", "either"]
                sex_value = sex_items[mob.sex] if 0 <= mob.sex < len(sex_items) else "none"
                dpg.add_combo(
                    items=sex_items,
                    default_value=sex_value,
                    width=150,
                    callback=lambda s, a, u: self._on_mobile_field_changed(s, sex_items.index(a), u),
                    user_data=('sex', mob_vnum)
                )

            # Size
            with dpg.group(horizontal=True):
                dpg.add_text("Size:", color=(150, 150, 150))
                size_items = ["tiny", "small", "medium", "large", "huge", "giant"]
                size_value = size_items[mob.size] if 0 <= mob.size < len(size_items) else "medium"
                dpg.add_combo(
                    items=size_items,
                    default_value=size_value,
                    width=150,
                    callback=lambda s, a, u: self._on_mobile_field_changed(s, size_items.index(a), u),
                    user_data=('size', mob_vnum)
                )

            # Material
            with dpg.group(horizontal=True):
                dpg.add_text("Material:", color=(150, 150, 150))
                dpg.add_input_text(
                    default_value=mob.material,
                    width=200,
                    callback=self._on_mobile_field_changed,
                    user_data=('material', mob_vnum)
                )
                dpg.add_text("(e.g., flesh, stone, wood)", color=(100, 100, 100))

            dpg.add_spacer(height=10)

            # Position Section
            dpg.add_text("Positions", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=5)

            position_items = ["dead", "mortal", "incap", "stunned", "sleeping", "resting", "sitting", "fighting", "standing"]

            # Start Position
            with dpg.group(horizontal=True):
                dpg.add_text("Start Position:", color=(150, 150, 150))
                start_pos_value = position_items[mob.start_position] if 0 <= mob.start_position < len(position_items) else "standing"
                dpg.add_combo(
                    items=position_items,
                    default_value=start_pos_value,
                    width=150,
                    callback=lambda s, a, u: self._on_mobile_field_changed(s, position_items.index(a), u),
                    user_data=('start_position', mob_vnum)
                )

            # Default Position
            with dpg.group(horizontal=True):
                dpg.add_text("Default Position:", color=(150, 150, 150))
                default_pos_value = position_items[mob.default_position] if 0 <= mob.default_position < len(position_items) else "standing"
                dpg.add_combo(
                    items=position_items,
                    default_value=default_pos_value,
                    width=150,
                    callback=lambda s, a, u: self._on_mobile_field_changed(s, position_items.index(a), u),
                    user_data=('default_position', mob_vnum)
                )

            dpg.add_spacer(height=10)

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

