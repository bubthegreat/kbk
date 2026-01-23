# KBK Area Editor - Features

This document outlines all implemented features in the Area Editor. This ensures AI agents and developers know which functionality must be preserved when making changes.

---

## Font Scaling

**Description**: Automatic font scaling based on screen resolution with manual override options.

**How to Test**:
1. Run `uv run python -m area_editor`
2. By default, font size auto-scales relative to 1920x1080 baseline (doubled for readability):
   - Uses geometric mean of width/height ratios, then doubles it
   - Example: 1280x720 → 1.25x, 1920x1080 → 2.0x, 2388x1495 → 2.5x, 3840x2160 → 3.0x
   - Rounds to nearest 0.25 increment (0.75, 1.0, 1.25, 1.5, etc.)
   - Recalculates every time you launch based on current window size
3. View > Font Size > Select "Auto (Based on Resolution)" to re-enable auto-scaling
4. View > Font Size > Select manual sizes (75% to 300%) to disable auto-scaling
5. Close and reopen - preference is saved (auto or manual)

**Additional Notes**:
- Uses DearPyGui's `set_global_font_scale()` function
- Font scale stored in config: `~/.kbk_area_editor/config.json`
- Baseline: 1920x1080 = 2.0x (doubled for better readability)
- Auto-scaling formula: `sqrt((width/1920) * (height/1080)) * 2.0`
- Clamped to range: 0.75x to 3.0x
- Default mode: Auto-scaling enabled (`font_scale_auto: true`)
- Manual mode: Selecting a specific size disables auto-scaling
- Available manual sizes: 75%, 100%, 125%, 150%, 175%, 200%, 250%, 300%
- Changes apply immediately without restart

**Related Tests**: `tests/test_config.py::test_config_get_set_font_scale`

---

## Command Line File Loading

**Description**: Load area files directly from the command line for testing and convenience.

**How to Test**:
1. Run `uv run area-editor --help` to see usage
2. Run `uv run area-editor --file tests/test.are` to load test.are on startup
3. Run `uv run area-editor -f ../area/limbo.are` to load limbo.are on startup
4. Verify the file loads automatically when the editor opens
5. Test error handling: `uv run area-editor --file nonexistent.are` (should show error and exit)

**Additional Notes**:
- Uses Python's `argparse` module for command line parsing
- Validates file existence before launching GUI
- Warns if file doesn't have `.are` extension but still loads it
- File path can be relative or absolute
- Useful for:
  - **Testing**: Quickly load test files during development
  - **Convenience**: Skip the File > Open dialog for frequently edited files
  - **Automation**: Script the editor to open specific files
- Syntax: `area-editor [-f|--file PATH]`

**Related Files**: `src/area_editor/main.py`, `src/area_editor/__main__.py`

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

## Creating New Mobiles

**Description**: Create new mobiles (NPCs) in the area with a user-friendly modal interface.

**How to Test**:
1. Open an area file or create a new area
2. Go to any room's "Mobiles" tab in the room editor
3. Click "Create New Mobile" button
4. In the modal, fill in:
   - **Vnum**: Auto-assigned (next available in area range)
   - **Keywords**: Space-separated words for targeting (e.g., "guard town soldier")
   - **Short Description**: How the mobile appears in the room (e.g., "a town guard")
   - **Long Description** (optional): Detailed description when examining (e.g., "This guard watches over the town.")
   - **Level**: Mobile's level (1-60)
   - **Alignment**: -1000 (evil) to 1000 (good), 0 = neutral
5. Click "Create Mobile" to create the mobile
6. The mobile editor will open automatically
7. If creating from a room's Mobiles tab, you'll be prompted to add the mobile to that room

**Additional Notes**:
- Automatically assigns next available vnum in the area's range
- Creates sensible defaults for stats based on level (HP, mana, damage dice, etc.)
- Mobile appears in the Area Explorer under "Mobiles" section
- Can also be triggered when trying to add a mobile to a room when none exist
- All mobile fields can be edited after creation in the mobile editor
- **ROM MUD Field Mapping**:
  - `short_description`: Appears in room listings (e.g., "a town guard")
  - `long_description`: Shown when you examine/look at the mobile
  - `description`: Additional detailed text (DESCR field in ROM format)

**Related Tests**: N/A (UI feature)

---

## Bidirectional Dig Command

**Description**: Create new rooms or link to existing rooms with automatic bidirectional exits using the `dig` command in the MUD terminal.

**How to Test**:
1. Load `tests/test.are`
2. Click on Room #1000 (The Town Square)
3. Click "Open Terminal" button
4. **Test creating a new room**:
   - Type `dig up` and press Enter
   - A new room is created with the next available vnum
   - An exit "up" is created from Town Square to the new room
   - An exit "down" is automatically created from the new room back to Town Square
   - You are automatically moved to the new room
   - The area file is saved automatically
5. **Test linking to an existing room**:
   - Navigate to a room (e.g., Room #1002 - General Store)
   - Type `dig south 1004` to link to the Village Gate (room #1004)
   - A bidirectional link is created: south from Store to Gate, north from Gate to Store
   - You are automatically moved to the target room
   - The area file is saved automatically
6. **Test error handling**:
   - Try `dig north` in a direction that already has an exit (should show error)
   - Try `dig east 9999` to link to a non-existent room (should show error)
   - Try `dig invalid` with invalid direction (should show usage)

**Additional Notes**:
- **Syntax**: `dig <direction> [vnum]`
  - `dig <direction>` - Creates a new room in that direction with bidirectional exits
  - `dig <direction> <vnum>` - Links to existing room with bidirectional exits
- **Directions**: north (n), south (s), east (e), west (w), up (u), down (d)
- **Bidirectional exits**: Always creates reverse exit automatically
  - north ↔ south
  - east ↔ west
  - up ↔ down
- **Auto-save**: Changes are saved to the area file immediately
- **Auto-navigation**: After digging, you are moved to the new/target room
- **UI sync**: The area tree is refreshed to show new rooms
- **Vnum allocation**: When creating new rooms, uses the next available vnum in the area's range
- **Error prevention**: Prevents creating duplicate exits in the same direction
- **Validation**: Checks that target room exists before creating link
- Based on the ROM MUD OLC (Online Creation) `dig` and `link` commands

**Related Tests**: `tests/test_mud_terminal.py::test_dig_command_creates_new_room`, `tests/test_mud_terminal.py::test_dig_command_links_to_existing_room`, `tests/test_mud_terminal.py::test_dig_command_prevents_duplicate_exits`

---

## Mobile Resets in Rooms

**Description**: Add and manage mobile (NPC) spawns in rooms through the room editor's Mobiles tab.

**How to Test**:
1. Open an area file (e.g., `tests/test.are`)
2. Select a room from the Area Explorer
3. Click on the "Mobiles" tab in the room editor
4. Click "+ Add Mobile to Room" to open the mobile selection modal
   - If no mobiles exist, you'll be prompted to create one first
5. In the modal:
   - Select which mobile to spawn from the dropdown (shows all mobiles in the area)
   - Set "Max in World" (maximum instances in entire game)
   - Set "Max in Room" (maximum instances in this specific room)
   - Click "Add Mobile" to create the spawn, or "Cancel" to abort
6. For existing mobile spawns, you can:
   - Change which mobile spawns using the dropdown
   - Adjust max in world and max in room values
   - Click "Delete" to remove the spawn
7. Save the area and verify the mobile resets are written correctly

**Additional Notes**:
- Mobile resets use the 'M' command in the #RESETS section
- Format: `M 0 <mobile_vnum> <max_world> <room_vnum> <max_room>`
- The "0" is the if_flag (always 0 in modern ROM)
- Mobile resets are stored in `area.resets` list
- Each mobile reset creates one spawn point for that mobile in the room
- Multiple resets can spawn the same mobile in different rooms
- The MUD terminal will show mobiles in rooms based on these resets
- Deleting a mobile reset removes the spawn point but doesn't delete the mobile definition

**Related Tests**: `tests/test_mobile_resets.py`

---

## Shop Assignment to Mobiles

**Description**: Assign shops to mobiles, allowing them to buy and sell items to players.

**How to Test**:
1. Open an area file (e.g., `tests/test.are`)
2. Select a room that has a mobile spawn (see "Mobile Resets in Rooms" feature)
3. Click on the "Mobiles" tab in the room editor
4. Find a mobile in the list and click "Add Shop" button
5. A shop editor modal will appear with the following settings:
   - Buy Profit %: Percentage the shop pays when buying from players (default: 120%)
   - Sell Profit %: Percentage the shop charges when selling to players (default: 80%)
   - Open Hour: Hour the shop opens (0-23, default: 0)
   - Close Hour: Hour the shop closes (0-23, default: 23)
   - Item Types to Buy: Up to 5 item type numbers the shop will purchase (0 = none)
6. Click "Save" to create the shop
7. Click "Edit Shop" to modify an existing shop
8. Click "Remove Shop" to delete the shop from the mobile
9. Save the area and verify the shop is written to the #SHOPS section

**Additional Notes**:
- Shops are stored in `area.shops` list
- Format in #SHOPS section: `<keeper_vnum> <type1> <type2> <type3> <type4> <type5> <profit_buy> <profit_sell> <open_hour> <close_hour>`
- Each mobile can have only one shop
- The shop is linked to the mobile via the keeper vnum
- Item type numbers correspond to ROM item types (see ROM documentation)
- Profit percentages: 100% = nominal price, >100% = markup, <100% = discount
- Shop hours use 24-hour format (0 = midnight, 12 = noon, 23 = 11 PM)
- Shops are validated to ensure the keeper mobile exists in the area
- The shop interface is accessible from any room where the mobile spawns

**Related Tests**: `tests/test_mobile_resets.py::test_parse_shops`, `tests/test_mobile_resets.py::test_create_shop`

---

