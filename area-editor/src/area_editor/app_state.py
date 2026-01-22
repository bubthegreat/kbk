"""
Application state manager for the Area Editor.
"""
from typing import Optional, Dict
from pathlib import Path
from area_editor.models.area import Area
from area_editor.validation import ValidationResult


class AppState:
    """Manages the application state including loaded areas."""

    def __init__(self):
        """Initialize the application state."""
        self.areas: Dict[str, Area] = {}  # Maps area_id (filename) -> Area
        self.area_files: Dict[str, Path] = {}  # Maps area_id -> filepath
        self.area_modified: Dict[str, bool] = {}  # Maps area_id -> is_modified
        self.validation_results: Dict[str, ValidationResult] = {}  # Maps area_id -> ValidationResult
        self.current_area_id: Optional[str] = None  # Currently active area
        self.selected_item_type: Optional[str] = None  # 'room', 'object', 'mobile', 'area'
        self.selected_item_vnum: Optional[int] = None

    @property
    def current_area(self) -> Optional[Area]:
        """Get the currently active area."""
        if self.current_area_id:
            return self.areas.get(self.current_area_id)
        return None

    @property
    def current_file(self) -> Optional[Path]:
        """Get the file path of the currently active area."""
        if self.current_area_id:
            return self.area_files.get(self.current_area_id)
        return None

    @property
    def is_modified(self) -> bool:
        """Check if the currently active area is modified."""
        if self.current_area_id:
            return self.area_modified.get(self.current_area_id, False)
        return False

    def load_area(self, area: Area, filepath: Path):
        """Load an area into the application state."""
        area_id = filepath.name  # Use filename as unique ID
        self.areas[area_id] = area
        self.area_files[area_id] = filepath
        self.area_modified[area_id] = False
        self.current_area_id = area_id  # Make this the active area
        # Don't clear selection when loading a new area

    def set_current_area(self, area_id: str):
        """Set the currently active area by ID."""
        if area_id in self.areas:
            self.current_area_id = area_id
            # Clear selection when switching areas
            self.selected_item_type = None
            self.selected_item_vnum = None

    def close_area(self, area_id: str):
        """Close an area."""
        if area_id in self.areas:
            del self.areas[area_id]
            del self.area_files[area_id]
            del self.area_modified[area_id]

            # If we closed the current area, switch to another or None
            if self.current_area_id == area_id:
                if self.areas:
                    self.current_area_id = next(iter(self.areas.keys()))
                else:
                    self.current_area_id = None
                self.selected_item_type = None
                self.selected_item_vnum = None

    def clear(self):
        """Clear all areas."""
        self.areas.clear()
        self.area_files.clear()
        self.area_modified.clear()
        self.current_area_id = None
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
    
    def mark_modified(self, area_id: Optional[str] = None):
        """Mark an area as modified.

        Args:
            area_id: ID of the area to mark as modified. If None, uses current area.
        """
        if area_id is None:
            area_id = self.current_area_id
        if area_id and area_id in self.areas:
            self.area_modified[area_id] = True

    def get_file_info(self) -> str:
        """Get file information string for status bar."""
        if not self.current_file or not self.current_area:
            if not self.areas:
                return "No areas loaded"
            else:
                return f"{len(self.areas)} area(s) loaded"

        modified_marker = "*" if self.is_modified else ""
        return f"{self.current_file.name}{modified_marker} - {len(self.current_area.rooms)} rooms, {len(self.current_area.objects)} objects, {len(self.current_area.mobiles)} mobiles"

    def set_validation_result(self, area_id: str, result: ValidationResult):
        """Store validation results for an area.

        Args:
            area_id: ID of the area
            result: Validation results
        """
        self.validation_results[area_id] = result

    def get_validation_result(self, area_id: Optional[str] = None) -> Optional[ValidationResult]:
        """Get validation results for an area.

        Args:
            area_id: ID of the area. If None, uses current area.

        Returns:
            ValidationResult if available, None otherwise
        """
        if area_id is None:
            area_id = self.current_area_id
        if area_id:
            return self.validation_results.get(area_id)
        return None

    def has_validation_errors(self, item_type: str, item_vnum: int, area_id: Optional[str] = None) -> bool:
        """Check if an item has validation errors.

        Args:
            item_type: Type of item ('room', 'object', 'mobile')
            item_vnum: Vnum of the item
            area_id: ID of the area. If None, uses current area.

        Returns:
            True if the item has validation errors
        """
        result = self.get_validation_result(area_id)
        if result:
            errors = result.get_errors_for_item(item_type, item_vnum)
            return len(errors) > 0
        return False


# Global application state instance
app_state = AppState()

