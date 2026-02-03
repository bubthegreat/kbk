"""
Tests for the validation system.
"""
import pytest
from area_editor.models import Area, Room, Object, Mobile, Shop, Reset
from area_editor.models.room import Exit
from area_editor.validation import AreaValidator, ValidationError, ValidationResult


def test_validation_result_creation():
    """Test creating a validation result."""
    result = ValidationResult(area_id="test.are")
    assert result.area_id == "test.are"
    assert len(result.errors) == 0
    assert len(result.warnings) == 0
    assert not result.has_errors()
    assert not result.has_warnings()


def test_validation_result_add_error():
    """Test adding errors to validation result."""
    result = ValidationResult(area_id="test.are")
    
    error = ValidationError(
        error_type="missing_room",
        severity="error",
        item_type="room",
        item_vnum=1000,
        message="Test error"
    )
    result.add_error(error)
    
    assert result.has_errors()
    assert len(result.errors) == 1
    assert result.errors[0].message == "Test error"


def test_validation_result_add_warning():
    """Test adding warnings to validation result."""
    result = ValidationResult(area_id="test.are")
    
    warning = ValidationError(
        error_type="empty_name",
        severity="warning",
        item_type="room",
        item_vnum=1000,
        message="Test warning"
    )
    result.add_error(warning)
    
    assert result.has_warnings()
    assert len(result.warnings) == 1
    assert result.warnings[0].message == "Test warning"


def test_validation_result_get_errors_for_item():
    """Test getting errors for a specific item."""
    result = ValidationResult(area_id="test.are")
    
    # Add errors for different items
    result.add_error(ValidationError(
        error_type="test", severity="error", item_type="room", item_vnum=1000, message="Error 1"
    ))
    result.add_error(ValidationError(
        error_type="test", severity="error", item_type="room", item_vnum=1001, message="Error 2"
    ))
    result.add_error(ValidationError(
        error_type="test", severity="warning", item_type="room", item_vnum=1000, message="Warning 1"
    ))
    
    # Get errors for room 1000
    errors_1000 = result.get_errors_for_item("room", 1000)
    assert len(errors_1000) == 2  # 1 error + 1 warning
    
    # Get errors for room 1001
    errors_1001 = result.get_errors_for_item("room", 1001)
    assert len(errors_1001) == 1


def test_validate_vnum_range():
    """Test validation of vnum ranges."""
    area = Area(name="Test Area", min_vnum=1000, max_vnum=1099)

    # Add a room within range
    area.rooms[1050] = Room(vnum=1050, name="Valid Room", description="A valid room")

    # Add a room outside range
    area.rooms[2000] = Room(vnum=2000, name="Invalid Room", description="An invalid room")

    validator = AreaValidator(area, "test.are")
    result = validator.validate()

    # Should have 1 error for the out-of-range room
    assert result.has_errors()
    errors = result.get_errors_for_item("room", 2000)
    assert len(errors) == 1
    assert "outside area range" in errors[0].message


def test_validate_room_exits():
    """Test validation of room exits."""
    area = Area(name="Test Area", min_vnum=1000, max_vnum=1099)

    # Add two rooms with descriptions
    room1 = Room(vnum=1000, name="Room 1", description="First room")
    room2 = Room(vnum=1001, name="Room 2", description="Second room")

    # Add exit from room1 to room2 (valid)
    room1.exits[0] = Exit(direction=0, to_room=1001)

    # Add exit from room1 to non-existent room (invalid)
    room1.exits[1] = Exit(direction=1, to_room=2000)

    # Add description-only direction (to_room=0) - should NOT trigger validation error
    room1.exits[2] = Exit(direction=2, to_room=0, description="A solid wall blocks the path.")

    area.rooms[1000] = room1
    area.rooms[1001] = room2

    validator = AreaValidator(area, "test.are")
    result = validator.validate()

    # Should have 1 warning for the missing room reference
    # Should NOT have a warning for the description-only direction (to_room=0)
    assert result.has_warnings()
    errors = result.get_errors_for_item("room", 1000)
    assert len(errors) == 1
    assert "does not exist" in errors[0].message


def test_validate_empty_room_name():
    """Test validation of empty room names."""
    area = Area(name="Test Area", min_vnum=1000, max_vnum=1099)
    
    # Add room with empty name
    area.rooms[1000] = Room(vnum=1000, name="")
    
    validator = AreaValidator(area, "test.are")
    result = validator.validate()
    
    # Should have 1 warning for empty name
    assert result.has_warnings()
    errors = result.get_errors_for_item("room", 1000)
    assert any("no name" in e.message for e in errors)


def test_validate_empty_room_description():
    """Test validation of empty room descriptions."""
    area = Area(name="Test Area", min_vnum=1000, max_vnum=1099)

    # Add room with empty description
    area.rooms[1000] = Room(vnum=1000, name="Test Room", description="")

    validator = AreaValidator(area, "test.are")
    result = validator.validate()

    # Should have 1 warning for empty description
    assert result.has_warnings()
    errors = result.get_errors_for_item("room", 1000)
    assert any("no description" in e.message for e in errors)


def test_validate_shop_missing_keeper():
    """Test validation of shops with missing keeper mobiles."""
    area = Area(name="Test Area", min_vnum=1000, max_vnum=1099)

    # Add a shop with a keeper that doesn't exist
    shop = Shop(keeper=1050, buy_types=[1, 2, 3, 0, 0])
    area.shops.append(shop)

    validator = AreaValidator(area, "test.are")
    result = validator.validate()

    # Should have 1 error for missing keeper
    assert result.has_errors()
    errors = result.get_errors_for_item("shop", 1050)
    assert len(errors) == 1
    assert "keeper" in errors[0].message.lower()


def test_validate_shop_with_valid_keeper():
    """Test validation of shops with valid keeper mobiles."""
    area = Area(name="Test Area", min_vnum=1000, max_vnum=1099)

    # Add a mobile
    area.mobiles[1050] = Mobile(vnum=1050, short_description="a shopkeeper")

    # Add a shop with that mobile as keeper
    shop = Shop(keeper=1050, buy_types=[1, 2, 3, 0, 0])
    area.shops.append(shop)

    validator = AreaValidator(area, "test.are")
    result = validator.validate()

    # Should have no errors for the shop
    errors = result.get_errors_for_item("shop", 1050)
    assert len(errors) == 0


def test_validate_reset_missing_mobile():
    """Test validation of M resets with missing mobiles."""
    area = Area(name="Test Area", min_vnum=1000, max_vnum=1099)

    # Add a room
    area.rooms[1000] = Room(vnum=1000, name="Test Room", description="A test room")

    # Add a reset for a mobile that doesn't exist
    reset = Reset(command='M', arg1=1050, arg2=1, arg3=1000, arg4=1)
    area.resets.append(reset)

    validator = AreaValidator(area, "test.are")
    result = validator.validate()

    # Should have 1 error for missing mobile
    assert result.has_errors()
    # Check for errors related to the mobile vnum
    all_errors = result.errors + result.warnings
    mobile_errors = [e for e in all_errors if e.item_vnum == 1050 and "mobile" in e.message.lower()]
    assert len(mobile_errors) >= 1


def test_validate_reset_missing_room():
    """Test validation of M resets with missing rooms."""
    area = Area(name="Test Area", min_vnum=1000, max_vnum=1099)

    # Add a mobile
    area.mobiles[1050] = Mobile(vnum=1050, short_description="a guard")

    # Add a reset for a room that doesn't exist
    reset = Reset(command='M', arg1=1050, arg2=1, arg3=2000, arg4=1)
    area.resets.append(reset)

    validator = AreaValidator(area, "test.are")
    result = validator.validate()

    # Should have 1 error for missing room
    assert result.has_errors()
    # Check for errors related to the room vnum
    all_errors = result.errors + result.warnings
    room_errors = [e for e in all_errors if e.item_vnum == 2000 and "room" in e.message.lower()]
    assert len(room_errors) >= 1


def test_validate_reset_valid_mobile():
    """Test validation of M resets with valid mobiles and rooms."""
    area = Area(name="Test Area", min_vnum=1000, max_vnum=1099)

    # Add a room and mobile
    area.rooms[1000] = Room(vnum=1000, name="Test Room", description="A test room")
    area.mobiles[1050] = Mobile(vnum=1050, short_description="a guard")

    # Add a valid reset
    reset = Reset(command='M', arg1=1050, arg2=1, arg3=1000, arg4=1)
    area.resets.append(reset)

    validator = AreaValidator(area, "test.are")
    result = validator.validate()

    # Should have no errors for the reset
    # (there might be warnings for other things, but no errors about missing mobile/room)
    all_errors = result.errors + result.warnings
    reset_errors = [e for e in all_errors if e.item_type == "reset"]
    assert len(reset_errors) == 0


def test_validate_reset_missing_object():
    """Test validation of O resets with missing objects."""
    area = Area(name="Test Area", min_vnum=1000, max_vnum=1099)

    # Add a room
    area.rooms[1000] = Room(vnum=1000, name="Test Room", description="A test room")

    # Add a reset for an object that doesn't exist
    reset = Reset(command='O', arg1=1060, arg2=0, arg3=1000, arg4=0)
    area.resets.append(reset)

    validator = AreaValidator(area, "test.are")
    result = validator.validate()

    # Should have 1 error for missing object
    assert result.has_errors()
    all_errors = result.errors + result.warnings
    object_errors = [e for e in all_errors if e.item_vnum == 1060 and "object" in e.message.lower()]
    assert len(object_errors) >= 1

