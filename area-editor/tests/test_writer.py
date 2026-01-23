"""
Tests for the area writer module.
"""
import pytest
from pathlib import Path
import tempfile
from area_editor.templates import create_new_area
from area_editor.writers.are_writer import AreWriter
from area_editor.parsers.are_parser import AreParser
from area_editor.models.area import Area
from area_editor.models.room import Room, Exit
from area_editor.models.mobile import Mobile
from area_editor.models.object import Object
from area_editor.models.reset import Reset
from area_editor.models.shop import Shop
from area_editor.models.special import Special


def test_create_new_area_template():
    """Test that create_new_area creates a valid area with a single room."""
    area = create_new_area(name="Test Area", author="TestBuilder", min_vnum=1000, max_vnum=1099)
    
    assert area.name == "Test Area"
    assert area.author == "TestBuilder"
    assert area.min_vnum == 1000
    assert area.max_vnum == 1099
    assert area.security == 9
    assert len(area.rooms) == 1
    assert 1000 in area.rooms
    assert area.rooms[1000].name == "A New Room"
    assert area.rooms[1000].vnum == 1000
    assert len(area.mobiles) == 0
    assert len(area.objects) == 0
    assert len(area.resets) == 0
    assert len(area.shops) == 0
    assert len(area.specials) == 0


def test_write_and_parse_new_area():
    """Test round-trip: create area -> write to file -> parse file -> verify data."""
    # Create a new area
    area = create_new_area(name="Round Trip Test", author="Tester", min_vnum=2000, max_vnum=2099)
    
    # Write to a temporary file
    with tempfile.NamedTemporaryFile(mode='w', suffix='.are', delete=False) as f:
        temp_path = Path(f.name)
    
    try:
        writer = AreWriter(area)
        writer.write(temp_path)
        
        # Parse the file back
        parser = AreParser(str(temp_path))
        parsed_area = parser.parse()
        
        # Verify the data matches
        assert parsed_area is not None
        assert parsed_area.name == "Round Trip Test"
        assert parsed_area.author == "Tester"
        assert parsed_area.min_vnum == 2000
        assert parsed_area.max_vnum == 2099
        assert len(parsed_area.rooms) == 1
        assert 2000 in parsed_area.rooms
        assert parsed_area.rooms[2000].name == "A New Room"
    finally:
        # Clean up
        temp_path.unlink()


def test_write_area_with_all_sections():
    """Test writing an area with all possible sections."""
    area = Area(
        name="Complete Test Area",
        author="CompleteBuilder",
        min_vnum=3000,
        max_vnum=3099,
        security=9
    )
    
    # Add a room
    room = Room(
        vnum=3000,
        name="Test Room",
        description="A test room.\\n",
        room_flags=0,
        sector_type=0
    )
    room.exits[0] = Exit(
        direction=0,
        to_room=3001,
        description="A door to the north.\\n",
        keywords="door north",
        locks=0,
        key_vnum=0
    )
    area.rooms[3000] = room
    
    # Add another room for the exit
    area.rooms[3001] = Room(
        vnum=3001,
        name="Second Room",
        description="Another test room.\\n",
        room_flags=0,
        sector_type=0
    )
    
    # Add a mobile
    mob = Mobile(
        vnum=3000,
        keywords="test mob",
        short_description="a test mobile",
        long_description="A test mobile stands here.",
        description="This is a test mobile for testing purposes.\\n",
        race="human",
        act_flags="0",
        affected_flags="0",
        alignment=0,
        level=10,
        hitroll=5,
        damage_dice="1d6+2",
        mana_dice="100d1+500",
        hit_dice="10d10+100",
        gold=100,
        start_position="stand",
        default_position="stand",
        sex="male"
    )
    area.mobiles[3000] = mob
    
    # Add an object
    obj = Object(
        vnum=3000,
        keywords="test sword",
        short_description="a test sword",
        long_description="A test sword lies here.",
        item_type="weapon",
        type_values=["sword", "10", "20", "slash", "0"],
        extra_flags=0,
        wear_flags="take wield",
        level=10,
        weight=50,
        cost=1000
    )
    area.objects[3000] = obj
    
    # Add a reset (mobile in room)
    area.resets.append(Reset(command="M", arg1=3000, arg2=1, arg3=3000, arg4=1))
    
    # Add a shop
    shop = Shop(
        keeper=3000,
        buy_types=[1, 2, 3, 4, 5],
        profit_buy=120,
        profit_sell=80,
        open_hour=6,
        close_hour=20
    )
    area.shops.append(shop)
    
    # Add a special
    special = Special(vnum=3000, spec_function="spec_guard")
    area.specials.append(special)
    
    # Write to a temporary file
    with tempfile.NamedTemporaryFile(mode='w', suffix='.are', delete=False) as f:
        temp_path = Path(f.name)
    
    try:
        writer = AreWriter(area)
        writer.write(temp_path)
        
        # Verify the file was created and has content
        assert temp_path.exists()
        content = temp_path.read_text()
        
        # Check that all sections are present
        assert "#AREADATA" in content
        assert "#MOBDATA" in content
        assert "#OBJDATA" in content
        assert "#ROOMDATA" in content
        assert "#SPECIALS" in content
        assert "#RESETS" in content
        assert "#SHOPS" in content
        assert "#$" in content
        
        # Check specific data
        assert "Complete Test Area" in content
        assert "CompleteBuilder" in content
        assert "3000 3099" in content
        assert "Test Room" in content
        assert "test mob" in content
        assert "test sword" in content
        assert "spec_guard" in content
    finally:
        # Clean up
        temp_path.unlink()

