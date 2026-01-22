"""
Status bar component showing application status and information.
"""
import dearpygui.dearpygui as dpg


class StatusBar:
    """Status bar at the bottom of the application."""
    
    def __init__(self):
        """Initialize the status bar."""
        self.status_text_id = None
        self.file_info_id = None
    
    def create(self):
        """Create the status bar."""
        with dpg.group(horizontal=True):
            self.status_text_id = dpg.add_text("Ready", color=(150, 150, 150))
            dpg.add_spacer(width=20)
            self.file_info_id = dpg.add_text("No file loaded", color=(120, 120, 120))
    
    def set_status(self, message, color=(150, 150, 150)):
        """Set the status message."""
        if self.status_text_id:
            dpg.set_value(self.status_text_id, message)
            dpg.configure_item(self.status_text_id, color=color)
    
    def set_file_info(self, info):
        """Set the file information."""
        if self.file_info_id:
            dpg.set_value(self.file_info_id, info)

