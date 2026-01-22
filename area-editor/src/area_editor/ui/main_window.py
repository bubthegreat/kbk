"""
Main window for the Area Editor application.
"""
import dearpygui.dearpygui as dpg
from pathlib import Path
from area_editor.ui.menu_bar import MenuBar
from area_editor.ui.area_tree import AreaTree
from area_editor.ui.editor_panel import EditorPanel
from area_editor.ui.properties_panel import PropertiesPanel
from area_editor.ui.status_bar import StatusBar
from area_editor.app_state import app_state
from area_editor.parsers.are_parser import AreParser


class MainWindow:
    """Main application window."""

    def __init__(self):
        """Initialize the main window."""
        self.window_id = None
        self.menu_bar = MenuBar(self)
        self.area_tree = AreaTree(self)
        self.editor_panel = EditorPanel(self)
        self.properties_panel = PropertiesPanel()
        self.status_bar = StatusBar()
        self.key_handler = None
    
    def create(self):
        """Create the main window and all its components."""
        with dpg.window(
            label="KBK Area Editor",
            tag="main_window",
            no_close=True,
            no_collapse=True,
            no_move=True,
            no_title_bar=True
        ) as self.window_id:
            # Menu bar
            self.menu_bar.create()

            # Main content area with horizontal split
            with dpg.group(horizontal=True):
                # Left sidebar - Area tree
                with dpg.child_window(
                    width=300,
                    height=-25,
                    border=True,
                    tag="left_sidebar"
                ):
                    self.area_tree.create()

                # Center panel - Editor
                with dpg.child_window(
                    width=-350,
                    height=-25,
                    border=True,
                    tag="center_panel"
                ):
                    self.editor_panel.create()

                # Right sidebar - Properties
                with dpg.child_window(
                    width=350,
                    height=-25,
                    border=True,
                    tag="right_sidebar"
                ):
                    self.properties_panel.create()

            # Status bar at bottom
            with dpg.child_window(
                height=25,
                border=False,
                tag="status_bar_container"
            ):
                self.status_bar.create()

        # Set up keyboard handlers
        self._setup_keyboard_handlers()

    def _setup_keyboard_handlers(self):
        """Set up keyboard handlers for navigation."""
        with dpg.handler_registry():
            dpg.add_key_press_handler(dpg.mvKey_Up, callback=self._on_key_up)
            dpg.add_key_press_handler(dpg.mvKey_Down, callback=self._on_key_down)

    def _on_key_up(self):
        """Handle up arrow key - navigate to previous room."""
        self._navigate_room(-1)

    def _on_key_down(self):
        """Handle down arrow key - navigate to next room."""
        self._navigate_room(1)

    def _navigate_room(self, direction: int):
        """Navigate to the previous or next room in the list.

        Args:
            direction: -1 for previous, 1 for next
        """
        if not app_state.current_area:
            return

        # Get current selection
        item_type = app_state.selected_item_type
        current_vnum = app_state.selected_item_vnum

        # Only navigate if a room is currently selected
        if item_type != 'room' or current_vnum is None:
            return

        # Get sorted list of room vnums
        room_vnums = sorted(app_state.current_area.rooms.keys())

        if not room_vnums:
            return

        # Find current index
        try:
            current_index = room_vnums.index(current_vnum)
        except ValueError:
            # Current room not found, select first room
            current_index = 0

        # Calculate new index
        new_index = current_index + direction

        # Wrap around if needed
        if new_index < 0:
            new_index = len(room_vnums) - 1
        elif new_index >= len(room_vnums):
            new_index = 0

        # Get new room vnum
        new_vnum = room_vnums[new_index]

        # Update selection
        app_state.select_item('room', new_vnum)

        # Update tree selection
        if hasattr(self, 'area_tree'):
            tree = self.area_tree
            # Deselect previous item
            if tree.current_selection and tree.current_selection in tree.selectable_items:
                prev_id = tree.selectable_items[tree.current_selection]
                if dpg.does_item_exist(prev_id):
                    dpg.set_value(prev_id, False)

            # Select new item
            tree.current_selection = ('room', new_vnum)
            if ('room', new_vnum) in tree.selectable_items:
                new_id = tree.selectable_items[('room', new_vnum)]
                if dpg.does_item_exist(new_id):
                    dpg.set_value(new_id, True)

        # Update editor panel
        room = app_state.current_area.rooms.get(new_vnum)
        if room:
            self.editor_panel.show_room_editor(new_vnum)
            self.properties_panel.show_room_properties(room, new_vnum)

    def open_area_file(self, filepath: str):
        """Open and load an area file."""
        try:
            self.status_bar.set_status("Loading area file...", color=(200, 200, 100))

            # Parse the file
            parser = AreParser(filepath)
            area = parser.parse()

            if area is None:
                self.status_bar.set_status("Failed to load area file", color=(255, 100, 100))
                return

            # Load into app state
            app_state.load_area(area, Path(filepath))

            # Update UI
            self.area_tree.populate_from_area(area)
            self.status_bar.set_status("Area loaded successfully", color=(100, 255, 100))
            self.status_bar.set_file_info(app_state.get_file_info())

        except Exception as e:
            self.status_bar.set_status(f"Error loading file: {e}", color=(255, 100, 100))
            print(f"Error loading area file: {e}")
            import traceback
            traceback.print_exc()

