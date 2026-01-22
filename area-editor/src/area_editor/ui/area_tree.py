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
        self.area_nodes = {}  # Maps area_id -> tree_node_id
        self.selectable_items = {}  # Maps (area_id, item_type, vnum) -> selectable_id
        self.current_selection = None  # Currently selected (area_id, item_type, vnum)
        self.no_areas_placeholder_id = None
    
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
            # Placeholder content - will be removed when first area is loaded
            self.no_areas_placeholder_id = dpg.add_text(
                "No areas loaded",
                color=(120, 120, 120)
            )
    
    def _on_search(self, sender, app_data):
        """Handle search input."""
        print(f"Search: {app_data}")

    def _on_item_clicked(self, sender, app_data, user_data):
        """Handle tree item click."""
        area_id, item_type, vnum = user_data

        # Deselect previous item
        if self.current_selection and self.current_selection in self.selectable_items:
            prev_id = self.selectable_items[self.current_selection]
            if dpg.does_item_exist(prev_id):
                dpg.set_value(prev_id, False)

        # Select new item
        self.current_selection = (area_id, item_type, vnum)

        # Set the current area if it changed
        if app_state.current_area_id != area_id:
            app_state.set_current_area(area_id)
            # Update status bar
            if hasattr(self.main_window, 'status_bar'):
                self.main_window.status_bar.set_file_info(app_state.get_file_info())

        app_state.select_item(item_type, vnum)

        # Update editor panel and properties panel
        if item_type == 'room':
            room = app_state.current_area.rooms.get(vnum)
            self.main_window.editor_panel.show_room_editor(vnum)
            if hasattr(self.main_window, 'properties_panel') and room:
                self.main_window.properties_panel.show_room_properties(room, vnum)
        elif item_type == 'object':
            obj = app_state.current_area.objects.get(vnum)
            self.main_window.editor_panel.show_object_editor(vnum)
            if hasattr(self.main_window, 'properties_panel') and obj:
                self.main_window.properties_panel.show_object_properties(obj, vnum)
        elif item_type == 'mobile':
            mob = app_state.current_area.mobiles.get(vnum)
            self.main_window.editor_panel.show_mobile_editor(vnum)
            if hasattr(self.main_window, 'properties_panel') and mob:
                self.main_window.properties_panel.show_mobile_properties(mob, vnum)
        elif item_type == 'area':
            self.main_window.editor_panel.show_area_editor()
            if hasattr(self.main_window, 'properties_panel') and app_state.current_area:
                self.main_window.properties_panel.show_area_properties(app_state.current_area)

    def populate_from_area(self, area: Area, area_id: str):
        """Add an area to the tree.

        Args:
            area: The area to add
            area_id: Unique identifier for the area (usually filename)
        """
        # Remove "No areas loaded" placeholder if it exists
        if self.no_areas_placeholder_id and dpg.does_item_exist(self.no_areas_placeholder_id):
            dpg.delete_item(self.no_areas_placeholder_id)
            self.no_areas_placeholder_id = None

        # If area already exists, remove it first
        if area_id in self.area_nodes:
            old_node_id = self.area_nodes[area_id]
            if dpg.does_item_exist(old_node_id):
                dpg.delete_item(old_node_id)
            # Remove old selectable items for this area
            keys_to_remove = [k for k in self.selectable_items.keys() if k[0] == area_id]
            for key in keys_to_remove:
                del self.selectable_items[key]

        # Create area node
        with dpg.tree_node(
            label=f"{area.name} ({area.min_vnum}-{area.max_vnum})",
            default_open=True,
            parent=self.tree_id
        ) as area_node_id:
            self.area_nodes[area_id] = area_node_id

            # Add area info as clickable item
            item_id = dpg.add_selectable(
                label=f"Area Info",
                callback=self._on_item_clicked,
                user_data=(area_id, 'area', 0)
            )
            self.selectable_items[(area_id, 'area', 0)] = item_id

            # Rooms section
            if area.rooms:
                with dpg.tree_node(label=f"Rooms ({len(area.rooms)})", default_open=True):
                    for vnum in sorted(area.rooms.keys()):
                        room = area.rooms[vnum]
                        label = f"#{vnum}: {room.name}" if room.name else f"#{vnum}"
                        item_id = dpg.add_selectable(
                            label=label,
                            callback=self._on_item_clicked,
                            user_data=(area_id, 'room', vnum)
                        )
                        self.selectable_items[(area_id, 'room', vnum)] = item_id

            # Objects section
            if area.objects:
                with dpg.tree_node(label=f"Objects ({len(area.objects)})", default_open=False):
                    for vnum in sorted(area.objects.keys()):
                        obj = area.objects[vnum]
                        label = f"#{vnum}: {obj.short_description}"
                        item_id = dpg.add_selectable(
                            label=label,
                            callback=self._on_item_clicked,
                            user_data=(area_id, 'object', vnum)
                        )
                        self.selectable_items[(area_id, 'object', vnum)] = item_id

            # Mobiles section
            if area.mobiles:
                with dpg.tree_node(label=f"Mobiles ({len(area.mobiles)})", default_open=False):
                    for vnum in sorted(area.mobiles.keys()):
                        mob = area.mobiles[vnum]
                        label = f"#{vnum}: {mob.short_description}"
                        item_id = dpg.add_selectable(
                            label=label,
                            callback=self._on_item_clicked,
                            user_data=(area_id, 'mobile', vnum)
                        )
                        self.selectable_items[(area_id, 'mobile', vnum)] = item_id

