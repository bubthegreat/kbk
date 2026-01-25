"""
Templates for creating new areas, rooms, objects, and mobiles.
"""
from area_editor.models.area import Area
from area_editor.models.room import Room


def create_new_area(name: str = "New Area", author: str = "Builder", min_vnum: int = 1000, max_vnum: int = 1099) -> Area:
    """
    Create a new area with a single default room and all required sections.
    
    Args:
        name: Name of the area
        author: Author/builder name
        min_vnum: Minimum vnum for the area
        max_vnum: Maximum vnum for the area
    
    Returns:
        Area object with a single default room
    """
    area = Area(
        name=name,
        author=author,
        min_vnum=min_vnum,
        max_vnum=max_vnum,
        security=9,
        area_flags=0,
        min_level=1,
        max_level=50
    )
    
    # Create a single default room
    default_room = Room(
        vnum=min_vnum,
        name="A New Room",
        description="This is a newly created room. Edit this description to describe what the room looks like.\n",
        room_flags=0,
        sector_type=0,  # Inside
        area_number=0
    )
    
    area.rooms[min_vnum] = default_room
    
    # Initialize empty collections (already done by dataclass defaults, but explicit for clarity)
    # area.mobiles = {}
    # area.objects = {}
    # area.resets = []
    # area.shops = []
    # area.specials = []
    # area.helps = []
    
    return area


def create_default_room(vnum: int) -> Room:
    """
    Create a default room with the given vnum.
    
    Args:
        vnum: Virtual number for the room
    
    Returns:
        Room object with default values
    """
    return Room(
        vnum=vnum,
        name="A New Room",
        description="This is a newly created room. Edit this description to describe what the room looks like.\n",
        room_flags=0,
        sector_type=0,  # Inside
        area_number=0
    )

