"""
Area data model.
"""
from dataclasses import dataclass, field
from typing import Dict, List


@dataclass
class Area:
    """Represents a MUD area."""

    name: str = ""
    filename: str = ""
    builders: str = "None"  # Builders field
    min_level: int = 1
    max_level: int = 50
    author: str = ""
    min_vnum: int = 0
    max_vnum: int = 0
    area_flags: int = 0
    security: int = 9
    credits: str = ""  # Credits line: "{min max} Author  AreaName~"
    xplore: int = 0  # Xplore flag
    recall: int = 0  # Recall vnum (optional, may not be in original files)
    
    # Collections of area contents
    rooms: Dict[int, 'Room'] = field(default_factory=dict)
    objects: Dict[int, 'Object'] = field(default_factory=dict)
    mobiles: Dict[int, 'Mobile'] = field(default_factory=dict)
    helps: List['Help'] = field(default_factory=list)
    resets: List['Reset'] = field(default_factory=list)
    shops: List['Shop'] = field(default_factory=list)
    specials: List['Special'] = field(default_factory=list)
    
    def get_vnum_range(self) -> str:
        """Get the vnum range as a string."""
        return f"{self.min_vnum}-{self.max_vnum}"
    
    def is_vnum_in_range(self, vnum: int) -> bool:
        """Check if a vnum is within this area's range."""
        return self.min_vnum <= vnum <= self.max_vnum
    
    def get_next_available_vnum(self, entity_type: str) -> int:
        """Get the next available vnum for the specified entity type."""
        if entity_type == "room":
            used_vnums = set(self.rooms.keys())
        elif entity_type == "object":
            used_vnums = set(self.objects.keys())
        elif entity_type == "mobile":
            used_vnums = set(self.mobiles.keys())
        else:
            return self.min_vnum
        
        for vnum in range(self.min_vnum, self.max_vnum + 1):
            if vnum not in used_vnums:
                return vnum
        
        return -1  # No available vnums

