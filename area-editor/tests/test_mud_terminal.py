"""
Tests for the MUD Terminal UI synchronization feature.
"""
import pytest
from pathlib import Path
from area_editor.parsers.are_parser import AreParser
from area_editor.app_state import app_state
from area_editor.ui.mud_terminal import MudTerminal


@pytest.fixture
def test_area():
    """Load the test area for testing."""
    test_dir = Path(__file__).parent
    test_are_path = test_dir / "test.are"
    
    parser = AreParser(str(test_are_path))
    area = parser.parse()
    
    # Load into app state
    app_state.load_area(area, test_are_path)
    
    yield area
    
    # Cleanup
    app_state.areas.clear()
    app_state.area_files.clear()
    app_state.area_modified.clear()
    app_state.current_area_id = None


def test_test_area_structure(test_area):
    """Test that the test area has the expected structure."""
    assert test_area.name == "The Village of Turgon"
    assert test_area.min_vnum == 1000
    assert test_area.max_vnum == 1099
    assert len(test_area.rooms) == 8
    
    # Verify key rooms exist
    assert 1000 in test_area.rooms  # Town Square
    assert 1001 in test_area.rooms  # Village Inn
    assert 1005 in test_area.rooms  # Guest Rooms
    
    # Verify Town Square has 4 exits (north, east, south, west)
    town_square = test_area.rooms[1000]
    assert len(town_square.exits) == 4
    assert 0 in town_square.exits  # north to inn
    assert 1 in town_square.exits  # east to store
    assert 2 in town_square.exits  # south to alley
    assert 3 in town_square.exits  # west to gate


def test_navigation_path_town_square_to_inn(test_area):
    """Test navigation from Town Square to Inn."""
    town_square = test_area.rooms[1000]
    
    # Verify north exit leads to Inn (1001)
    assert 0 in town_square.exits
    north_exit = town_square.exits[0]
    assert north_exit.to_room == 1001
    
    # Verify Inn exists and has south exit back to Town Square
    inn = test_area.rooms[1001]
    assert 2 in inn.exits  # south
    south_exit = inn.exits[2]
    assert south_exit.to_room == 1000


def test_navigation_path_inn_to_guest_rooms(test_area):
    """Test navigation from Inn to Guest Rooms (up/down)."""
    inn = test_area.rooms[1001]
    
    # Verify up exit leads to Guest Rooms (1005)
    assert 4 in inn.exits  # up
    up_exit = inn.exits[4]
    assert up_exit.to_room == 1005
    
    # Verify Guest Rooms has down exit back to Inn
    guest_rooms = test_area.rooms[1005]
    assert 5 in guest_rooms.exits  # down
    down_exit = guest_rooms.exits[5]
    assert down_exit.to_room == 1001


def test_navigation_path_alley_to_dead_end(test_area):
    """Test navigation from Dark Alley to Dead End."""
    alley = test_area.rooms[1003]
    
    # Verify east exit leads to Dead End (1006)
    assert 1 in alley.exits  # east
    east_exit = alley.exits[1]
    assert east_exit.to_room == 1006
    
    # Verify Dead End has west exit back to Alley
    dead_end = test_area.rooms[1006]
    assert 3 in dead_end.exits  # west
    west_exit = dead_end.exits[3]
    assert west_exit.to_room == 1003


def test_extra_descriptions_in_rooms(test_area):
    """Test that rooms have extra descriptions for 'look' commands."""
    town_square = test_area.rooms[1000]
    assert len(town_square.extra_descriptions) == 2
    
    # Check for fountain extra description
    fountain_edesc = next((ed for ed in town_square.extra_descriptions 
                          if 'fountain' in ed.keywords.lower()), None)
    assert fountain_edesc is not None
    assert 'cracked stone' in fountain_edesc.description.lower()
    
    # Check Inn has extra descriptions
    inn = test_area.rooms[1001]
    assert len(inn.extra_descriptions) == 2


def test_mud_terminal_initialization():
    """Test that MUD terminal can be initialized."""
    terminal = MudTerminal()
    assert terminal.window_id is None
    assert terminal.current_room_vnum is None
    assert terminal.command_history == []


def test_mud_terminal_with_main_window():
    """Test that MUD terminal can be initialized with main_window reference."""
    class MockMainWindow:
        pass
    
    main_window = MockMainWindow()
    terminal = MudTerminal(main_window)
    assert terminal.main_window is main_window


def test_app_state_selection_tracking(test_area):
    """Test that app_state correctly tracks room selection."""
    # Select a room
    app_state.select_item('room', 1000)
    
    assert app_state.selected_item_type == 'room'
    assert app_state.selected_item_vnum == 1000
    
    # Get selected item
    selected = app_state.get_selected_item()
    assert selected is not None
    assert selected.vnum == 1000
    assert selected.name == "The Town Square"


def test_circular_navigation_path(test_area):
    """Test a circular navigation path: Town Square → Inn → Town Square."""
    # Start at Town Square
    current_room = test_area.rooms[1000]
    assert current_room.name == "The Town Square"
    
    # Go north to Inn
    assert 0 in current_room.exits
    next_vnum = current_room.exits[0].to_room
    current_room = test_area.rooms[next_vnum]
    assert current_room.name == "The Village Inn"
    assert next_vnum == 1001
    
    # Go south back to Town Square
    assert 2 in current_room.exits
    next_vnum = current_room.exits[2].to_room
    current_room = test_area.rooms[next_vnum]
    assert current_room.name == "The Town Square"
    assert next_vnum == 1000

