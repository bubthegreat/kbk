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

