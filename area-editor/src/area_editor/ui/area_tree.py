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
        self.error_theme_id = None  # Theme for items with errors
    
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

        # Update editor panel (properties are now embedded in editor)
        if item_type == 'room':
            self.main_window.editor_panel.show_room_editor(vnum)
        elif item_type == 'object':
            self.main_window.editor_panel.show_object_editor(vnum)
        elif item_type == 'mobile':
            self.main_window.editor_panel.show_mobile_editor(vnum)
        elif item_type == 'area':
            self.main_window.editor_panel.show_area_editor()

    def _get_error_theme(self):
        """Get or create a theme for items with validation errors."""
        if self.error_theme_id is None or not dpg.does_item_exist(self.error_theme_id):
            with dpg.theme() as self.error_theme_id:
                with dpg.theme_component(dpg.mvAll):
                    # Red text color for errors
                    dpg.add_theme_color(dpg.mvThemeCol_Text, (255, 100, 100), category=dpg.mvThemeCat_Core)
        return self.error_theme_id

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

        # If area already exists, save its state before removing
        was_open = None

        # Check if we have a saved state from rebuild_all_areas
        if hasattr(self, '_saved_state'):
            was_open = self._saved_state
        elif area_id in self.area_nodes:
            old_node_id = self.area_nodes[area_id]
            if dpg.does_item_exist(old_node_id):
                # Save current open/closed state (True = open, False = closed)
                was_open = dpg.get_value(old_node_id)
                dpg.delete_item(old_node_id)
            # Remove old selectable items for this area
            keys_to_remove = [k for k in self.selectable_items.keys() if k[0] == area_id]
            for key in keys_to_remove:
                del self.selectable_items[key]

        # Create area node
        # If refreshing, preserve previous state; otherwise collapse by default if multiple areas
        num_areas = len(app_state.areas)
        if was_open is not None:
            # Preserve previous state when refreshing
            default_open = was_open
        else:
            # New area: collapse by default if multiple areas are loaded, expand if only one
            default_open = (num_areas == 1)

        # If we're adding a second area, collapse all existing area nodes
        if num_areas == 2:
            for existing_area_id, existing_node_id in self.area_nodes.items():
                if dpg.does_item_exist(existing_node_id):
                    dpg.set_value(existing_node_id, False)  # Collapse the node

        with dpg.tree_node(
            label=f"{area.name} ({area.min_vnum}-{area.max_vnum})",
            default_open=default_open,
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

            # Rooms section - collapse if multiple areas
            if area.rooms:
                with dpg.tree_node(label=f"Rooms ({len(area.rooms)})", default_open=default_open):
                    for vnum in sorted(area.rooms.keys()):
                        room = area.rooms[vnum]
                        label = f"#{vnum}: {room.name}" if room.name else f"#{vnum}"

                        # Check if room has validation errors
                        has_errors = app_state.has_validation_errors('room', vnum, area_id)

                        # Add warning icon and color if there are errors
                        if has_errors:
                            label = f"⚠ {label}"
                            # Create a group with colored text
                            with dpg.group(horizontal=True):
                                item_id = dpg.add_selectable(
                                    label=label,
                                    callback=self._on_item_clicked,
                                    user_data=(area_id, 'room', vnum)
                                )
                                # Apply red color to indicate error
                                dpg.bind_item_theme(item_id, self._get_error_theme())
                        else:
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

    def refresh(self):
        """Refresh the area tree to reflect current state."""
        # Rebuild the tree for the current area
        if app_state.current_area and app_state.current_area_id:
            self.populate_from_area(app_state.current_area, app_state.current_area_id)

    def rebuild_all_areas(self):
        """Rebuild the entire area tree in sorted order by vnum range."""
        # Save all current open/closed states
        saved_states = {}
        for area_id, node_id in self.area_nodes.items():
            if dpg.does_item_exist(node_id):
                saved_states[area_id] = dpg.get_value(node_id)

        # Clear all area nodes
        for area_id in list(self.area_nodes.keys()):
            node_id = self.area_nodes[area_id]
            if dpg.does_item_exist(node_id):
                dpg.delete_item(node_id)

        self.area_nodes.clear()
        self.selectable_items.clear()

        # Sort areas by min_vnum
        sorted_areas = sorted(
            app_state.areas.items(),
            key=lambda x: x[1].min_vnum
        )

        # Rebuild in sorted order
        for area_id, area in sorted_areas:
            # Temporarily restore saved state for this area
            if area_id in saved_states:
                self._saved_state = saved_states[area_id]
            else:
                self._saved_state = None

            self.populate_from_area(area, area_id)

        # Clean up temporary state
        if hasattr(self, '_saved_state'):
            delattr(self, '_saved_state')

