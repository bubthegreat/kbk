"""
Special procedure data model.
"""
from dataclasses import dataclass


@dataclass
class Special:
    """Represents a special procedure assigned to a mobile."""
    
    vnum: int = 0  # Mobile vnum
    spec_function: str = ""  # Name of the special function (e.g., "spec_breath_fire")

