"""
Tests for the .are file parser.
"""
import pytest
from pathlib import Path
from area_editor.parsers.are_parser import AreParser


def test_parse_limbo_area():
    """Test parsing the limbo.are file."""
    # Path relative to the kbk root directory
    test_dir = Path(__file__).parent
    limbo_path = test_dir.parent.parent / "area" / "limbo.are"

    if not limbo_path.exists():
        pytest.skip(f"Limbo area file not found at {limbo_path}")
    
    parser = AreParser(str(limbo_path))
    area = parser.parse()
    
    assert area is not None
    assert area.name == "Limbo"
    assert area.min_vnum == 1
    assert area.max_vnum == 110
    assert area.security == 9


def test_base_parser_read_string():
    """Test the base parser's read_string method."""
    from area_editor.parsers.base_parser import BaseParser
    from io import StringIO
    
    test_data = "This is a test~\nNext line"
    f = StringIO(test_data)
    parser = BaseParser(f)
    parser.read_line()
    
    result = parser.read_string()
    assert result == "This is a test"


def test_base_parser_read_number_letter_codes():
    """Test parsing letter-coded numbers."""
    from area_editor.parsers.base_parser import BaseParser
    from io import StringIO

    # Test letter codes (A=1, B=2, AB=3, C=4, AC=5, etc.)
    test_data = "AB 0 C 1|2|4\n"
    parser = BaseParser(StringIO(test_data))
    parser.read_line()

    # AB = A(bit 0) + B(bit 1) = 1 + 2 = 3
    assert parser.read_number() == 3

    # 0 = 0
    assert parser.read_number() == 0

    # C = bit 2 = 4
    assert parser.read_number() == 4

    # 1|2|4 = 7
    assert parser.read_number() == 7


def test_base_parser_read_number():
    """Test the base parser's read_number method."""
    from area_editor.parsers.base_parser import BaseParser
    from io import StringIO

    # Test simple number
    f = StringIO("42")
    parser = BaseParser(f)
    parser.read_line()
    assert parser.read_number() == 42

    # Test bit flag notation
    f = StringIO("1|2|4")
    parser = BaseParser(f)
    parser.read_line()
    assert parser.read_number() == 7


def test_parse_rooms():
    """Test parsing rooms from limbo.are."""
    test_dir = Path(__file__).parent
    limbo_path = test_dir.parent.parent / "area" / "limbo.are"

    if not limbo_path.exists():
        pytest.skip(f"Limbo area file not found at {limbo_path}")

    parser = AreParser(str(limbo_path))
    area = parser.parse()

    assert area is not None
    assert len(area.rooms) > 0

    # Check specific room (The Void - vnum 1)
    room1 = area.rooms.get(1)
    assert room1 is not None
    assert room1.name == "The Void"
    assert "floating in nothing" in room1.description
    assert room1.sector_type == 3

    # Check room with exits (Tracerah Land - vnum 4)
    room4 = area.rooms.get(4)
    assert room4 is not None
    assert room4.name == "Tracerah Land"
    assert len(room4.exits) == 6  # Has exits in all 6 directions

    # Check specific exit
    north_exit = room4.exits.get(0)  # North
    assert north_exit is not None
    assert north_exit.to_room == 3014


def test_parse_mobiles():
    """Test parsing mobiles from limbo.are."""
    test_dir = Path(__file__).parent
    limbo_path = test_dir.parent.parent / "area" / "limbo.are"

    if not limbo_path.exists():
        pytest.skip(f"Limbo area file not found at {limbo_path}")

    parser = AreParser(str(limbo_path))
    area = parser.parse()

    assert area is not None
    assert len(area.mobiles) > 0

    # Check specific mobile (massive lich - vnum 3)
    mob3 = area.mobiles.get(3)
    assert mob3 is not None
    assert mob3.short_description == "a massive lich"
    assert mob3.race == "lich"
    assert mob3.level == 100
    assert mob3.hitroll == 60
    assert mob3.hit_dice == "100d100+1000"
    assert mob3.damage_dice == "100d100+50"

    # Check Trogdor (vnum 4)
    mob4 = area.mobiles.get(4)
    assert mob4 is not None
    assert mob4.short_description == "Trogdor the Burninator"
    assert mob4.race == "dragon"
    assert mob4.alignment == -1000


def test_parse_objects():
    """Test parsing objects from limbo.are."""
    test_dir = Path(__file__).parent
    limbo_path = test_dir.parent.parent / "area" / "limbo.are"

    if not limbo_path.exists():
        pytest.skip(f"Limbo area file not found at {limbo_path}")

    parser = AreParser(str(limbo_path))
    area = parser.parse()

    assert area is not None
    assert len(area.objects) > 0

    # Check simple object (silver coin - vnum 1)
    obj1 = area.objects.get(1)
    assert obj1 is not None
    assert obj1.short_description == "a silver coin"
    assert obj1.item_type == "money"
    assert obj1.material == "silver"

    # Check object with affects (Korinth d'Verin - vnum 6)
    obj6 = area.objects.get(6)
    assert obj6 is not None
    assert obj6.short_description == "the Korinth d'Verin"
    assert obj6.item_type == "weapon"
    assert len(obj6.affects) == 2
    assert obj6.limit == 1

    # Check object with extra descriptions (potion of return - vnum 7)
    obj7 = area.objects.get(7)
    assert obj7 is not None
    assert obj7.short_description == "a potion of return"
    assert obj7.item_type == "potion"
    assert len(obj7.extra_descriptions) == 2


if __name__ == "__main__":
    pytest.main([__file__, "-v"])

