"""
Editor panel for editing areas, rooms, objects, and mobiles.
"""
import dearpygui.dearpygui as dpg
from area_editor.app_state import app_state


class EditorPanel:
    """Main editor panel that displays different editors based on selection."""

    def __init__(self, main_window):
        """Initialize the editor panel."""
        self.main_window = main_window
        self.current_editor = None
        self.welcome_group_id = None
        self.editor_group_id = None
    
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

            # Show the room editor
            self.show_room_editor(room_vnum)
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

        with dpg.group(parent=self.editor_group_id):
            dpg.add_text(f"Room #{room_vnum}", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=10)

            dpg.add_text("Name:", color=(150, 150, 150))
            dpg.add_input_text(default_value=room.name, width=-1)
            dpg.add_spacer(height=10)

            dpg.add_text("Description:", color=(150, 150, 150))
            dpg.add_input_text(
                default_value=room.description,
                multiline=True,
                height=100,
                width=-1
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
                for edesc in room.extra_descriptions:
                    dpg.add_text(f"  Keywords: {edesc.keywords}", color=(120, 120, 120))

    def show_object_editor(self, obj_vnum):
        """Show the object editor for the specified object."""
        obj = app_state.current_area.objects.get(obj_vnum)
        if not obj:
            return

        self._clear_editor()
        self._show_editor()

        with dpg.group(parent=self.editor_group_id):
            dpg.add_text(f"Object #{obj_vnum}", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=10)

            dpg.add_text("Short Description:", color=(150, 150, 150))
            dpg.add_input_text(default_value=obj.short_description, width=-1)
            dpg.add_spacer(height=10)

            dpg.add_text("Long Description:", color=(150, 150, 150))
            dpg.add_input_text(default_value=obj.long_description, width=-1)
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

    def show_mobile_editor(self, mob_vnum):
        """Show the mobile editor for the specified mobile."""
        mob = app_state.current_area.mobiles.get(mob_vnum)
        if not mob:
            return

        self._clear_editor()
        self._show_editor()

        with dpg.group(parent=self.editor_group_id):
            dpg.add_text(f"Mobile #{mob_vnum}", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=10)

            dpg.add_text("Short Description:", color=(150, 150, 150))
            dpg.add_input_text(default_value=mob.short_description, width=-1)
            dpg.add_spacer(height=10)

            dpg.add_text("Long Description:", color=(150, 150, 150))
            dpg.add_input_text(
                default_value=mob.long_description,
                multiline=True,
                height=60,
                width=-1
            )
            dpg.add_spacer(height=10)

            dpg.add_text(f"Race: {mob.race}", color=(150, 150, 150))
            dpg.add_text(f"Level: {mob.level}", color=(150, 150, 150))
            dpg.add_text(f"Alignment: {mob.alignment}", color=(150, 150, 150))
            dpg.add_text(f"Hit Dice: {mob.hit_dice}", color=(150, 150, 150))
            dpg.add_text(f"Damage Dice: {mob.damage_dice}", color=(150, 150, 150))
            dpg.add_text(f"Hitroll: {mob.hitroll}", color=(150, 150, 150))

    def show_area_editor(self):
        """Show the area editor for the current area."""
        area = app_state.current_area
        if not area:
            return

        self._clear_editor()
        self._show_editor()

        with dpg.group(parent=self.editor_group_id):
            dpg.add_text("Area Information", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=10)

            dpg.add_text("Area Name:", color=(150, 150, 150))
            dpg.add_input_text(default_value=area.name, width=-1)
            dpg.add_spacer(height=10)

            dpg.add_text(f"VNUM Range: {area.min_vnum} - {area.max_vnum}", color=(150, 150, 150))
            dpg.add_text(f"Security: {area.security}", color=(150, 150, 150))
            dpg.add_spacer(height=10)

            dpg.add_text("Statistics:", color=(150, 150, 150))
            dpg.add_text(f"  Rooms: {len(area.rooms)}", color=(120, 120, 120))
            dpg.add_text(f"  Objects: {len(area.objects)}", color=(120, 120, 120))
            dpg.add_text(f"  Mobiles: {len(area.mobiles)}", color=(120, 120, 120))

