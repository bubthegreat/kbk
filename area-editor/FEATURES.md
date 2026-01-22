# KBK Area Editor - Features

This document outlines all implemented features in the Area Editor. This ensures AI agents and developers know which functionality must be preserved when making changes.

---

## File Parsing

**Description**: Parse .are files in the ROM 2.4 format with support for AREADATA, ROOMDATA, MOBDATA, and OBJDATA sections.

**How to Test**:
1. Run `uv run python -m area_editor`
2. File > Open Area
3. Select `arkham.are` or `tests/test.are`
4. Verify area loads without errors
5. Check that rooms, objects, and mobiles appear in the tree

**Additional Notes**:
- Supports tilde-terminated strings
- Handles bit flag notation (e.g., "1|2|4")
- Handles letter-coded flags (e.g., "AB" = 3, "ACDEFHIJKN")
- Parses extra descriptions (EDESC)
- Parses room exits with doors, keywords, and locks
- Parses mobile stats, dice notation, and flags
- Parses object types, values, affects, and extra descriptions

**Related Tests**: `tests/test_parser.py`

---

## Multi-Area Support

**Description**: Load and manage multiple .are files simultaneously with the ability to switch between them.

**How to Test**:
1. Run `uv run python -m area_editor`
2. File > Open Area > Select `arkham.are`
3. File > Open Area > Select `tests/test.are`
4. Verify both areas appear in the tree
5. Click items in different areas to switch between them
6. Verify status bar shows the active area

**Additional Notes**:
- Each area gets its own tree node
- Areas are stored in a dictionary with area_id (filename) as key
- Current area is tracked with `current_area_id`
- "No areas loaded" placeholder is removed when first area loads
- Selection tracking includes area_id: `(area_id, item_type, vnum)`

**Related Tests**: Manual testing required (UI feature)

---

## Area Tree Navigation

**Description**: Hierarchical tree view showing areas, rooms, objects, and mobiles with click-to-select functionality.

**How to Test**:
1. Load an area file
2. Expand the area node in the tree
3. Click on different rooms, objects, and mobiles
4. Verify the editor panel updates to show the selected item
5. Verify the properties panel updates with item details

**Additional Notes**:
- Tree shows item counts (e.g., "Rooms (174)")
- Clicking an item updates app_state selection
- Previous selection is properly deselected
- Tree supports multiple areas simultaneously

**Related Tests**: Manual testing required (UI feature)

---

## Room Editor

**Description**: View and edit room properties including name, description, exits, and extra descriptions.

**How to Test**:
1. Load an area file
2. Click on a room in the tree
3. Verify room name and description are displayed
4. Edit the name or description
5. Verify the status bar shows an asterisk (*) indicating changes
6. Verify exits are displayed with clickable links
7. Click an exit link to navigate to the connected room

**Additional Notes**:
- Name and description fields are editable
- Changes are tracked with `app_state.mark_modified()`
- Exit links are clickable and navigate to connected rooms
- Extra descriptions are displayed (keywords + text)
- Supports all 6 directions: north, east, south, west, up, down

**Related Tests**: Manual testing required (UI feature)

---

## Object Editor

**Description**: View and edit object properties including descriptions, type, stats, and affects.

**How to Test**:
1. Load an area file
2. Click on an object in the tree
3. Verify short and long descriptions are displayed
4. Edit the descriptions
5. Verify changes are tracked
6. Verify extra descriptions are shown
7. Verify affects are displayed

**Additional Notes**:
- Short and long description fields are editable
- Extra descriptions are displayed (keywords + text)
- Affects show apply type and modifier (e.g., "hitroll: +2")
- Shows first 5 affects, with "... and X more" if there are more

**Related Tests**: Manual testing required (UI feature)

---

## Mobile Editor

**Description**: View and edit mobile (NPC) properties including descriptions, race, level, and stats.

**How to Test**:
1. Load an area file
2. Click on a mobile in the tree
3. Verify short and long descriptions are displayed
4. Edit the descriptions
5. Verify changes are tracked
6. Verify mobile stats are shown in properties panel

**Additional Notes**:
- Short and long description fields are editable
- Properties panel shows race, level, alignment, combat stats
- Armor class values: ac_pierce, ac_bash, ac_slash, ac_exotic (NOT ac_magic)

**Related Tests**: Manual testing required (UI feature)

---

## Properties Panel

**Description**: Quick reference panel showing key properties of the selected item.

**How to Test**:
1. Load an area file
2. Click on different items (rooms, objects, mobiles, area)
3. Verify the properties panel updates with relevant information
4. For rooms: check exit count, extra descriptions, sector, flags
5. For objects: check type, level, weight, cost, affects
6. For mobiles: check race, level, combat stats

**Additional Notes**:
- Updates automatically when selection changes
- Shows different properties based on item type
- Provides quick reference without scrolling in editor panel
- Extra descriptions are counted and displayed

**Related Tests**: Manual testing required (UI feature)

---

## Keyboard Navigation

**Description**: Navigate through rooms using arrow keys (Up/Down).

**How to Test**:
1. Load an area file
2. Click on a room in the tree
3. Press Down Arrow (↓) to go to next room
4. Press Up Arrow (↑) to go to previous room
5. Verify tree selection, editor panel, and properties panel all update
6. Verify navigation wraps around (last → first, first → last)

**Additional Notes**:
- Only active when a room is selected
- Navigates in VNUM order
- Wraps around at boundaries
- Updates all UI components (tree, editor, properties)
- Hotkeys work globally when not typing in a text field

**Related Tests**: Manual testing required (UI feature)

---

## MUD Terminal Simulator

**Description**: Interactive terminal for testing area navigation from a player's perspective.

**How to Test**:
1. Load `tests/test.are`
2. Click on Room #1000 (The Town Square)
3. Click "Open Terminal" button
4. Type "north" and press Enter to move to the Inn
5. Type "up" to go to Guest Rooms
6. Type "down" to return to Inn
7. Type "south" to return to Town Square
8. Type "look fountain" to see extra description
9. Verify UI selection updates as you move

**Additional Notes**:
- **Movement commands**: north (n), south (s), east (e), west (w), up (u), down (d)
- **Look command**: look (shows current room), look <target> (looks at specific things)
- **Help command**: Shows available commands
- **Quit command**: Closes the terminal window
- **Command repeat**: Press Enter on empty input to repeat last command
- **Auto-focus**: Input field stays focused after each command
- **Auto-select**: Previous command stays highlighted - type to replace, Enter to repeat
- **Auto-show room**: Room is displayed automatically when terminal opens
- **Auto-scroll**: Terminal automatically scrolls to bottom when new output is added
- **Color-coded output**: Room names (cyan), exits (gold), commands (green), errors (red)
- **Selectable text**: All terminal output is selectable for copy/paste
- **UI synchronization**: When navigating in terminal, the selected room in the UI updates automatically
- Simplified simulator - no combat, timing, or lag mechanics

**Related Tests**: `tests/test_mud_terminal.py`

---

