"""
Tests for room editor features including tabs, key selection, and helper methods.
"""
import pytest
from area_editor.models.area import Area
from area_editor.models.room import Room, Exit
from area_editor.models.object import Object
from area_editor.app_state import app_state


@pytest.fixture
def test_area():
    """Create a test area with rooms and objects."""
    area = Area(
        name="Test Area",
        filename="test.are",
        min_vnum=1000,
        max_vnum=1999,
        min_level=1,
        max_level=50
    )
    
    # Add a test room with an exit
    room = Room(
        vnum=1000,
        name="Test Room",
        description="A test room.",
        room_flags=0,
        sector_type=0
    )
    room.exits[0] = Exit(direction=0, to_room=1001, description="north exit", keywords="door", locks=0, key_vnum=0)
    area.rooms[1000] = room
    
    # Add another room
    room2 = Room(
        vnum=1001,
        name="North Room",
        description="A room to the north.",
        room_flags=0,
        sector_type=0
    )
    area.rooms[1001] = room2
    
    # Add some key objects
    key1 = Object(
        vnum=1100,
        keywords="brass key",
        short_description="a brass key",
        long_description="A brass key lies here.",
        item_type="key",
        material="brass",
        level=1,
        weight=1,
        cost=10
    )
    area.objects[1100] = key1
    
    key2 = Object(
        vnum=1101,
        keywords="iron key",
        short_description="an iron key",
        long_description="An iron key lies here.",
        item_type="key",
        material="iron",
        level=1,
        weight=1,
        cost=5
    )
    area.objects[1101] = key2
    
    # Add a non-key object
    sword = Object(
        vnum=1102,
        keywords="sword",
        short_description="a sword",
        long_description="A sword lies here.",
        item_type="weapon",
        material="steel",
        level=5,
        weight=10,
        cost=100
    )
    area.objects[1102] = sword
    
    return area


def test_area_has_key_objects(test_area):
    """Test that the test area has key objects."""
    key_objects = [obj for obj in test_area.objects.values() if obj.item_type.lower() == "key"]
    assert len(key_objects) == 2
    assert any(obj.vnum == 1100 for obj in key_objects)
    assert any(obj.vnum == 1101 for obj in key_objects)


def test_area_has_non_key_objects(test_area):
    """Test that the test area has non-key objects."""
    non_key_objects = [obj for obj in test_area.objects.values() if obj.item_type.lower() != "key"]
    assert len(non_key_objects) == 1
    assert non_key_objects[0].vnum == 1102


def test_get_next_available_vnum_for_object(test_area):
    """Test getting the next available vnum for an object."""
    next_vnum = test_area.get_next_available_vnum('object')
    # Should return first available vnum in range (1000-1999)
    # Objects are at 1100, 1101, 1102, so first available is 1000
    assert next_vnum == 1000
    assert next_vnum not in test_area.objects


def test_create_key_object(test_area):
    """Test creating a new key object."""
    next_vnum = test_area.get_next_available_vnum('object')
    
    new_key = Object(
        vnum=next_vnum,
        keywords="key north",
        short_description="a north key",
        long_description="A small key for the north exit lies here.",
        item_type="key",
        material="iron",
        level=1,
        weight=1,
        cost=10,
        condition=100
    )
    
    test_area.objects[next_vnum] = new_key
    
    assert next_vnum in test_area.objects
    assert test_area.objects[next_vnum].item_type == "key"
    assert test_area.objects[next_vnum].keywords == "key north"


def test_link_key_to_exit(test_area):
    """Test linking a key to an exit."""
    room = test_area.rooms[1000]
    exit_obj = room.exits[0]
    
    # Initially no key
    assert exit_obj.key_vnum == 0
    
    # Link a key
    exit_obj.key_vnum = 1100
    assert exit_obj.key_vnum == 1100
    
    # Verify the key exists
    assert 1100 in test_area.objects
    assert test_area.objects[1100].item_type == "key"


def test_clear_key_from_exit(test_area):
    """Test clearing a key from an exit."""
    room = test_area.rooms[1000]
    exit_obj = room.exits[0]
    
    # Set a key
    exit_obj.key_vnum = 1100
    assert exit_obj.key_vnum == 1100
    
    # Clear the key
    exit_obj.key_vnum = 0
    assert exit_obj.key_vnum == 0

