"""
Main window for the Area Editor application.
"""
import dearpygui.dearpygui as dpg
from pathlib import Path
from area_editor.ui.menu_bar import MenuBar
from area_editor.ui.area_tree import AreaTree
from area_editor.ui.editor_panel import EditorPanel
from area_editor.ui.status_bar import StatusBar
from area_editor.app_state import app_state
from area_editor.parsers.are_parser import AreParser
from area_editor.config import config
from area_editor.validation import AreaValidator


class MainWindow:
    """Main application window."""

    def __init__(self):
        """Initialize the main window."""
        self.window_id = None
        self.menu_bar = MenuBar(self)
        self.area_tree = AreaTree(self)
        self.editor_panel = EditorPanel(self)
        self.status_bar = StatusBar()
        self.key_handler = None

        # Load panel widths from config
        self.left_sidebar_width = config.get_ui_width("left_sidebar")
        self.right_sidebar_width = config.get_ui_width("right_sidebar")
    
    def create(self):
        """Create the main window and all its components."""
        with dpg.window(
            label="KBK Area Editor",
            tag="main_window",
            no_close=True,
            no_collapse=True,
            no_move=True,
            no_resize=True,
            no_title_bar=True
        ) as self.window_id:
            # Menu bar
            self.menu_bar.create()

            # Main content area with horizontal split
            # Calculate actual widths to ensure they're fixed
            left_width = max(self.left_sidebar_width, 200)
            right_width = max(self.right_sidebar_width, 80)

            with dpg.group(horizontal=True):
                # Left sidebar - Area tree (LOCKED to left, minimum 200px)
                with dpg.child_window(
                    width=left_width,
                    height=-25,
                    border=True,
                    tag="left_sidebar"
                ):
                    self.area_tree.create()

                # Center panel - Editor (fills remaining space)
                # Using negative width to fill remaining space after accounting for right sidebar
                with dpg.child_window(
                    width=-right_width,
                    height=-25,
                    border=True,
                    tag="center_panel"
                ):
                    self.editor_panel.create()

                # Right sidebar - MUD Terminal (LOCKED to right, minimum 80px)
                with dpg.child_window(
                    width=right_width,
                    height=-25,
                    border=True,
                    tag="right_sidebar"
                ):
                    # MUD Terminal will be created here by editor_panel when needed
                    dpg.add_text("MUD Terminal", color=(200, 200, 200))
                    dpg.add_separator()
                    with dpg.group(tag="terminal_container"):
                        dpg.add_text(
                            "Select a room and click 'Open Terminal' to start",
                            color=(120, 120, 120)
                        )

            # Status bar at bottom
            with dpg.child_window(
                height=25,
                border=False,
                tag="status_bar_container"
            ):
                self.status_bar.create()

        # Set up keyboard handlers
        self._setup_keyboard_handlers()

        # Set up resize callback to save panel sizes
        self._setup_resize_callback()

    def _setup_keyboard_handlers(self):
        """Set up keyboard handlers for navigation."""
        with dpg.handler_registry():
            dpg.add_key_press_handler(dpg.mvKey_Up, callback=self._on_key_up)
            dpg.add_key_press_handler(dpg.mvKey_Down, callback=self._on_key_down)
            # Ctrl+S for save
            dpg.add_key_press_handler(dpg.mvKey_S, callback=self._on_key_save)

    def _setup_resize_callback(self):
        """Set up callback to save panel sizes when they change."""
        # We don't need resize callbacks anymore since panels are fixed
        # The layout is: [fixed left] [flexible center] [fixed right]
        # DearPyGui handles this automatically with negative width on center
        pass

    def _on_key_up(self):
        """Handle up arrow key - navigate to previous room."""
        self._navigate_room(-1)

    def _on_key_down(self):
        """Handle down arrow key - navigate to next room."""
        self._navigate_room(1)

    def _on_key_save(self):
        """Handle Ctrl+S key - save current area."""
        # Check if Ctrl is pressed
        if dpg.is_key_down(dpg.mvKey_LControl) or dpg.is_key_down(dpg.mvKey_RControl):
            self.editor_panel._on_save_shortcut()

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
        if hasattr(self, 'area_tree') and app_state.current_area_id:
            tree = self.area_tree
            area_id = app_state.current_area_id

            # Deselect previous item
            if tree.current_selection and tree.current_selection in tree.selectable_items:
                prev_id = tree.selectable_items[tree.current_selection]
                if dpg.does_item_exist(prev_id):
                    dpg.set_value(prev_id, False)

            # Select new item
            tree.current_selection = (area_id, 'room', new_vnum)
            if (area_id, 'room', new_vnum) in tree.selectable_items:
                new_id = tree.selectable_items[(area_id, 'room', new_vnum)]
                if dpg.does_item_exist(new_id):
                    dpg.set_value(new_id, True)

        # Update editor panel (properties are now embedded in editor)
        room = app_state.current_area.rooms.get(new_vnum)
        if room:
            self.editor_panel.show_room_editor(new_vnum)

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
            filepath_obj = Path(filepath)
            app_state.load_area(area, filepath_obj)
            area_id = filepath_obj.name  # Use filename as area ID

            # Run validation
            validator = AreaValidator(area, area_id)
            validation_result = validator.validate()
            app_state.set_validation_result(area_id, validation_result)

            # Log validation results
            if validation_result.has_errors():
                print(f"Validation found {len(validation_result.errors)} error(s) in {area_id}")
                for error in validation_result.errors[:5]:  # Show first 5 errors
                    print(f"  {error}")
            if validation_result.has_warnings():
                print(f"Validation found {len(validation_result.warnings)} warning(s) in {area_id}")

            # Update UI
            self.area_tree.populate_from_area(area, area_id)

            # Update status bar with validation info
            status_msg = "Area loaded successfully"
            if validation_result.has_errors():
                status_msg += f" ({len(validation_result.errors)} errors, {len(validation_result.warnings)} warnings)"
                status_color = (255, 200, 100)  # Orange for warnings/errors
            else:
                status_color = (100, 255, 100)  # Green for success

            self.status_bar.set_status(status_msg, color=status_color)
            self.status_bar.set_file_info(app_state.get_file_info())

        except Exception as e:
            self.status_bar.set_status(f"Error loading file: {e}", color=(255, 100, 100))
            print(f"Error loading area file: {e}")
            import traceback
            traceback.print_exc()

