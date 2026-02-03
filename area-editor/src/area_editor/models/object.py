"""
Object data model.
"""
from dataclasses import dataclass, field
from typing import List


@dataclass
class ObjectAffect:
    """Represents an object affect (apply)."""

    location: str = ""  # Where the affect applies (e.g., 'O' for object)
    apply_type: int = 0  # What stat is affected
    modifier: int = 0  # How much to modify
    bitvector: int = 0  # Additional flags


@dataclass
class ExtraDescription:
    """Represents an extra description."""

    keywords: str = ""
    description: str = ""


@dataclass
class Object:
    """Represents a MUD object."""

    vnum: int = 0
    keywords: str = ""
    short_description: str = ""
    long_description: str = ""
    material: str = "none"

    # TYPE line - format varies by item type
    item_type: str = ""  # e.g., "weapon", "armor", "potion", "money"
    type_values: List[str] = field(default_factory=list)  # Remaining values on TYPE line

    # EXTRA line - can have 2, 3, or 4 values
    extra_flags: int = 0
    extra_value2: int = 0
    extra_value3: int = 0
    extra_value4: int = 0

    # WEAR flags
    wear_flags: str = ""

    # Stats
    level: int = 0
    weight: int = 0
    cost: int = 0
    condition: int = 100  # COND
    limit: int = 0  # LIMIT (optional)
    restriction: str = ""  # RESTR (optional)

    extra_descriptions: List[ExtraDescription] = field(default_factory=list)
    affects: List[ObjectAffect] = field(default_factory=list)

    # Internal flag to track if this object was loaded from old format
    # (used for validation warnings)
    _loaded_from_old_format: bool = field(default=False, repr=False)

    def add_extra_description(self, keywords: str, description: str) -> ExtraDescription:
        """Add an extra description."""
        extra_desc = ExtraDescription(keywords=keywords, description=description)
        self.extra_descriptions.append(extra_desc)
        return extra_desc
    
    def add_affect(self, apply_type: int, apply_value: int) -> ObjectAffect:
        """Add an affect to the object."""
        affect = ObjectAffect(apply_type=apply_type, apply_value=apply_value)
        self.affects.append(affect)
        return affect
    
    def get_value_by_index(self, index: int) -> int:
        """Get value by index (0-3)."""
        if index == 0:
            return self.value0
        elif index == 1:
            return self.value1
        elif index == 2:
            return self.value2
        elif index == 3:
            return self.value3
        return 0
    
    def set_value_by_index(self, index: int, value: int):
        """Set value by index (0-3)."""
        if index == 0:
            self.value0 = value
        elif index == 1:
            self.value1 = value
        elif index == 2:
            self.value2 = value
        elif index == 3:
            self.value3 = value

