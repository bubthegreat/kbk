"""
Menu bar component for the Area Editor.
"""
import dearpygui.dearpygui as dpg
from pathlib import Path


class MenuBar:
    """Menu bar with File, Edit, View, Tools, and Help menus."""

    def __init__(self, main_window):
        """Initialize the menu bar."""
        self.main_window = main_window
        self.file_dialog_id = None
    
    def create(self):
        """Create the menu bar."""
        # Create file dialog (hidden by default)
        with dpg.file_dialog(
            directory_selector=False,
            show=False,
            callback=self._file_dialog_callback,
            file_count=1,
            width=700,
            height=400,
            tag="file_dialog"
        ):
            dpg.add_file_extension(".are", color=(150, 255, 150, 255))
            dpg.add_file_extension(".*")

        with dpg.menu_bar():
            # File menu
            with dpg.menu(label="File"):
                dpg.add_menu_item(label="New Area", callback=self._on_new_area)
                dpg.add_menu_item(label="Open Area...", callback=self._on_open_area)
                dpg.add_separator()
                dpg.add_menu_item(label="Save", callback=self._on_save)
                dpg.add_menu_item(label="Save As...", callback=self._on_save_as)
                dpg.add_separator()
                dpg.add_menu_item(label="Exit", callback=self._on_exit)
            
            # Edit menu
            with dpg.menu(label="Edit"):
                dpg.add_menu_item(label="Undo", enabled=False)
                dpg.add_menu_item(label="Redo", enabled=False)
                dpg.add_separator()
                dpg.add_menu_item(label="Cut", enabled=False)
                dpg.add_menu_item(label="Copy", enabled=False)
                dpg.add_menu_item(label="Paste", enabled=False)
            
            # View menu
            with dpg.menu(label="View"):
                dpg.add_menu_item(label="Show Map", callback=self._on_show_map)
                dpg.add_menu_item(label="Show Validation", callback=self._on_show_validation)
            
            # Tools menu
            with dpg.menu(label="Tools"):
                dpg.add_menu_item(label="Validate Area", callback=self._on_validate)
                dpg.add_menu_item(label="Find References", callback=self._on_find_references)
            
            # Help menu
            with dpg.menu(label="Help"):
                dpg.add_menu_item(label="Documentation", callback=self._on_documentation)
                dpg.add_menu_item(label="About", callback=self._on_about)
    
    def _file_dialog_callback(self, sender, app_data):
        """Handle file dialog selection."""
        selections = app_data.get('selections', {})
        if selections:
            # Get the first selected file
            filepath = list(selections.values())[0]
            self.main_window.open_area_file(filepath)

    def _on_new_area(self):
        """Handle New Area menu item."""
        print("New Area clicked")

    def _on_open_area(self):
        """Handle Open Area menu item."""
        dpg.show_item("file_dialog")
    
    def _on_save(self):
        """Handle Save menu item."""
        print("Save clicked")
    
    def _on_save_as(self):
        """Handle Save As menu item."""
        print("Save As clicked")
    
    def _on_exit(self):
        """Handle Exit menu item."""
        dpg.stop_dearpygui()
    
    def _on_show_map(self):
        """Handle Show Map menu item."""
        print("Show Map clicked")
    
    def _on_show_validation(self):
        """Handle Show Validation menu item."""
        print("Show Validation clicked")
    
    def _on_validate(self):
        """Handle Validate Area menu item."""
        print("Validate Area clicked")
    
    def _on_find_references(self):
        """Handle Find References menu item."""
        print("Find References clicked")
    
    def _on_documentation(self):
        """Handle Documentation menu item."""
        print("Documentation clicked")
    
    def _on_about(self):
        """Handle About menu item."""
        print("About clicked")

