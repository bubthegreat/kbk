"""
Application state manager for the Area Editor.
"""
from typing import Optional
from pathlib import Path
from area_editor.models.area import Area


class AppState:
    """Manages the application state including loaded areas."""
    
    def __init__(self):
        """Initialize the application state."""
        self.current_area: Optional[Area] = None
        self.current_file: Optional[Path] = None
        self.is_modified: bool = False
        self.selected_item_type: Optional[str] = None  # 'room', 'object', 'mobile', 'area'
        self.selected_item_vnum: Optional[int] = None
    
    def load_area(self, area: Area, filepath: Path):
        """Load an area into the application state."""
        self.current_area = area
        self.current_file = filepath
        self.is_modified = False
        self.selected_item_type = None
        self.selected_item_vnum = None
    
    def clear(self):
        """Clear the current area."""
        self.current_area = None
        self.current_file = None
        self.is_modified = False
        self.selected_item_type = None
        self.selected_item_vnum = None
    
    def select_item(self, item_type: str, vnum: int):
        """Select an item in the editor."""
        self.selected_item_type = item_type
        self.selected_item_vnum = vnum
    
    def get_selected_item(self):
        """Get the currently selected item."""
        if not self.current_area or not self.selected_item_type:
            return None
        
        if self.selected_item_type == 'room':
            return self.current_area.rooms.get(self.selected_item_vnum)
        elif self.selected_item_type == 'object':
            return self.current_area.objects.get(self.selected_item_vnum)
        elif self.selected_item_type == 'mobile':
            return self.current_area.mobiles.get(self.selected_item_vnum)
        elif self.selected_item_type == 'area':
            return self.current_area
        
        return None
    
    def mark_modified(self):
        """Mark the current area as modified."""
        self.is_modified = True
    
    def get_file_info(self) -> str:
        """Get file information string for status bar."""
        if not self.current_file:
            return "No file loaded"
        
        modified_marker = "*" if self.is_modified else ""
        return f"{self.current_file.name}{modified_marker} - {len(self.current_area.rooms)} rooms, {len(self.current_area.objects)} objects, {len(self.current_area.mobiles)} mobiles"


# Global application state instance
app_state = AppState()

