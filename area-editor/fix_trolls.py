#!/usr/bin/env python3
"""
Script to fix the trolls.are file by re-parsing and re-writing it with the correct format.
"""
from pathlib import Path
from area_editor.parsers.are_parser import AreParser
from area_editor.writers.are_writer import AreWriter

# Parse the trolls.are file
trolls_path = Path("../area/trolls.are")
print(f"Parsing {trolls_path}...")
parser = AreParser(trolls_path)
area = parser.parse()

if area:
    print(f"Successfully parsed area: {area.name}")
    print(f"  Rooms: {len(area.rooms)}")
    print(f"  Objects: {len(area.objects)}")
    print(f"  Mobiles: {len(area.mobiles)}")
    
    # Write it back out with the correct format
    print(f"\nWriting corrected file to {trolls_path}...")
    writer = AreWriter(area)
    writer.write(trolls_path)
    print("Done!")
else:
    print("Failed to parse area file!")

