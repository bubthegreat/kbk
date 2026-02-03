"""
Test exit descriptions functionality.
"""
import pytest
from pathlib import Path
from area_editor.models.room import Room, Exit
from area_editor.models.area import Area
from area_editor.writers.are_writer import AreWriter
from area_editor.parsers.are_parser import AreParser


def test_exit_default_description():
    """Test that new exits have the default description."""
    # Create a new exit
    exit_obj = Exit(direction=0, to_room=100)
    
    # Check that it has the default description
    assert exit_obj.description == "You see nothing special here."


def test_exit_custom_description():
    """Test that exits can have custom descriptions."""
    # Create an exit with a custom description
    exit_obj = Exit(
        direction=0,
        to_room=100,
        description="A dark passage leads north."
    )
    
    # Check that it has the custom description
    assert exit_obj.description == "A dark passage leads north."


def test_exit_description_persists_through_write_and_parse(tmp_path):
    """Test that exit descriptions are written and parsed correctly."""
    # Create an area with a room that has exits with descriptions
    area = Area(
        name="Test Area",
        min_vnum=1000,
        max_vnum=1099,
        author="Test Author"
    )
    
    room1 = Room(
        vnum=1000,
        name="Test Room 1",
        description="A test room."
    )
    
    room2 = Room(
        vnum=1001,
        name="Test Room 2",
        description="Another test room."
    )
    
    # Add exits with different descriptions
    room1.exits[0] = Exit(
        direction=0,
        to_room=1001,
        description="You see nothing special here."  # Default
    )
    
    room1.exits[1] = Exit(
        direction=1,
        to_room=1001,
        description="A narrow passage leads east."  # Custom
    )
    
    room2.exits[2] = Exit(
        direction=2,
        to_room=1000
        # Uses default description
    )
    
    area.rooms[1000] = room1
    area.rooms[1001] = room2
    
    # Write the area to a file
    test_file = tmp_path / "test_exits.are"
    writer = AreWriter(area)
    writer.write(test_file)
    
    # Parse the file back
    parser = AreParser(str(test_file))
    parsed_area = parser.parse()
    
    # Verify the descriptions were preserved
    parsed_room1 = parsed_area.rooms[1000]
    assert parsed_room1.exits[0].description == "You see nothing special here."
    assert parsed_room1.exits[1].description == "A narrow passage leads east."
    
    parsed_room2 = parsed_area.rooms[1001]
    assert parsed_room2.exits[2].description == "You see nothing special here."


def test_room_add_exit_uses_default_description():
    """Test that Room.add_exit() uses the default description."""
    room = Room(vnum=1000, name="Test Room")
    
    # Add an exit using the room method
    exit_obj = room.add_exit(direction=0, to_room=1001)
    
    # Check that it has the default description
    assert exit_obj.description == "You see nothing special here."


def test_exit_description_in_written_file(tmp_path):
    """Test that the exit description appears correctly in the written file."""
    area = Area(
        name="Test Area",
        min_vnum=1000,
        max_vnum=1099,
        author="Test Author"
    )

    room = Room(
        vnum=1000,
        name="Test Room",
        description="A test room."
    )

    room.exits[0] = Exit(
        direction=0,
        to_room=1001,
        description="You see nothing special here."
    )

    area.rooms[1000] = room

    # Write the area to a file
    test_file = tmp_path / "test_exit_format.are"
    writer = AreWriter(area)
    writer.write(test_file)

    # Read the file and check the format
    content = test_file.read_text()

    # The file should contain the DOOR section with the description
    assert "DOOR 0" in content
    assert "You see nothing special here.~" in content


def test_direction_description_without_exit(tmp_path):
    """Test that we can add descriptions to directions without actual exits (to_room=0)."""
    area = Area(
        name="Test Area",
        min_vnum=1000,
        max_vnum=1099,
        author="Test Author"
    )

    room = Room(
        vnum=1000,
        name="Test Room",
        description="A test room."
    )

    # Add a description for north, but no actual exit (to_room=0)
    room.exits[0] = Exit(
        direction=0,
        to_room=0,  # No actual exit
        description="A solid stone wall blocks your path."
    )

    # Add an actual exit to the east
    room.exits[1] = Exit(
        direction=1,
        to_room=1001,
        description="A doorway leads east."
    )

    area.rooms[1000] = room

    # Write the area to a file
    test_file = tmp_path / "test_direction_desc.are"
    writer = AreWriter(area)
    writer.write(test_file)

    # Parse the file back
    parser = AreParser(str(test_file))
    parsed_area = parser.parse()

    # Verify both directions were preserved
    parsed_room = parsed_area.rooms[1000]
    assert 0 in parsed_room.exits
    assert 1 in parsed_room.exits

    # Verify the description-only direction
    assert parsed_room.exits[0].to_room == 0
    assert parsed_room.exits[0].description == "A solid stone wall blocks your path."

    # Verify the actual exit
    assert parsed_room.exits[1].to_room == 1001
    assert parsed_room.exits[1].description == "A doorway leads east."


def test_all_six_directions_can_have_descriptions(tmp_path):
    """Test that all 6 cardinal directions can have descriptions."""
    area = Area(
        name="Test Area",
        min_vnum=1000,
        max_vnum=1099,
        author="Test Author"
    )

    room = Room(
        vnum=1000,
        name="Test Room",
        description="A test room."
    )

    # Add descriptions for all 6 directions
    direction_descriptions = [
        "North: A mountain looms.",
        "East: A forest stretches.",
        "South: A river flows.",
        "West: A desert expands.",
        "Up: The sky is clear.",
        "Down: The ground is solid."
    ]

    for direction in range(6):
        room.exits[direction] = Exit(
            direction=direction,
            to_room=0,  # No actual exits, just descriptions
            description=direction_descriptions[direction]
        )

    area.rooms[1000] = room

    # Write and parse back
    test_file = tmp_path / "test_all_directions.are"
    writer = AreWriter(area)
    writer.write(test_file)

    parser = AreParser(str(test_file))
    parsed_area = parser.parse()

    # Verify all 6 directions were preserved
    parsed_room = parsed_area.rooms[1000]
    assert len(parsed_room.exits) == 6

    for direction in range(6):
        assert direction in parsed_room.exits
        assert parsed_room.exits[direction].to_room == 0
        assert parsed_room.exits[direction].description == direction_descriptions[direction]


def test_direction_with_keywords():
    """Test that direction descriptions can have keywords."""
    room = Room(
        vnum=1000,
        name="Test Room",
        description="A test room."
    )

    # Add a direction with keywords
    room.exits[0] = Exit(
        direction=0,
        to_room=0,
        description="A massive iron gate blocks the passage north.",
        keywords="gate iron massive"
    )

    # Verify the keywords are set
    assert room.exits[0].keywords == "gate iron massive"
    assert room.exits[0].description == "A massive iron gate blocks the passage north."

