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

    def _on_door_preset_changed(self, sender, app_data, user_data):
        """Handle door preset dropdown changes."""
        room_vnum, direction = user_data

        # Door preset configurations (name, flags value)
        door_presets = {
            "No Door (Passageway)": 0,
            "Open Door": 1,  # door
            "Closed Door": 3,  # door + closed
            "Locked Door": 7,  # door + closed + locked
            "Pickproof Locked Door": 15,  # door + closed + locked + pickproof
            "Hidden Passage": 2048,  # nonobvious
            "Hidden Door": 2049,  # door + nonobvious
            "Hidden Closed Door": 2051,  # door + closed + nonobvious
            "Hidden Locked Door": 2055,  # door + closed + locked + nonobvious
            "Hidden Pickproof Locked Door": 2063,  # door + closed + locked + pickproof + nonobvious
        }

        # Get the flags value for the selected preset
        flags_value = door_presets.get(app_data, 0)

        # Update the exit with the new flags
        self._on_exit_field_changed(sender, flags_value, ('locks', room_vnum, direction))

        # Refresh the editor to show the updated flags
        self.show_room_editor(room_vnum)

    def _on_exit_field_changed(self, sender, app_data, user_data):
        """Handle exit field changes."""
        field_name, room_vnum, direction = user_data
        room = app_state.current_area.rooms.get(room_vnum)
        if not room:
            return

        # Create exit if it doesn't exist (this happens when user edits a direction for the first time)
        if direction not in room.exits:
            from area_editor.models.room import Exit
            room.exits[direction] = Exit(direction=direction, to_room=0)

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

    def _on_add_direction_description_clicked(self, sender, app_data, user_data):
        """Handle add direction description button click."""
        room_vnum, direction = user_data
        room = app_state.current_area.rooms.get(room_vnum)
        if not room:
            return

        # Add a new direction description (with to_room=0, meaning no actual exit)
        from area_editor.models.room import Exit
        room.exits[direction] = Exit(
            direction=direction,
            to_room=0  # 0 means description only, no actual exit
        )

        # Mark as modified
        app_state.mark_modified()

        # Refresh the editor
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
                    to_room=0
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

    def _on_add_mobile_reset_clicked(self, sender, app_data, user_data):
        """Handle add mobile reset button click - show modal to select mobile."""
        room_vnum = user_data

        # Check if mobiles exist
        if not app_state.current_area.mobiles:
            # Show option to create a new mobile
            self._show_create_mobile_prompt(room_vnum)
            return

        # Show modal to select mobile
        self._show_add_mobile_modal(room_vnum)

    def _on_delete_mobile_reset_clicked(self, sender, app_data, user_data):
        """Handle delete mobile reset button click."""
        room_vnum, reset_index = user_data

        if reset_index < len(app_state.current_area.resets):
            # Delete the reset
            del app_state.current_area.resets[reset_index]

            # Mark as modified
            app_state.mark_modified()

            # Refresh the editor
            self.show_room_editor(room_vnum)

            if hasattr(self.main_window, 'status_bar'):
                self.main_window.status_bar.set_status(
                    f"Deleted mobile reset from room #{room_vnum}",
                    color=(100, 255, 100)
                )

    def _on_mobile_reset_field_changed(self, sender, app_data, user_data):
        """Handle mobile reset field changes."""
        field_name, reset_index = user_data

        if reset_index >= len(app_state.current_area.resets):
            return

        reset = app_state.current_area.resets[reset_index]

        # Update the field
        if field_name == 'mobile_vnum':
            # Parse vnum from combo selection (format: "#vnum: description")
            vnum_str = app_data.split(':')[0].strip('#')
            try:
                reset.arg1 = int(vnum_str)
            except ValueError:
                return
        elif field_name == 'max_world':
            reset.arg2 = app_data
        elif field_name == 'max_room':
            reset.arg4 = app_data

        # Mark as modified
        app_state.mark_modified()

        # Auto-save
        self._auto_save()

    def _on_add_shop_clicked(self, sender, app_data, user_data):
        """Handle add shop button click."""
        from area_editor.models.shop import Shop

        mob_vnum = user_data

        # Check if shop already exists
        for shop in app_state.current_area.shops:
            if shop.keeper == mob_vnum:
                if hasattr(self.main_window, 'status_bar'):
                    self.main_window.status_bar.set_status(
                        f"Mobile #{mob_vnum} already has a shop",
                        color=(255, 100, 100)
                    )
                return

        # Create a new shop
        new_shop = Shop(
            keeper=mob_vnum,
            buy_types=[0, 0, 0, 0, 0],  # No item types by default
            profit_buy=120,  # 120% markup when buying from players
            profit_sell=80,  # 80% markdown when selling to players
            open_hour=0,
            close_hour=23
        )

        # Add to shops list
        app_state.current_area.shops.append(new_shop)

        # Mark as modified
        app_state.mark_modified()

        # Refresh the editor
        if self.current_vnum:
            self.show_room_editor(self.current_vnum)

        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_status(
                f"Added shop to mobile #{mob_vnum}",
                color=(100, 255, 100)
            )

    def _on_remove_shop_clicked(self, sender, app_data, user_data):
        """Handle remove shop button click."""
        mob_vnum = user_data

        # Find and remove the shop
        for i, shop in enumerate(app_state.current_area.shops):
            if shop.keeper == mob_vnum:
                del app_state.current_area.shops[i]

                # Mark as modified
                app_state.mark_modified()

                # Refresh the editor
                if self.current_vnum:
                    self.show_room_editor(self.current_vnum)

                if hasattr(self.main_window, 'status_bar'):
                    self.main_window.status_bar.set_status(
                        f"Removed shop from mobile #{mob_vnum}",
                        color=(100, 255, 100)
                    )
                return

        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_status(
                f"No shop found for mobile #{mob_vnum}",
                color=(255, 100, 100)
            )

    def _on_edit_shop_clicked(self, sender, app_data, user_data):
        """Handle edit shop button click."""
        mob_vnum = user_data

        # Find the shop
        shop = None
        for s in app_state.current_area.shops:
            if s.keeper == mob_vnum:
                shop = s
                break

        if not shop:
            if hasattr(self.main_window, 'status_bar'):
                self.main_window.status_bar.set_status(
                    f"No shop found for mobile #{mob_vnum}",
                    color=(255, 100, 100)
                )
            return

        # Create a modal window for editing the shop
        self._show_shop_editor_modal(shop, mob_vnum)

    def _show_create_mobile_prompt(self, room_vnum=None):
        """Show a prompt to create a new mobile when none exist."""
        modal_tag = "create_mobile_prompt"
        with dpg.window(
            label="No Mobiles Found",
            modal=True,
            show=True,
            width=400,
            height=200,
            pos=[400, 250],
            tag=modal_tag
        ):
            dpg.add_text("No mobiles exist in this area yet.", color=(200, 200, 100))
            dpg.add_spacer(height=10)
            dpg.add_text("Would you like to create a new mobile?", color=(150, 150, 150))
            dpg.add_spacer(height=20)

            with dpg.group(horizontal=True):
                dpg.add_button(
                    label="Create New Mobile",
                    callback=lambda: self._on_create_mobile_from_prompt(modal_tag, room_vnum),
                    width=150
                )
                dpg.add_button(
                    label="Cancel",
                    callback=lambda: dpg.delete_item(modal_tag),
                    width=100
                )

    def _on_create_mobile_from_prompt(self, modal_tag, room_vnum):
        """Handle creating a mobile from the prompt."""
        dpg.delete_item(modal_tag)
        self._show_create_mobile_modal(room_vnum)

    def _show_create_mobile_modal(self, room_vnum=None):
        """Show a modal window for creating a new mobile."""
        from area_editor.models.mobile import Mobile

        # Find next available vnum
        area = app_state.current_area
        next_vnum = None
        for vnum in range(area.min_vnum, area.max_vnum + 1):
            if vnum not in area.mobiles:
                next_vnum = vnum
                break

        if next_vnum is None:
            if hasattr(self.main_window, 'status_bar'):
                self.main_window.status_bar.set_status(
                    f"No available vnums in area range {area.min_vnum}-{area.max_vnum}",
                    color=(255, 100, 100)
                )
            return

        modal_tag = "create_mobile_modal"
        with dpg.window(
            label="Create New Mobile",
            modal=True,
            show=True,
            width=600,
            height=500,
            pos=[300, 150],
            tag=modal_tag
        ):
            dpg.add_text("Create New Mobile", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=10)

            # Vnum (read-only)
            with dpg.group(horizontal=True):
                dpg.add_text("Vnum:", color=(150, 150, 150))
                dpg.add_spacer(width=85)
                dpg.add_text(f"#{next_vnum}", color=(100, 255, 100))
                dpg.add_text("(auto-assigned)", color=(120, 120, 120))

            dpg.add_spacer(height=10)

            # Keywords
            dpg.add_text("Keywords:", color=(150, 150, 150))
            dpg.add_text("(Space-separated words for targeting, e.g., 'guard town soldier')", color=(120, 120, 120))
            dpg.add_input_text(
                default_value="",
                width=-1,
                tag="new_mobile_keywords"
            )

            dpg.add_spacer(height=10)

            # Short description
            dpg.add_text("Short Description:", color=(150, 150, 150))
            dpg.add_text("(How the mobile appears in the room, e.g., 'a town guard')", color=(120, 120, 120))
            dpg.add_input_text(
                default_value="",
                width=-1,
                tag="new_mobile_short_desc"
            )

            dpg.add_spacer(height=10)

            # Long description
            dpg.add_text("Long Description (optional):", color=(150, 150, 150))
            dpg.add_text("(Detailed description when examining, e.g., 'This guard watches over the town.')", color=(120, 120, 120))
            dpg.add_input_text(
                default_value="",
                width=-1,
                tag="new_mobile_long_desc"
            )

            dpg.add_spacer(height=10)

            # Level
            with dpg.group(horizontal=True):
                dpg.add_text("Level:", color=(150, 150, 150))
                dpg.add_spacer(width=75)
                dpg.add_input_int(
                    default_value=1,
                    width=100,
                    tag="new_mobile_level",
                    min_value=1,
                    max_value=60,
                    min_clamped=True,
                    max_clamped=True
                )

            dpg.add_spacer(height=10)

            # Alignment
            with dpg.group(horizontal=True):
                dpg.add_text("Alignment:", color=(150, 150, 150))
                dpg.add_spacer(width=45)
                dpg.add_input_int(
                    default_value=0,
                    width=100,
                    tag="new_mobile_alignment",
                    min_value=-1000,
                    max_value=1000,
                    min_clamped=True,
                    max_clamped=True
                )
                dpg.add_text("(-1000 = evil, 0 = neutral, 1000 = good)", color=(120, 120, 120))

            dpg.add_spacer(height=20)

            # Buttons
            with dpg.group(horizontal=True):
                dpg.add_button(
                    label="Create Mobile",
                    callback=self._on_create_mobile_save,
                    user_data=(next_vnum, room_vnum),
                    width=150
                )
                dpg.add_button(
                    label="Cancel",
                    callback=lambda: dpg.delete_item(modal_tag),
                    width=100
                )

    def _on_create_mobile_save(self, sender, app_data, user_data):
        """Handle save button in create mobile modal."""
        from area_editor.models.mobile import Mobile

        next_vnum, room_vnum = user_data
        modal_tag = "create_mobile_modal"

        # Get values from inputs
        keywords = dpg.get_value("new_mobile_keywords").strip()
        short_desc = dpg.get_value("new_mobile_short_desc").strip()
        long_desc = dpg.get_value("new_mobile_long_desc").strip()
        level = dpg.get_value("new_mobile_level")
        alignment = dpg.get_value("new_mobile_alignment")

        # Validate required fields
        if not keywords:
            if hasattr(self.main_window, 'status_bar'):
                self.main_window.status_bar.set_status(
                    "Keywords are required",
                    color=(255, 100, 100)
                )
            return

        if not short_desc:
            if hasattr(self.main_window, 'status_bar'):
                self.main_window.status_bar.set_status(
                    "Short description is required",
                    color=(255, 100, 100)
                )
            return

        # Create new mobile with defaults
        # Note: In ROM MUDs:
        # - short_description: appears in room listings (e.g., "a town guard")
        # - long_description: shown when you examine/look at the mobile
        new_mobile = Mobile(
            vnum=next_vnum,
            keywords=keywords,
            short_description=short_desc,
            long_description=long_desc if long_desc else "You see nothing special about them.\n",
            description="",  # Additional description text (DESCR field)
            level=level,
            alignment=alignment,
            race="human",
            sex="none",
            act_flags="",
            affected_flags="",
            hit_dice=f"{level}d8+{level*10}",
            mana_dice=f"{level}d10+{level*5}",
            damage_dice=f"{max(1, level//5)}d{max(4, level//3)}+{level//2}",
            ac_pierce=0,
            ac_bash=0,
            ac_slash=0,
            ac_exotic=0,
            hitroll=level,
            gold=level * 10,
            start_position="stand",
            default_position="stand",
            size="medium",
            material="flesh"
        )

        # Add to area
        app_state.current_area.mobiles[next_vnum] = new_mobile

        # Mark as modified
        app_state.mark_modified()

        # Close modal
        dpg.delete_item(modal_tag)

        # Refresh area tree to show new mobile
        if hasattr(self.main_window, 'area_tree'):
            self.main_window.area_tree.refresh()

        # Show the mobile editor
        self.show_mobile_editor(next_vnum)

        # If we came from a room, offer to add it to that room
        if room_vnum is not None:
            self._show_add_mobile_to_room_prompt(next_vnum, room_vnum)

        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_status(
                f"Created mobile #{next_vnum}: {short_desc}",
                color=(100, 255, 100)
            )

    def _show_add_mobile_to_room_prompt(self, mob_vnum, room_vnum):
        """Ask if user wants to add the newly created mobile to the room."""
        modal_tag = "add_mobile_to_room_prompt"
        mob = app_state.current_area.mobiles.get(mob_vnum)
        room = app_state.current_area.rooms.get(room_vnum)

        if not mob or not room:
            return

        # Delete existing modal if it exists
        if dpg.does_item_exist(modal_tag):
            dpg.delete_item(modal_tag)

        # Create window using add_window instead of context manager to avoid container stack issues
        window_id = dpg.add_window(
            label="Add to Room?",
            modal=True,
            show=True,
            width=450,
            height=180,
            pos=[400, 250],
            tag=modal_tag
        )

        dpg.add_text(f"Mobile created: {mob.short_description}", color=(100, 255, 100), parent=window_id)
        dpg.add_spacer(height=10, parent=window_id)
        dpg.add_text(f"Add this mobile to room #{room_vnum}: {room.name}?", color=(150, 150, 150), parent=window_id)
        dpg.add_spacer(height=20, parent=window_id)

        button_group = dpg.add_group(horizontal=True, parent=window_id)
        dpg.add_button(
            label="Yes, Add to Room",
            callback=lambda: self._on_add_new_mobile_to_room(modal_tag, mob_vnum, room_vnum),
            width=150,
            parent=button_group
        )
        dpg.add_button(
            label="No, Thanks",
            callback=lambda: dpg.delete_item(modal_tag),
            width=100,
            parent=button_group
        )

    def _on_add_new_mobile_to_room(self, modal_tag, mob_vnum, room_vnum):
        """Add the newly created mobile to the room."""
        from area_editor.models.reset import Reset

        # Create mobile reset
        new_reset = Reset(
            command='M',
            arg1=mob_vnum,
            arg2=1,  # max in world
            arg3=room_vnum,
            arg4=1   # max in room
        )

        # Add to resets
        app_state.current_area.resets.append(new_reset)
        app_state.mark_modified()

        # Close modal
        dpg.delete_item(modal_tag)

        # Show room editor with mobiles tab
        self.show_room_editor(room_vnum)

        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_status(
                f"Added mobile to room #{room_vnum}",
                color=(100, 255, 100)
            )

    def _show_add_mobile_modal(self, room_vnum):
        """Show a modal window for selecting which mobile to add to the room."""
        from area_editor.models.reset import Reset

        # Create modal window
        modal_tag = f"add_mobile_modal_{room_vnum}"
        with dpg.window(
            label=f"Add Mobile to Room #{room_vnum}",
            modal=True,
            show=True,
            width=500,
            height=300,
            pos=[300, 200],
            tag=modal_tag
        ):
            dpg.add_text("Select Mobile to Add", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=10)

            # Mobile selector
            with dpg.group(horizontal=True):
                dpg.add_text("Mobile:", color=(150, 150, 150))
                dpg.add_spacer(width=40)

                # Create list of available mobiles
                mobile_items = [f"#{vnum}: {m.short_description}" for vnum, m in sorted(app_state.current_area.mobiles.items())]
                mobile_vnums = [vnum for vnum in sorted(app_state.current_area.mobiles.keys())]

                dpg.add_combo(
                    items=mobile_items,
                    default_value=mobile_items[0] if mobile_items else "",
                    width=300,
                    tag=f"add_mobile_combo_{room_vnum}"
                )

            dpg.add_spacer(height=10)

            # Max in world
            with dpg.group(horizontal=True):
                dpg.add_text("Max in World:", color=(150, 150, 150))
                dpg.add_spacer(width=10)
                dpg.add_input_int(
                    default_value=1,
                    width=100,
                    tag=f"add_mobile_max_world_{room_vnum}",
                    min_value=1,
                    min_clamped=True
                )
                dpg.add_text("(Maximum instances in entire game)", color=(120, 120, 120))

            # Max in room
            with dpg.group(horizontal=True):
                dpg.add_text("Max in Room:", color=(150, 150, 150))
                dpg.add_spacer(width=15)
                dpg.add_input_int(
                    default_value=1,
                    width=100,
                    tag=f"add_mobile_max_room_{room_vnum}",
                    min_value=1,
                    min_clamped=True
                )
                dpg.add_text("(Maximum instances in this room)", color=(120, 120, 120))

            dpg.add_spacer(height=20)

            # Buttons
            with dpg.group(horizontal=True):
                dpg.add_button(
                    label="Add Mobile",
                    callback=self._on_add_mobile_modal_save,
                    user_data=(room_vnum, mobile_vnums),
                    width=120
                )
                dpg.add_button(
                    label="Cancel",
                    callback=lambda: dpg.delete_item(modal_tag),
                    width=100
                )

    def _on_add_mobile_modal_save(self, sender, app_data, user_data):
        """Handle save button in add mobile modal."""
        from area_editor.models.reset import Reset

        room_vnum, mobile_vnums = user_data
        modal_tag = f"add_mobile_modal_{room_vnum}"

        # Get selected mobile from combo
        combo_value = dpg.get_value(f"add_mobile_combo_{room_vnum}")

        # Extract vnum from combo value (format: "#1050: the innkeeper")
        selected_index = 0
        mobile_items = [f"#{vnum}: {app_state.current_area.mobiles[vnum].short_description}"
                       for vnum in mobile_vnums]

        for idx, item in enumerate(mobile_items):
            if item == combo_value:
                selected_index = idx
                break

        selected_vnum = mobile_vnums[selected_index]

        # Get max values
        max_world = dpg.get_value(f"add_mobile_max_world_{room_vnum}")
        max_room = dpg.get_value(f"add_mobile_max_room_{room_vnum}")

        # Create new reset
        new_reset = Reset(
            command='M',
            arg1=selected_vnum,  # mobile vnum
            arg2=max_world,      # max in world
            arg3=room_vnum,      # room vnum
            arg4=max_room        # max in room
        )

        # Add to resets list
        app_state.current_area.resets.append(new_reset)

        # Mark as modified
        app_state.mark_modified()

        # Close modal
        dpg.delete_item(modal_tag)

        # Refresh the editor
        self.show_room_editor(room_vnum)

        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_status(
                f"Added mobile #{selected_vnum} to room #{room_vnum}",
                color=(100, 255, 100)
            )

    def _show_shop_editor_modal(self, shop, mob_vnum):
        """Show a modal window for editing shop details."""
        # Create modal window
        with dpg.window(
            label=f"Edit Shop for Mobile #{mob_vnum}",
            modal=True,
            show=True,
            width=500,
            height=400,
            pos=[300, 200],
            tag=f"shop_editor_modal_{mob_vnum}"
        ):
            dpg.add_text("Shop Settings", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=10)

            # Profit buy
            with dpg.group(horizontal=True):
                dpg.add_text("Buy Profit %:", color=(150, 150, 150))
                dpg.add_spacer(width=20)
                dpg.add_input_int(
                    default_value=shop.profit_buy,
                    width=100,
                    tag=f"shop_profit_buy_{mob_vnum}",
                    min_value=1,
                    min_clamped=True
                )
                dpg.add_text("(Shop pays this % when buying from players)", color=(120, 120, 120))

            # Profit sell
            with dpg.group(horizontal=True):
                dpg.add_text("Sell Profit %:", color=(150, 150, 150))
                dpg.add_spacer(width=20)
                dpg.add_input_int(
                    default_value=shop.profit_sell,
                    width=100,
                    tag=f"shop_profit_sell_{mob_vnum}",
                    min_value=1,
                    min_clamped=True
                )
                dpg.add_text("(Shop charges this % when selling to players)", color=(120, 120, 120))

            # Open hour
            with dpg.group(horizontal=True):
                dpg.add_text("Open Hour:", color=(150, 150, 150))
                dpg.add_spacer(width=35)
                dpg.add_input_int(
                    default_value=shop.open_hour,
                    width=100,
                    tag=f"shop_open_hour_{mob_vnum}",
                    min_value=0,
                    max_value=23,
                    min_clamped=True,
                    max_clamped=True
                )
                dpg.add_text("(0-23)", color=(120, 120, 120))

            # Close hour
            with dpg.group(horizontal=True):
                dpg.add_text("Close Hour:", color=(150, 150, 150))
                dpg.add_spacer(width=30)
                dpg.add_input_int(
                    default_value=shop.close_hour,
                    width=100,
                    tag=f"shop_close_hour_{mob_vnum}",
                    min_value=0,
                    max_value=23,
                    min_clamped=True,
                    max_clamped=True
                )
                dpg.add_text("(0-23)", color=(120, 120, 120))

            dpg.add_spacer(height=10)
            dpg.add_text("Item Types to Buy (0 = none):", color=(150, 150, 150))
            dpg.add_text("(Item type numbers - see ROM documentation)", color=(120, 120, 120))

            # Buy types (5 slots)
            for i in range(5):
                with dpg.group(horizontal=True):
                    dpg.add_text(f"Type {i+1}:", color=(150, 150, 150))
                    dpg.add_spacer(width=20)
                    dpg.add_input_int(
                        default_value=shop.buy_types[i] if i < len(shop.buy_types) else 0,
                        width=100,
                        tag=f"shop_buy_type_{mob_vnum}_{i}",
                        min_value=0,
                        min_clamped=True
                    )

            dpg.add_spacer(height=20)

            # Buttons
            with dpg.group(horizontal=True):
                dpg.add_button(
                    label="Save",
                    callback=self._on_shop_editor_save,
                    user_data=(shop, mob_vnum),
                    width=100
                )
                dpg.add_button(
                    label="Cancel",
                    callback=lambda: dpg.delete_item(f"shop_editor_modal_{mob_vnum}"),
                    width=100
                )

    def _on_shop_editor_save(self, sender, app_data, user_data):
        """Save shop editor changes."""
        shop, mob_vnum = user_data

        # Update shop values from inputs
        shop.profit_buy = dpg.get_value(f"shop_profit_buy_{mob_vnum}")
        shop.profit_sell = dpg.get_value(f"shop_profit_sell_{mob_vnum}")
        shop.open_hour = dpg.get_value(f"shop_open_hour_{mob_vnum}")
        shop.close_hour = dpg.get_value(f"shop_close_hour_{mob_vnum}")

        # Update buy types
        shop.buy_types = []
        for i in range(5):
            shop.buy_types.append(dpg.get_value(f"shop_buy_type_{mob_vnum}_{i}"))

        # Mark as modified
        app_state.mark_modified()

        # Close modal
        dpg.delete_item(f"shop_editor_modal_{mob_vnum}")

        # Refresh the editor
        if self.current_vnum:
            self.show_room_editor(self.current_vnum)

        if hasattr(self.main_window, 'status_bar'):
            self.main_window.status_bar.set_status(
                f"Updated shop for mobile #{mob_vnum}",
                color=(100, 255, 100)
            )

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
            # Condition can be 'P' (perfect) or a number (0-100)
            if app_data == 'P' or app_data == 'p':
                obj.condition = 'P'
            else:
                try:
                    obj.condition = int(app_data)
                except ValueError:
                    # Invalid input, keep current value
                    pass
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
        if field_name == 'keywords':
            mob.keywords = app_data
        elif field_name == 'short_description':
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

                # Mobiles Tab
                with dpg.tab(label="Mobiles"):
                    dpg.add_spacer(height=10)
                    self._show_mobiles_editor(room, room_vnum)

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
        dpg.add_text("Direction Descriptions & Exits:", color=(150, 150, 150))
        dpg.add_text("(All 6 directions always have descriptions. Set 'To Room' to 0 for description-only, or a room vnum for an actual exit)", color=(120, 120, 120))
        dpg.add_spacer(height=5)

        direction_names = ['north', 'east', 'south', 'west', 'up', 'down']

        # Display ALL 6 directions with edit controls
        for direction in range(6):
            # Get existing exit or use default values
            exit_obj = room.exits.get(direction)
            dir_name = direction_names[direction]

            # If no exit exists, use default values for display
            if exit_obj is None:
                description = "You see nothing special here."
                to_room = 0
                keywords = ""
                locks = 0
                key_vnum = 0
            else:
                description = exit_obj.description
                to_room = exit_obj.to_room
                keywords = exit_obj.keywords
                locks = exit_obj.locks
                key_vnum = exit_obj.key_vnum

            # Determine if this is an actual exit (to_room > 0) or just a description
            is_actual_exit = to_room > 0

            with dpg.child_window(height=200, border=True):
                # Header with direction
                with dpg.group(horizontal=True):
                    if is_actual_exit:
                        dpg.add_text(f"{dir_name.upper()}", color=(200, 200, 100))
                        dpg.add_text(f"(Exit to room #{to_room})", color=(150, 150, 150))
                    else:
                        dpg.add_text(f"{dir_name.upper()}", color=(150, 150, 150))
                        dpg.add_text("(description only)", color=(100, 100, 100))

                dpg.add_separator()
                dpg.add_spacer(height=5)

                # Description field (always shown)
                dpg.add_text("Description:", color=(150, 150, 150))
                dpg.add_input_text(
                    default_value=description,
                    width=-1,
                    callback=self._on_exit_field_changed,
                    user_data=('description', room_vnum, direction)
                )

                dpg.add_spacer(height=5)

                # To Room field with navigation button
                with dpg.group(horizontal=True):
                    dpg.add_text("To Room:", color=(150, 150, 150))
                    dpg.add_text("(0 = description only)", color=(100, 100, 100))
                    dpg.add_input_int(
                        default_value=to_room,
                        width=100,
                        callback=self._on_exit_field_changed,
                        user_data=('to_room', room_vnum, direction)
                    )
                    if to_room > 0:
                        dpg.add_button(
                            label=f"Go to #{to_room}",
                            callback=self._on_room_link_clicked,
                            user_data=to_room,
                            width=100,
                            height=20
                        )

                # Keywords field
                with dpg.group(horizontal=True):
                    dpg.add_text("Keywords:", color=(150, 150, 150))
                    dpg.add_input_text(
                        default_value=keywords,
                        width=300,
                        callback=self._on_exit_field_changed,
                        user_data=('keywords', room_vnum, direction)
                    )

                dpg.add_spacer(height=5)

                # Door Configuration Preset dropdown
                dpg.add_text("Door Configuration:", color=(150, 150, 150))
                door_preset_items = [
                    "No Door (Passageway)",
                    "Open Door",
                    "Closed Door",
                    "Locked Door",
                    "Pickproof Locked Door",
                    "Hidden Passage",
                    "Hidden Door",
                    "Hidden Closed Door",
                    "Hidden Locked Door",
                    "Hidden Pickproof Locked Door",
                ]

                # Determine which preset matches the current flags
                door_presets_map = {
                    0: "No Door (Passageway)",
                    1: "Open Door",
                    3: "Closed Door",
                    7: "Locked Door",
                    15: "Pickproof Locked Door",
                    2048: "Hidden Passage",
                    2049: "Hidden Door",
                    2051: "Hidden Closed Door",
                    2055: "Hidden Locked Door",
                    2063: "Hidden Pickproof Locked Door",
                }
                current_preset = door_presets_map.get(locks, "No Door (Passageway)")

                dpg.add_combo(
                    items=door_preset_items,
                    default_value=current_preset,
                    width=300,
                    callback=self._on_door_preset_changed,
                    user_data=(room_vnum, direction)
                )

                dpg.add_spacer(height=5)

                # Lock Flags field (manual override)
                with dpg.group(horizontal=True):
                    dpg.add_text("Lock Flags (manual):", color=(150, 150, 150))
                    dpg.add_input_int(
                        default_value=locks,
                        width=100,
                        callback=self._on_exit_field_changed,
                        user_data=('locks', room_vnum, direction)
                    )
                    # Show which flags are currently set
                    from area_editor.constants import EXIT_FLAGS
                    active_flags = []
                    for flag_name, flag_value, _ in EXIT_FLAGS:
                        if locks & flag_value:
                            active_flags.append(flag_name)
                    if active_flags:
                        dpg.add_text(f"({', '.join(active_flags)})", color=(100, 200, 100))
                    else:
                        dpg.add_text("(no flags)", color=(100, 100, 100))

                dpg.add_spacer(height=5)

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

    def _show_mobiles_editor(self, room, room_vnum):
        """Show the mobiles editor section for managing mobile resets and shops."""
        from area_editor.models.reset import Reset
        from area_editor.models.shop import Shop

        # Get all mobile resets for this room
        mobile_resets = []
        for i, reset in enumerate(app_state.current_area.resets):
            if reset.command == 'M' and reset.arg3 == room_vnum:
                mobile_resets.append((i, reset))

        dpg.add_text(f"Mobiles in Room ({len(mobile_resets)}):", color=(150, 150, 150))
        dpg.add_text("(Mobiles that spawn in this room)", color=(120, 120, 120))
        dpg.add_spacer(height=5)

        if mobile_resets:
            for reset_index, reset in mobile_resets:
                mob_vnum = reset.arg1
                mob = app_state.current_area.mobiles.get(mob_vnum)

                with dpg.child_window(height=200, border=True):
                    # Header with mobile info and delete button
                    with dpg.group(horizontal=True):
                        if mob:
                            dpg.add_text(f"Mobile #{mob_vnum}: {mob.short_description}", color=(200, 200, 100))
                        else:
                            dpg.add_text(f"Mobile #{mob_vnum} (NOT FOUND)", color=(255, 100, 100))
                        dpg.add_spacer(width=10)
                        dpg.add_button(
                            label="Delete",
                            callback=self._on_delete_mobile_reset_clicked,
                            user_data=(room_vnum, reset_index),
                            width=80,
                            height=20
                        )

                    dpg.add_separator()
                    dpg.add_spacer(height=5)

                    # Mobile vnum selector
                    with dpg.group(horizontal=True):
                        dpg.add_text("Mobile:", color=(150, 150, 150))
                        dpg.add_spacer(width=40)

                        # Create list of available mobiles
                        mobile_items = [f"#{vnum}: {m.short_description}" for vnum, m in sorted(app_state.current_area.mobiles.items())]
                        if mobile_items:
                            current_index = 0
                            for idx, (vnum, m) in enumerate(sorted(app_state.current_area.mobiles.items())):
                                if vnum == mob_vnum:
                                    current_index = idx
                                    break

                            dpg.add_combo(
                                items=mobile_items,
                                default_value=mobile_items[current_index] if current_index < len(mobile_items) else mobile_items[0],
                                width=300,
                                callback=self._on_mobile_reset_field_changed,
                                user_data=('mobile_vnum', reset_index)
                            )
                        else:
                            dpg.add_text("No mobiles defined in area", color=(255, 100, 100))

                    # Max in world
                    with dpg.group(horizontal=True):
                        dpg.add_text("Max in World:", color=(150, 150, 150))
                        dpg.add_spacer(width=10)
                        dpg.add_input_int(
                            default_value=reset.arg2,
                            width=100,
                            callback=self._on_mobile_reset_field_changed,
                            user_data=('max_world', reset_index),
                            min_value=1,
                            min_clamped=True
                        )
                        dpg.add_text("(Maximum instances in entire game)", color=(120, 120, 120))

                    # Max in room
                    with dpg.group(horizontal=True):
                        dpg.add_text("Max in Room:", color=(150, 150, 150))
                        dpg.add_spacer(width=15)
                        dpg.add_input_int(
                            default_value=reset.arg4,
                            width=100,
                            callback=self._on_mobile_reset_field_changed,
                            user_data=('max_room', reset_index),
                            min_value=1,
                            min_clamped=True
                        )
                        dpg.add_text("(Maximum instances in this room)", color=(120, 120, 120))

                    dpg.add_spacer(height=5)

                    # Shop assignment
                    if mob:
                        # Check if this mobile has a shop
                        shop = None
                        for s in app_state.current_area.shops:
                            if s.keeper == mob_vnum:
                                shop = s
                                break

                        with dpg.group(horizontal=True):
                            dpg.add_text("Shop:", color=(150, 150, 150))
                            dpg.add_spacer(width=55)
                            if shop:
                                dpg.add_text("✓ Has Shop", color=(100, 255, 100))
                                dpg.add_button(
                                    label="Edit Shop",
                                    callback=self._on_edit_shop_clicked,
                                    user_data=mob_vnum,
                                    width=100,
                                    height=20
                                )
                                dpg.add_button(
                                    label="Remove Shop",
                                    callback=self._on_remove_shop_clicked,
                                    user_data=mob_vnum,
                                    width=100,
                                    height=20
                                )
                            else:
                                dpg.add_text("No Shop", color=(150, 150, 150))
                                dpg.add_button(
                                    label="Add Shop",
                                    callback=self._on_add_shop_clicked,
                                    user_data=mob_vnum,
                                    width=100,
                                    height=20
                                )

                dpg.add_spacer(height=5)
        else:
            # No mobiles in room yet
            dpg.add_text("No mobiles in this room yet.", color=(120, 120, 120))
            dpg.add_spacer(height=10)

        # Buttons
        with dpg.group(horizontal=True):
            dpg.add_button(
                label="+ Add Mobile to Room",
                callback=self._on_add_mobile_reset_clicked,
                user_data=room_vnum,
                width=200,
                height=25
            )
            dpg.add_button(
                label="Create New Mobile",
                callback=lambda: self._show_create_mobile_modal(room_vnum),
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

                # Import item types list
                from area_editor.constants import ITEM_TYPES

                # Make sure current item type is in the list
                current_item_type = obj.item_type if obj.item_type else "treasure"
                if current_item_type not in ITEM_TYPES:
                    # If not in list, add it (for backwards compatibility)
                    item_types_list = list(ITEM_TYPES) + [current_item_type]
                else:
                    item_types_list = list(ITEM_TYPES)

                dpg.add_combo(
                    items=item_types_list,
                    default_value=current_item_type,
                    width=200,
                    callback=self._on_object_field_changed,
                    user_data=('item_type', obj_vnum)
                )

            # Material
            with dpg.group(horizontal=True):
                dpg.add_text("Material:", color=(150, 150, 150))

                # Import materials list
                from area_editor.parsers.materials_parser import get_materials
                materials = get_materials()

                # Make sure current material is in the list
                current_material = obj.material if obj.material else "iron"
                if current_material not in materials:
                    materials.append(current_material)

                dpg.add_combo(
                    items=sorted(materials),
                    default_value=current_material,
                    width=200,
                    callback=self._on_object_field_changed,
                    user_data=('material', obj_vnum)
                )
                dpg.add_text(f"({len(materials)} materials available)", color=(100, 100, 100))

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
                # Condition can be 'P' (perfect) or a number (0-100)
                condition_str = str(obj.condition) if obj.condition is not None else "100"
                dpg.add_input_text(
                    default_value=condition_str,
                    width=100,
                    callback=self._on_object_field_changed,
                    user_data=('condition', obj_vnum)
                )
                dpg.add_text("(0-100 or 'P' for perfect)", color=(100, 100, 100))

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

            # Keywords (NAME field in ROM format)
            dpg.add_text("Keywords:", color=(150, 150, 150))
            dpg.add_text("(Space-separated words for targeting, e.g., 'guard town soldier')", color=(120, 120, 120))
            dpg.add_input_text(
                default_value=mob.keywords,
                width=-1,
                callback=self._on_mobile_field_changed,
                user_data=('keywords', mob_vnum)
            )
            dpg.add_spacer(height=10)

            # Short Description (SHORT field in ROM format)
            dpg.add_text("Short Description:", color=(150, 150, 150))
            dpg.add_text("(What appears in the room, e.g., 'a town guard')", color=(120, 120, 120))
            dpg.add_input_text(
                default_value=mob.short_description,
                width=-1,
                callback=self._on_mobile_field_changed,
                user_data=('short_description', mob_vnum)
            )
            dpg.add_spacer(height=10)

            # Long Description (LONG field in ROM format)
            dpg.add_text("Long Description:", color=(150, 150, 150))
            dpg.add_text("(What you see when you 'look' at the mobile - auto-wraps at 80 characters)", color=(120, 120, 120))
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

            # Description (DESCR field in ROM format)
            dpg.add_text("Description (DESCR):", color=(150, 150, 150))
            dpg.add_text("(Additional description text - rarely used)", color=(120, 120, 120))
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

                # Import race types list
                from area_editor.constants import RACE_TYPES

                # Make sure current race is in the list
                current_race = mob.race if mob.race else "human"
                if current_race not in RACE_TYPES:
                    # If not in list, add it (for backwards compatibility)
                    race_types_list = list(RACE_TYPES) + [current_race]
                else:
                    race_types_list = list(RACE_TYPES)

                dpg.add_combo(
                    items=race_types_list,
                    default_value=current_race,
                    width=200,
                    callback=lambda s, a, u: self._on_mobile_field_changed(s, a, u),
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
                # mob.sex is a string, not an index
                sex_value = mob.sex if mob.sex in sex_items else "none"
                dpg.add_combo(
                    items=sex_items,
                    default_value=sex_value,
                    width=150,
                    callback=lambda s, a, u: self._on_mobile_field_changed(s, a, u),
                    user_data=('sex', mob_vnum)
                )

            # Size
            with dpg.group(horizontal=True):
                dpg.add_text("Size:", color=(150, 150, 150))
                size_items = ["tiny", "small", "medium", "large", "huge", "giant"]
                # mob.size is a string, not an index
                size_value = mob.size if mob.size in size_items else "medium"
                dpg.add_combo(
                    items=size_items,
                    default_value=size_value,
                    width=150,
                    callback=lambda s, a, u: self._on_mobile_field_changed(s, a, u),
                    user_data=('size', mob_vnum)
                )

            # Material
            with dpg.group(horizontal=True):
                dpg.add_text("Material:", color=(150, 150, 150))
                material_items = [
                    "flesh", "bone", "stone", "wood", "iron", "steel", "bronze",
                    "copper", "silver", "gold", "platinum", "mithril", "adamantite",
                    "cloth", "leather", "hide", "fur", "silk", "wool",
                    "glass", "crystal", "diamond", "ruby", "emerald", "sapphire",
                    "water", "ice", "fire", "air", "earth", "energy",
                    "paper", "parchment", "vellum", "food", "organic",
                    "plastic", "rubber", "ceramic", "porcelain",
                    "ethereal", "shadow", "light", "darkness", "void"
                ]
                # mob.material is a string
                material_value = mob.material if mob.material in material_items else (mob.material if mob.material else "flesh")
                dpg.add_combo(
                    items=material_items,
                    default_value=material_value,
                    width=200,
                    callback=lambda s, a, u: self._on_mobile_field_changed(s, a, u),
                    user_data=('material', mob_vnum)
                )

            dpg.add_spacer(height=10)

            # Position Section
            dpg.add_text("Positions", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=5)

            position_items = ["dead", "mortal", "incap", "stunned", "sleeping", "resting", "sitting", "fighting", "standing"]

            # Start Position
            with dpg.group(horizontal=True):
                dpg.add_text("Start Position:", color=(150, 150, 150))
                # mob.start_position is a string, not an index
                start_pos_value = mob.start_position if mob.start_position in position_items else "standing"
                dpg.add_combo(
                    items=position_items,
                    default_value=start_pos_value,
                    width=150,
                    callback=lambda s, a, u: self._on_mobile_field_changed(s, a, u),
                    user_data=('start_position', mob_vnum)
                )

            # Default Position
            with dpg.group(horizontal=True):
                dpg.add_text("Default Position:", color=(150, 150, 150))
                # mob.default_position is a string, not an index
                default_pos_value = mob.default_position if mob.default_position in position_items else "standing"
                dpg.add_combo(
                    items=position_items,
                    default_value=default_pos_value,
                    width=150,
                    callback=lambda s, a, u: self._on_mobile_field_changed(s, a, u),
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

