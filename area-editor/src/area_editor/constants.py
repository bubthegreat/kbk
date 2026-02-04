"""
Constants for MUD area editing.

This module contains constants for room flags, sector types, and other
MUD-specific values based on the ROM 2.4 codebase.
"""
from typing import Dict, List, Tuple

# Room flags (from src/merc.h and src/tables.c)
ROOM_FLAGS: List[Tuple[str, int, str]] = [
    ("dark", 1, "Room is dark"),
    ("no_mob", 4, "Mobs cannot enter"),
    ("indoors", 8, "Room is indoors"),
    ("private", 512, "Room is private (max 2 people)"),
    ("safe", 1024, "No combat allowed"),
    ("solitary", 2048, "Room is solitary (max 1 person)"),
    ("pet_shop", 4096, "Room is a pet shop"),
    ("no_recall", 8192, "Cannot recall from room"),
    ("imp_only", 16384, "Implementors only"),
    ("gods_only", 32768, "Gods only"),
    ("heroes_only", 65536, "Heroes only"),
    ("newbies_only", 131072, "Newbies only"),
    ("law", 262144, "Law enforcement active"),
    ("nowhere", 524288, "Cannot use 'where' command"),
    ("no_gate", 1048576, "Cannot gate to/from room"),
    ("no_summon", 8388608, "Cannot summon to/from room"),
    ("no_consecrate", 16777216, "Cannot consecrate room"),
    ("low_only", 134217728, "Low levels only"),
    ("no_magic", 268435456, "No magic allowed"),
    ("consecrated", 4194304, "Room is consecrated"),
    ("no_newbie", 2, "No newbies allowed"),
]

# Sector types (from src/merc.h and src/tables.c)
SECTOR_TYPES: List[Tuple[str, int, str]] = [
    ("inside", 0, "Inside"),
    ("city", 1, "City"),
    ("field", 2, "Field"),
    ("forest", 3, "Forest"),
    ("hills", 4, "Hills"),
    ("mountain", 5, "Mountain"),
    ("swim", 6, "Water (swimmable)"),
    ("noswim", 7, "Water (not swimmable)"),
    ("unused", 8, "Unused"),
    ("air", 9, "Air"),
    ("desert", 10, "Desert"),
    ("road", 11, "Road"),
    ("underwater", 12, "Underwater"),
]

# Exit flags (from src/merc.h and src/tables.c)
EXIT_FLAGS: List[Tuple[str, int, str]] = [
    ("door", 1, "Exit has a door"),
    ("closed", 2, "Door is closed"),
    ("locked", 4, "Door is locked"),
    ("pickproof", 8, "Lock cannot be picked"),
    ("nopass", 16, "Cannot pass through"),
    ("easy", 32, "Easy to pick"),
    ("hard", 64, "Hard to pick"),
    ("infuriating", 128, "Very hard to pick"),
    ("noclose", 256, "Door cannot be closed"),
    ("nolock", 512, "Door cannot be locked"),
    ("nobash", 1024, "Door cannot be bashed"),
    ("nonobvious", 2048, "Exit is not obvious"),
    ("current", 4096, "Water current"),
]

# Position flags (for mobiles)
POSITION_FLAGS: List[Tuple[str, int, str]] = [
    ("dead", 0, "Dead"),
    ("mortal", 1, "Mortally wounded"),
    ("incap", 2, "Incapacitated"),
    ("stunned", 3, "Stunned"),
    ("sleeping", 4, "Sleeping"),
    ("resting", 5, "Resting"),
    ("sitting", 6, "Sitting"),
    ("fighting", 7, "Fighting"),
    ("standing", 8, "Standing"),
]

# Sex flags (for mobiles)
SEX_FLAGS: List[Tuple[str, int, str]] = [
    ("none", 0, "Neutral"),
    ("male", 1, "Male"),
    ("female", 2, "Female"),
]

# Size flags (for mobiles)
SIZE_FLAGS: List[Tuple[str, str, str]] = [
    ("tiny", "tiny", "Tiny"),
    ("small", "small", "Small"),
    ("medium", "medium", "Medium"),
    ("large", "large", "Large"),
    ("huge", "huge", "Huge"),
    ("giant", "giant", "Giant"),
]

# Item types (from src/const.c item_table)
ITEM_TYPES: List[str] = [
    "light", "scroll", "wand", "staff", "weapon", "treasure", "armor",
    "potion", "clothing", "furniture", "trash", "container", "drink",
    "key", "food", "money", "boat", "fountain", "pill", "protect",
    "map", "portal", "warp_stone", "room_key", "gem", "jewelry",
    "jukebox", "skeleton", "instrument"
]

# Race types (from src/const.c race_table) - PC races first, then NPC races
RACE_TYPES: List[str] = [
    # PC races (playable)
    "human", "dwarf", "elf", "svirfnebli", "dark-elf", "centaur", "troll",
    "cloud", "gnome", "draconian", "ethereal", "changeling", "arial", "fire",
    "duergar", "storm", "lich", "angel", "wood-elf", "illithid", "vuuhu",
    "ruugrah", "malefisti",
    # NPC races (non-playable)
    "bat", "bear", "cat", "centipede", "dog", "doll", "dragon", "fido",
    "fish", "fox", "goblin", "hobgoblin", "kobold", "lizard", "modron",
    "pig", "rabbit", "school monster", "snake", "song bird", "water fowl",
    "wolf", "wyvern", "nymph", "titan", "unique"
]

# Weapon types (from src/const.c weapon_table)
WEAPON_TYPES: List[str] = [
    "sword", "mace", "dagger", "axe", "spear", "flail", "whip",
    "polearm", "staff", "hand to hand"
]

# Damage types (from src/const.c attack_table)
DAMAGE_TYPES: List[str] = [
    "none", "slice", "stab", "slash", "whip", "claw", "blast", "pound",
    "crush", "grep", "bite", "pierce", "suction", "beating", "digestion",
    "charge", "slap", "punch"
]

# Wear locations (from src/tables.c wear_loc_flags) - for equipment slots
WEAR_LOCATIONS: List[str] = [
    "none", "light", "lfinger", "rfinger", "neck1", "neck2", "body",
    "head", "legs", "feet", "hands", "arms", "shield", "about", "waist",
    "lwrist", "rwrist", "wielded", "hold", "dualwielded"
]

# Apply types (from src/tables.c apply_types) - for object affects
APPLY_TYPES: List[str] = [
    "affects", "object", "immune", "resist", "vuln", "weapon"
]

# Apply flags (from src/tables.c apply_flags) - what stats can be affected
APPLY_FLAGS: List[str] = [
    "none", "strength", "dexterity", "intelligence", "wisdom", "constitution",
    "sex", "class", "level", "age", "height", "weight", "mana", "hp", "move",
    "gold", "experience", "ac", "hitroll", "damroll"
]

# Helper functions
def get_flag_name(flags: int, flag_list: List[Tuple[str, int, str]]) -> str:
    """Get a human-readable string of flag names from a bit field."""
    if flags == 0:
        return "none"
    
    names = []
    for name, value, _ in flag_list:
        if flags & value:
            names.append(name)
    
    return " ".join(names) if names else "none"


def get_sector_name(sector: int) -> str:
    """Get the name of a sector type."""
    for name, value, _ in SECTOR_TYPES:
        if value == sector:
            return name
    return f"unknown({sector})"


def get_sector_description(sector: int) -> str:
    """Get the description of a sector type."""
    for name, value, desc in SECTOR_TYPES:
        if value == sector:
            return desc
    return "Unknown"


def set_flag(current_flags: int, flag_value: int, enabled: bool) -> int:
    """Set or clear a flag bit."""
    if enabled:
        return current_flags | flag_value
    else:
        return current_flags & ~flag_value


def has_flag(flags: int, flag_value: int) -> bool:
    """Check if a flag is set."""
    return (flags & flag_value) != 0

