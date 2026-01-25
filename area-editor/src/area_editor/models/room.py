"""
Room data model.
"""
from dataclasses import dataclass, field
from typing import Dict, List, Optional


@dataclass
class Exit:
    """Represents a room exit."""
    
    direction: int  # 0=north, 1=east, 2=south, 3=west, 4=up, 5=down
    description: str = ""
    keywords: str = ""
    locks: int = 0  # 0=open, 1=door, 2=pickproof
    key_vnum: int = 0
    to_room: int = 0


@dataclass
class ExtraDescription:
    """Represents an extra description."""
    
    keywords: str = ""
    description: str = ""


@dataclass
class Room:
    """Represents a MUD room."""
    
    vnum: int = 0
    name: str = "Unnamed Room"
    description: str = ""
    area_number: int = 0  # Obsolete but kept for compatibility
    room_flags: int = 0
    sector_type: int = 0
    
    exits: Dict[int, Exit] = field(default_factory=dict)
    extra_descriptions: List[ExtraDescription] = field(default_factory=list)
    
    # Visual map position (not part of .are format)
    map_x: int = 0
    map_y: int = 0
    
    def get_exit(self, direction: int) -> Optional[Exit]:
        """Get exit in the specified direction."""
        return self.exits.get(direction)
    
    def add_exit(self, direction: int, to_room: int, **kwargs) -> Exit:
        """Add an exit in the specified direction."""
        exit_data = Exit(direction=direction, to_room=to_room, **kwargs)
        self.exits[direction] = exit_data
        return exit_data
    
    def remove_exit(self, direction: int) -> bool:
        """Remove an exit in the specified direction."""
        if direction in self.exits:
            del self.exits[direction]
            return True
        return False
    
    def add_extra_description(self, keywords: str, description: str) -> ExtraDescription:
        """Add an extra description."""
        extra_desc = ExtraDescription(keywords=keywords, description=description)
        self.extra_descriptions.append(extra_desc)
        return extra_desc

