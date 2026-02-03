"""
Test that the validator warns about objects in old format.
"""
from pathlib import Path
from area_editor.parsers.are_parser import AreParser
from area_editor.validation import AreaValidator


def test_old_format_object_warning(tmp_path):
    """Test that objects using old format (VALUES/STATS) generate warnings."""
    # Create a test area file with an object in old format
    test_file = tmp_path / "test_old_format.are"
    test_file.write_text("""#AREADATA
NAME Test Area~
AUTHOR TestAuthor~
LEVELS 1 50
VNUMS 1000 1099
SECURITY 9
FLAGS 0
End

#OBJDATA
#1000
NAME test object~
SHORT a test object~
DESCR A test object is here.~
TYPE treasure BC 0 0 0 0
EXTRA 1 0 0 0 0
VALUES 0 0 0 0 0
STATS 10 5 100 P
WEAR AE 0 0
End

#0

#ROOMDATA
#1000
NAME Test Room~
DESCR
This is a test room.~
FLAGS 0
SECTOR inside
EXITS
End

#0

#RESETS
S

#SHOPS
0

#SPECIALS
S

#$
""")
    
    # Parse the file
    parser = AreParser(test_file)
    area = parser.parse()
    
    assert area is not None
    assert 1000 in area.objects
    
    # Check that the object was marked as old format
    obj = area.objects[1000]
    assert hasattr(obj, '_loaded_from_old_format')
    assert obj._loaded_from_old_format is True
    
    # Run validation
    validator = AreaValidator(area, "test_old_format.are")
    result = validator.validate()
    
    # Should have warnings
    assert result.has_warnings()
    
    # Find the old format warning
    old_format_warnings = [w for w in result.warnings 
                          if w.error_type == "old_format" and w.item_vnum == 1000]
    
    assert len(old_format_warnings) == 1
    warning = old_format_warnings[0]
    assert warning.severity == "warning"
    assert warning.item_type == "object"
    assert "old format" in warning.message.lower()
    assert "VALUES/STATS" in warning.message


def test_new_format_object_no_warning(tmp_path):
    """Test that objects using new format (LEVEL/WEIGHT/COST/COND) do NOT generate warnings."""
    # Create a test area file with an object in new format
    test_file = tmp_path / "test_new_format.are"
    test_file.write_text("""#AREADATA
NAME Test Area~
AUTHOR TestAuthor~
LEVELS 1 50
VNUMS 1000 1099
SECURITY 9
FLAGS 0
End

#OBJDATA
#1000
NAME test object~
SHORT a test object~
DESCR A test object is here.~
MAT oldstyle~
TYPE treasure BC 0 0 0 0
EXTRA 0 0
WEAR AE
LEVEL 10
WEIGHT 5
COST 100
COND 100
End

#0

#ROOMDATA
#1000
NAME Test Room~
DESCR
This is a test room.~
FLAGS 0
SECTOR inside
EXITS
End

#0

#RESETS
S

#SHOPS
0

#SPECIALS
S

#$
""")
    
    # Parse the file
    parser = AreParser(test_file)
    area = parser.parse()
    
    assert area is not None
    assert 1000 in area.objects
    
    # Check that the object was NOT marked as old format
    obj = area.objects[1000]
    assert not (hasattr(obj, '_loaded_from_old_format') and obj._loaded_from_old_format)
    
    # Run validation
    validator = AreaValidator(area, "test_new_format.are")
    result = validator.validate()
    
    # Should NOT have old format warnings
    old_format_warnings = [w for w in result.warnings 
                          if w.error_type == "old_format"]
    
    assert len(old_format_warnings) == 0

