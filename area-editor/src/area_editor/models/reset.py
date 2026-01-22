"""
Reset data model.
"""
from dataclasses import dataclass


@dataclass
class Reset:
    """Represents a MUD reset command."""
    
    command: str = ""  # M=mobile, O=object, P=put, G=give, E=equip, D=door, R=randomize, S=stop
    arg1: int = 0  # Varies by command type
    arg2: int = 0  # Varies by command type
    arg3: int = 0  # Varies by command type
    arg4: int = 0  # Varies by command type
    
    # For M (mobile) command:
    # arg1 = mobile vnum
    # arg2 = max count in world
    # arg3 = room vnum
    # arg4 = max count in room
    
    # For O (object) command:
    # arg1 = object vnum
    # arg2 = 0 (unused)
    # arg3 = room vnum
    # arg4 = 0 (unused)
    
    # For P (put object in object) command:
    # arg1 = object vnum to put
    # arg2 = max count
    # arg3 = container object vnum
    # arg4 = max count in container
    
    # For G (give object to mobile) command:
    # arg1 = object vnum
    # arg2 = max count
    # arg3 = 0 (unused)
    # arg4 = 0 (unused)
    
    # For E (equip object on mobile) command:
    # arg1 = object vnum
    # arg2 = max count
    # arg3 = wear location
    # arg4 = 0 (unused)

