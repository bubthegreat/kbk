"""
Validation system for area files.

This module provides validation for area files to detect common errors such as:
- Missing room references in exits
- Missing object/mobile references
- Invalid vnum ranges
- Malformed data
"""
from dataclasses import dataclass, field
from typing import Dict, List, Set, Optional
from area_editor.models import Area, Room, Object, Mobile, Shop, Reset


@dataclass
class ValidationError:
    """Represents a validation error."""
    
    error_type: str  # e.g., "missing_room", "invalid_vnum", "missing_object"
    severity: str  # "error", "warning", "info"
    item_type: str  # "room", "object", "mobile", "area"
    item_vnum: int  # The vnum of the item with the error
    message: str  # Human-readable error message
    details: str = ""  # Additional details about the error
    
    def __str__(self) -> str:
        """Return a string representation of the error."""
        return f"[{self.severity.upper()}] {self.item_type} #{self.item_vnum}: {self.message}"


@dataclass
class ValidationResult:
    """Results from validating an area."""
    
    area_id: str  # Area identifier (filename)
    errors: List[ValidationError] = field(default_factory=list)
    warnings: List[ValidationError] = field(default_factory=list)
    
    def add_error(self, error: ValidationError):
        """Add an error to the results."""
        if error.severity == "error":
            self.errors.append(error)
        elif error.severity == "warning":
            self.warnings.append(error)
    
    def has_errors(self) -> bool:
        """Check if there are any errors."""
        return len(self.errors) > 0
    
    def has_warnings(self) -> bool:
        """Check if there are any warnings."""
        return len(self.warnings) > 0
    
    def get_errors_for_item(self, item_type: str, item_vnum: int) -> List[ValidationError]:
        """Get all errors for a specific item."""
        return [e for e in self.errors + self.warnings 
                if e.item_type == item_type and e.item_vnum == item_vnum]


class AreaValidator:
    """Validates area files for common errors."""

    def __init__(self, area: Area, area_id: str, all_areas: Optional[Dict[str, Area]] = None):
        """Initialize the validator.

        Args:
            area: The area to validate
            area_id: Unique identifier for the area (usually filename)
            all_areas: Optional dict of all loaded areas (for cross-area validation)
        """
        self.area = area
        self.area_id = area_id
        self.all_areas = all_areas or {}
        self.result = ValidationResult(area_id=area_id)
    
    def validate(self) -> ValidationResult:
        """Run all validations and return the results."""
        self._validate_vnums()
        self._validate_room_exits()
        self._validate_room_references()
        self._validate_object_format()
        self._validate_shops()
        self._validate_resets()
        return self.result
    
    def _validate_vnums(self):
        """Validate that all vnums are within the area's vnum range."""
        # Check rooms
        for vnum, room in self.area.rooms.items():
            if not (self.area.min_vnum <= vnum <= self.area.max_vnum):
                self.result.add_error(ValidationError(
                    error_type="invalid_vnum",
                    severity="error",
                    item_type="room",
                    item_vnum=vnum,
                    message=f"Room vnum {vnum} is outside area range ({self.area.min_vnum}-{self.area.max_vnum})"
                ))
        
        # Check objects
        for vnum, obj in self.area.objects.items():
            if not (self.area.min_vnum <= vnum <= self.area.max_vnum):
                self.result.add_error(ValidationError(
                    error_type="invalid_vnum",
                    severity="error",
                    item_type="object",
                    item_vnum=vnum,
                    message=f"Object vnum {vnum} is outside area range ({self.area.min_vnum}-{self.area.max_vnum})"
                ))
        
        # Check mobiles
        for vnum, mob in self.area.mobiles.items():
            if not (self.area.min_vnum <= vnum <= self.area.max_vnum):
                self.result.add_error(ValidationError(
                    error_type="invalid_vnum",
                    severity="error",
                    item_type="mobile",
                    item_vnum=vnum,
                    message=f"Mobile vnum {vnum} is outside area range ({self.area.min_vnum}-{self.area.max_vnum})"
                ))
    
    def _validate_room_exits(self):
        """Validate that room exits point to valid rooms."""
        for vnum, room in self.area.rooms.items():
            for direction, exit_data in room.exits.items():
                target_vnum = exit_data.to_room

                # Skip validation for description-only directions (to_room = 0)
                if target_vnum == 0:
                    continue

                # Check if target room exists in this area
                if target_vnum not in self.area.rooms:
                    # Check if it exists in other loaded areas (cross-area exit)
                    found_in_other_area = False
                    for other_area_id, other_area in self.all_areas.items():
                        if other_area_id != self.area_id and target_vnum in other_area.rooms:
                            found_in_other_area = True
                            break

                    # Only warn if the room doesn't exist in ANY loaded area
                    if not found_in_other_area:
                        direction_names = ["north", "east", "south", "west", "up", "down"]
                        dir_name = direction_names[direction] if 0 <= direction < 6 else str(direction)

                        self.result.add_error(ValidationError(
                            error_type="missing_room",
                            severity="warning",
                            item_type="room",
                            item_vnum=vnum,
                            message=f"Exit {dir_name} points to room #{target_vnum} which does not exist in any loaded area",
                            details=f"This room may be in an area that is not currently loaded, or it may be missing"
                        ))
    
    def _validate_room_references(self):
        """Validate that rooms have basic required data."""
        for vnum, room in self.area.rooms.items():
            # Check for empty room name
            if not room.name or room.name.strip() == "":
                self.result.add_error(ValidationError(
                    error_type="empty_name",
                    severity="warning",
                    item_type="room",
                    item_vnum=vnum,
                    message="Room has no name"
                ))

            # Check for empty description
            if not room.description or room.description.strip() == "":
                self.result.add_error(ValidationError(
                    error_type="empty_description",
                    severity="warning",
                    item_type="room",
                    item_vnum=vnum,
                    message="Room has no description"
                ))

    def _validate_object_format(self):
        """Validate that objects are not using the old format."""
        for vnum, obj in self.area.objects.items():
            # Check if object was loaded from old format (VALUES/STATS keywords)
            if hasattr(obj, '_loaded_from_old_format') and obj._loaded_from_old_format:
                self.result.add_error(ValidationError(
                    error_type="old_format",
                    severity="warning",
                    item_type="object",
                    item_vnum=vnum,
                    message="Object uses old format (VALUES/STATS keywords)",
                    details="This file will be converted to the new format (LEVEL/WEIGHT/COST/COND) when saved. "
                           "The MUD server only supports the new format."
                ))

    def _validate_shops(self):
        """Validate that shops reference valid mobiles."""
        for shop in self.area.shops:
            # Check if keeper mobile exists
            if shop.keeper not in self.area.mobiles:
                self.result.add_error(ValidationError(
                    error_type="missing_mobile",
                    severity="error",
                    item_type="shop",
                    item_vnum=shop.keeper,
                    message=f"Shop keeper mobile #{shop.keeper} does not exist",
                    details="Shop references a mobile that is not defined in this area"
                ))

    def _validate_resets(self):
        """Validate that resets reference valid rooms, mobiles, and objects."""
        for i, reset in enumerate(self.area.resets):
            if reset.command == 'M':
                # Mobile reset - check mobile and room exist
                mob_vnum = reset.arg1
                room_vnum = reset.arg3

                if mob_vnum not in self.area.mobiles:
                    self.result.add_error(ValidationError(
                        error_type="missing_mobile",
                        severity="error",
                        item_type="reset",
                        item_vnum=mob_vnum,
                        message=f"Reset #{i}: Mobile #{mob_vnum} does not exist",
                        details=f"M reset references mobile {mob_vnum} which is not defined"
                    ))

                if room_vnum not in self.area.rooms:
                    self.result.add_error(ValidationError(
                        error_type="missing_room",
                        severity="error",
                        item_type="reset",
                        item_vnum=room_vnum,
                        message=f"Reset #{i}: Room #{room_vnum} does not exist",
                        details=f"M reset tries to place mobile in room {room_vnum} which is not defined"
                    ))

            elif reset.command == 'O':
                # Object reset - check object and room exist
                obj_vnum = reset.arg1
                room_vnum = reset.arg3

                if obj_vnum not in self.area.objects:
                    self.result.add_error(ValidationError(
                        error_type="missing_object",
                        severity="error",
                        item_type="reset",
                        item_vnum=obj_vnum,
                        message=f"Reset #{i}: Object #{obj_vnum} does not exist",
                        details=f"O reset references object {obj_vnum} which is not defined"
                    ))

                if room_vnum not in self.area.rooms:
                    self.result.add_error(ValidationError(
                        error_type="missing_room",
                        severity="error",
                        item_type="reset",
                        item_vnum=room_vnum,
                        message=f"Reset #{i}: Room #{room_vnum} does not exist",
                        details=f"O reset tries to place object in room {room_vnum} which is not defined"
                    ))

            elif reset.command in ['P', 'G', 'E']:
                # Object-related resets - check object exists
                obj_vnum = reset.arg1

                if obj_vnum not in self.area.objects:
                    self.result.add_error(ValidationError(
                        error_type="missing_object",
                        severity="error",
                        item_type="reset",
                        item_vnum=obj_vnum,
                        message=f"Reset #{i}: Object #{obj_vnum} does not exist",
                        details=f"{reset.command} reset references object {obj_vnum} which is not defined"
                    ))

                # For P command, also check container object
                if reset.command == 'P':
                    container_vnum = reset.arg3
                    if container_vnum not in self.area.objects:
                        self.result.add_error(ValidationError(
                            error_type="missing_object",
                            severity="error",
                            item_type="reset",
                            item_vnum=container_vnum,
                            message=f"Reset #{i}: Container object #{container_vnum} does not exist",
                            details=f"P reset tries to put object in container {container_vnum} which is not defined"
                        ))

