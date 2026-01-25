#!/usr/bin/env python3
"""
Quick test script to verify new area creation works.
"""
from pathlib import Path
from area_editor.templates import create_new_area
from area_editor.writers.are_writer import AreWriter

# Create a new area
area = create_new_area(
    name="Test New Area",
    author="TestBuilder",
    min_vnum=5000,
    max_vnum=5099
)

print(f"Created area: {area.name}")
print(f"  Author: {area.author}")
print(f"  VNUMs: {area.min_vnum}-{area.max_vnum}")
print(f"  Rooms: {len(area.rooms)}")
print(f"  Room {area.min_vnum}: {area.rooms[area.min_vnum].name}")

# Write to a test file
output_path = Path("/tmp/test_new_area.are")
writer = AreWriter(area)
writer.write(output_path)

print(f"\nWrote area to: {output_path}")
print(f"File exists: {output_path.exists()}")
print(f"File size: {output_path.stat().st_size} bytes")

# Show the content
print("\n--- File Content ---")
print(output_path.read_text())

