"""
Area tree component showing the hierarchy of areas, rooms, objects, and mobiles.
"""
import dearpygui.dearpygui as dpg
from area_editor.models.area import Area
from area_editor.app_state import app_state


class AreaTree:
    """Tree view showing areas and their contents."""

    def __init__(self, main_window):
        """Initialize the area tree."""
        self.main_window = main_window
        self.tree_id = None
        self.area_node_id = None
        self.selectable_items = {}  # Maps (item_type, vnum) -> selectable_id
        self.current_selection = None  # Currently selected (item_type, vnum)
    
    def create(self):
        """Create the area tree view."""
        dpg.add_text("Area Explorer", color=(200, 200, 200))
        dpg.add_separator()
        
        # Search box
        dpg.add_input_text(
            label="Search",
            hint="Search areas, rooms, objects...",
            callback=self._on_search
        )
        
        dpg.add_separator()
        
        # Tree view
        with dpg.tree_node(label="Areas", default_open=True) as self.tree_id:
            # Placeholder content - will be populated when areas are loaded
            with dpg.tree_node(label="No areas loaded", leaf=True):
                pass
    
    def _on_search(self, sender, app_data):
        """Handle search input."""
        print(f"Search: {app_data}")

    def _on_item_clicked(self, sender, app_data, user_data):
        """Handle tree item click."""
        item_type, vnum = user_data

        # Deselect previous item
        if self.current_selection and self.current_selection in self.selectable_items:
            prev_id = self.selectable_items[self.current_selection]
            if dpg.does_item_exist(prev_id):
                dpg.set_value(prev_id, False)

        # Select new item
        self.current_selection = (item_type, vnum)
        app_state.select_item(item_type, vnum)

        # Update editor panel
        if item_type == 'room':
            self.main_window.editor_panel.show_room_editor(vnum)
        elif item_type == 'object':
            self.main_window.editor_panel.show_object_editor(vnum)
        elif item_type == 'mobile':
            self.main_window.editor_panel.show_mobile_editor(vnum)
        elif item_type == 'area':
            self.main_window.editor_panel.show_area_editor()

    def populate_from_area(self, area: Area):
        """Populate the tree with data from an area."""
        # Clear existing tree content and selection tracking
        if self.area_node_id and dpg.does_item_exist(self.area_node_id):
            dpg.delete_item(self.area_node_id)

        self.selectable_items.clear()
        self.current_selection = None

        # Create area node
        with dpg.tree_node(
            label=f"{area.name} ({area.min_vnum}-{area.max_vnum})",
            default_open=True,
            parent=self.tree_id
        ) as self.area_node_id:
            # Add area info as clickable item
            item_id = dpg.add_selectable(
                label=f"Area Info",
                callback=self._on_item_clicked,
                user_data=('area', 0)
            )
            self.selectable_items[('area', 0)] = item_id

            # Rooms section
            if area.rooms:
                with dpg.tree_node(label=f"Rooms ({len(area.rooms)})", default_open=True):
                    for vnum in sorted(area.rooms.keys()):
                        room = area.rooms[vnum]
                        label = f"#{vnum}: {room.name}" if room.name else f"#{vnum}"
                        item_id = dpg.add_selectable(
                            label=label,
                            callback=self._on_item_clicked,
                            user_data=('room', vnum)
                        )
                        self.selectable_items[('room', vnum)] = item_id

            # Objects section
            if area.objects:
                with dpg.tree_node(label=f"Objects ({len(area.objects)})", default_open=False):
                    for vnum in sorted(area.objects.keys()):
                        obj = area.objects[vnum]
                        label = f"#{vnum}: {obj.short_description}"
                        item_id = dpg.add_selectable(
                            label=label,
                            callback=self._on_item_clicked,
                            user_data=('object', vnum)
                        )
                        self.selectable_items[('object', vnum)] = item_id

            # Mobiles section
            if area.mobiles:
                with dpg.tree_node(label=f"Mobiles ({len(area.mobiles)})", default_open=False):
                    for vnum in sorted(area.mobiles.keys()):
                        mob = area.mobiles[vnum]
                        label = f"#{vnum}: {mob.short_description}"
                        item_id = dpg.add_selectable(
                            label=label,
                            callback=self._on_item_clicked,
                            user_data=('mobile', vnum)
                        )
                        self.selectable_items[('mobile', vnum)] = item_id

