"""
Menu bar component for the Area Editor.
"""
import dearpygui.dearpygui as dpg
from pathlib import Path
from area_editor.config import config


class MenuBar:
    """Menu bar with File, Edit, View, Tools, and Help menus."""

    def __init__(self, main_window):
        """Initialize the menu bar."""
        self.main_window = main_window
        self.file_dialog_id = None
    
    def create(self):
        """Create the menu bar."""
        # Create file dialog (hidden by default) - supports multiple file selection
        with dpg.file_dialog(
            directory_selector=False,
            show=False,
            callback=self._file_dialog_callback,
            file_count=0,  # 0 = unlimited file selection
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
                dpg.add_separator()
                with dpg.menu(label="Font Size"):
                    dpg.add_menu_item(label="Auto (Based on Resolution)", callback=self._set_font_scale_auto)
                    dpg.add_separator()
                    dpg.add_menu_item(label="Small (75%)", callback=lambda: self._set_font_scale(0.75))
                    dpg.add_menu_item(label="Normal (100%)", callback=lambda: self._set_font_scale(1.0))
                    dpg.add_menu_item(label="Medium (125%)", callback=lambda: self._set_font_scale(1.25))
                    dpg.add_menu_item(label="Large (150%)", callback=lambda: self._set_font_scale(1.5))
                    dpg.add_menu_item(label="Extra Large (175%)", callback=lambda: self._set_font_scale(1.75))
                    dpg.add_menu_item(label="Huge (200%)", callback=lambda: self._set_font_scale(2.0))
                    dpg.add_menu_item(label="Massive (250%)", callback=lambda: self._set_font_scale(2.5))
                    dpg.add_menu_item(label="Giant (300%)", callback=lambda: self._set_font_scale(3.0))
            
            # Tools menu
            with dpg.menu(label="Tools"):
                dpg.add_menu_item(label="Validate Area", callback=self._on_validate)
                dpg.add_menu_item(label="Find References", callback=self._on_find_references)
            
            # Help menu
            with dpg.menu(label="Help"):
                dpg.add_menu_item(label="Documentation", callback=self._on_documentation)
                dpg.add_menu_item(label="About", callback=self._on_about)
    
    def _file_dialog_callback(self, sender, app_data):
        """Handle file dialog selection - supports multiple files."""
        selections = app_data.get('selections', {})
        if selections:
            # Load all selected files
            for filepath in selections.values():
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

    def _set_font_scale(self, scale: float):
        """Set the global font scale manually (disables auto-scaling).

        Args:
            scale: Font scale multiplier (1.0 = 100%)
        """
        dpg.set_global_font_scale(scale)
        config.set_font_scale(scale, auto=False)
        config.save()

    def _set_font_scale_auto(self):
        """Enable automatic font scaling based on resolution."""
        # Re-enable auto-scaling
        config.set_font_scale(1.0, auto=True)
        config.save()

        # Recalculate and apply the scale immediately
        import math
        window_width = dpg.get_viewport_width()
        window_height = dpg.get_viewport_height()

        baseline_width = 1920.0
        baseline_height = 1080.0

        width_ratio = window_width / baseline_width
        height_ratio = window_height / baseline_height

        base_scale = math.sqrt(width_ratio * height_ratio)

        # Double the scale factor for better readability
        font_scale = base_scale * 2.0

        font_scale = max(0.75, min(3.0, font_scale))
        font_scale = round(font_scale * 4) / 4

        dpg.set_global_font_scale(font_scale)
        config.set_font_scale(font_scale, auto=True)
        config.save()

