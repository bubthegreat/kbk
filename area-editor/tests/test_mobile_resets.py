"""
Tests for mobile resets and shop functionality.
"""
import pytest
from area_editor.parsers.are_parser import AreParser
from area_editor.writers.are_writer import AreWriter
from area_editor.models.area import Area
from area_editor.models.reset import Reset
from area_editor.models.shop import Shop
from area_editor.models.mobile import Mobile
from pathlib import Path


def test_parse_mobile_resets():
    """Test parsing mobile resets from test.are."""
    parser = AreParser("tests/test.are")
    area = parser.parse()
    
    # Check that we have mobile resets
    mobile_resets = [r for r in area.resets if r.command == 'M']
    assert len(mobile_resets) > 0, "Should have at least one mobile reset"
    
    # Check first mobile reset (innkeeper in room 1001)
    reset = mobile_resets[0]
    assert reset.command == 'M'
    assert reset.arg1 == 1050  # innkeeper vnum
    assert reset.arg2 == 1     # max in world
    assert reset.arg3 == 1001  # room vnum
    assert reset.arg4 == 1     # max in room


def test_parse_shops():
    """Test parsing shops from test.are."""
    parser = AreParser("tests/test.are")
    area = parser.parse()
    
    # Check that we have shops
    assert len(area.shops) > 0, "Should have at least one shop"
    
    # Check shopkeeper shop
    shop = area.shops[0]
    assert shop.keeper == 1051  # shopkeeper vnum
    assert shop.profit_buy == 120
    assert shop.profit_sell == 80
    assert shop.open_hour == 0
    assert shop.close_hour == 23


def test_create_mobile_reset():
    """Test creating a new mobile reset."""
    area = Area(
        name="Test Area",
        min_vnum=1000,
        max_vnum=1099
    )
    
    # Add a mobile
    mob = Mobile(
        vnum=1000,
        keywords="test mob",
        short_description="a test mobile",
        long_description="A test mobile stands here.",
        level=10
    )
    area.mobiles[1000] = mob
    
    # Create a mobile reset
    reset = Reset(
        command='M',
        arg1=1000,  # mobile vnum
        arg2=2,     # max in world
        arg3=1001,  # room vnum
        arg4=1      # max in room
    )
    area.resets.append(reset)
    
    # Verify
    assert len(area.resets) == 1
    assert area.resets[0].command == 'M'
    assert area.resets[0].arg1 == 1000
    assert area.resets[0].arg2 == 2
    assert area.resets[0].arg3 == 1001
    assert area.resets[0].arg4 == 1


def test_create_shop():
    """Test creating a new shop."""
    area = Area(
        name="Test Area",
        min_vnum=1000,
        max_vnum=1099
    )
    
    # Add a mobile
    mob = Mobile(
        vnum=1000,
        keywords="shopkeeper",
        short_description="a shopkeeper",
        long_description="A shopkeeper stands here.",
        level=10
    )
    area.mobiles[1000] = mob
    
    # Create a shop
    shop = Shop(
        keeper=1000,
        buy_types=[1, 2, 3, 0, 0],
        profit_buy=120,
        profit_sell=80,
        open_hour=6,
        close_hour=20
    )
    area.shops.append(shop)
    
    # Verify
    assert len(area.shops) == 1
    assert area.shops[0].keeper == 1000
    assert area.shops[0].profit_buy == 120
    assert area.shops[0].profit_sell == 80
    assert area.shops[0].open_hour == 6
    assert area.shops[0].close_hour == 20
    assert area.shops[0].buy_types == [1, 2, 3, 0, 0]


def test_write_mobile_resets_and_shops(tmp_path):
    """Test writing mobile resets and shops to a file."""
    area = Area(
        name="Test Area",
        min_vnum=1000,
        max_vnum=1099
    )
    
    # Add a mobile
    mob = Mobile(
        vnum=1000,
        keywords="shopkeeper",
        short_description="a shopkeeper",
        long_description="A shopkeeper stands here.",
        level=10
    )
    area.mobiles[1000] = mob
    
    # Add mobile reset
    reset = Reset(
        command='M',
        arg1=1000,
        arg2=1,
        arg3=1001,
        arg4=1
    )
    area.resets.append(reset)
    
    # Add shop
    shop = Shop(
        keeper=1000,
        buy_types=[1, 2, 0, 0, 0],
        profit_buy=120,
        profit_sell=80,
        open_hour=0,
        close_hour=23
    )
    area.shops.append(shop)
    
    # Write to file
    output_file = tmp_path / "test_output.are"
    writer = AreWriter(area)
    writer.write(output_file)
    
    # Parse it back
    parser = AreParser(str(output_file))
    parsed_area = parser.parse()
    
    # Verify mobile reset
    assert len(parsed_area.resets) == 1
    assert parsed_area.resets[0].command == 'M'
    assert parsed_area.resets[0].arg1 == 1000
    
    # Verify shop
    assert len(parsed_area.shops) == 1
    assert parsed_area.shops[0].keeper == 1000
    assert parsed_area.shops[0].profit_buy == 120

