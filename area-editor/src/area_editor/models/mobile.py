"""
Mobile data model.
"""
from dataclasses import dataclass


@dataclass
class Mobile:
    """Represents a MUD mobile (NPC)."""

    vnum: int = 0
    keywords: str = ""
    short_description: str = ""
    long_description: str = ""
    description: str = ""
    race: str = "human"

    # Flags
    act_flags: str = ""  # ACT flags (letter codes)
    affected_flags: str = ""  # AFF flags (letter codes)
    offensive_flags: str = ""  # OFF flags (letter codes)
    immunity_flags: str = ""  # IMM flags (letter codes)
    resistance_flags: str = ""  # RES flags (letter codes)
    vulnerability_flags: str = ""  # VULN flags (letter codes)

    # Stats
    alignment: int = 0
    group: int = 0
    level: int = 1
    hitroll: int = 0
    enhancement: str = "100.00%"  # ENHA field
    material: str = "flesh"  # MATER field

    # Dice notation strings
    hit_dice: str = "1d1+0"  # HDICE
    mana_dice: str = "1d1+0"  # MDICE
    damage_dice: str = "1d1+0"  # DDICE

    # Other stats
    regen: int = 0  # REGEN
    damage_type: str = ""  # DTYPE

    # Armor class (4 values)
    ac_pierce: int = 0
    ac_bash: int = 0
    ac_slash: int = 0
    ac_exotic: int = 0

    # Position
    start_position: str = "stand"  # First value in POS
    default_position: str = "stand"  # Second value in POS

    # Other
    sex: str = "none"  # SEX
    gold: int = 0
    form: str = ""  # FORM flags
    parts: str = ""  # PARTS flags
    size: str = "medium"  # SIZE
    damage_modifier: int = 0  # DMOD
    armor_modifier: int = 0  # AMOD
    quest: int = 0  # QUEST
    weapon_spec: int = 0  # WSPEC

    # Special function and shop (if any)
    spec_fun: str = ""
    shop_data: 'Shop' = None

